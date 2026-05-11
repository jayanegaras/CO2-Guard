#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "arduino_secrets.h"

// --- KONFIGURASI HARDWARE ---
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pinBaca = 17;      // Pin input tombol
const int pinGndBuatan = 4;  // Pin GND buatan untuk tombol
const int ledInternal = 2;   // LED Indikator

// --- TIMEZONE (WIB = GMT+7) ---
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200; 
const int daylightOffset_sec = 0;

// --- VARIABEL KONTROL ---
bool statusLed = false;       // Nyala atau mati
bool lastButtonState = HIGH;  // Untuk deteksi perubahan tekan
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

// --- FUNGSI UPDATE LAYAR ---
void drawOLED(String status, String info) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(0, 0);
  display.println("FIREBASE MONITOR");
  display.drawLine(0, 10, 127, 10, SH110X_WHITE);
  
  display.setCursor(0, 25);
  display.setTextSize(2);
  display.println(status);
  
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(info);
  
  display.display();
}

// --- FUNGSI AMBIL WAKTU ---
String getClock() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Waktu Gagal Sync";
  char buff[30];
  strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buff);
}

// --- FUNGSI KIRIM DATA (HTTPS) ---
void pushToFirebase(int val) {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client->setInsecure(); // Bypass SSL Certificate Check untuk cegah error -1
    
    HTTPClient http;
    String url = String(SECRET_FIREBASE_URL) + "?auth=" + String(SECRET_FIREBASE_API);
    
    if (http.begin(*client, url)) {
      http.addHeader("Content-Type", "application/json");
      
      // Data JSON
      String timestamp = getClock();
      String payload = "{\"waktu\":\"" + timestamp + "\", \"nilai\":" + String(val) + "}";
      
      int httpCode = http.POST(payload);
      
      if (httpCode > 0) {
        Serial.printf("[HTTP] POST Berhasil, kode: %d\n", httpCode);
        drawOLED(val ? "STATUS: ON" : "STATUS:OFF", "Sent: " + String(httpCode));
      } else {
        Serial.printf("[HTTP] POST Gagal, error: %s\n", http.errorToString(httpCode).c_str());
        drawOLED("ERR: " + String(httpCode), "Check Internet");
      }
      http.end();
    }
    delete client;
  }
}

void setup() {
  Serial.begin(115200);

  // Setup I/O
  pinMode(ledInternal, OUTPUT);
  pinMode(pinGndBuatan, OUTPUT);
  digitalWrite(pinGndBuatan, LOW); 
  pinMode(pinBaca, INPUT_PULLUP);

  // Setup OLED
  if (!display.begin(i2c_Address, true)) {
    Serial.println("SH1106 Gagal!");
    for (;;);
  }
  drawOLED("BOOTING", "Connecting WiFi...");

  // Setup WiFi
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK!");

  // Sinkronisasi Waktu
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  drawOLED("CONNECTED", "System Ready");
}

void loop() {
  int reading = digitalRead(pinBaca);

  // Cek kalau tombol baru ditekan (Transisi dari HIGH ke LOW)
  if (reading == LOW && lastButtonState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
    
    statusLed = !statusLed; // Toggle status
    digitalWrite(ledInternal, statusLed ? HIGH : LOW);
    
    Serial.println(statusLed ? "Lampu Nyala" : "Lampu Mati");
    drawOLED(statusLed ? "STATUS:ON" : "STATUS:OFF", "Uploading...");
    
    // Kirim ke Firebase
    pushToFirebase(statusLed ? 1 : 0);
    
    lastDebounceTime = millis();
  }

  lastButtonState = reading;
}