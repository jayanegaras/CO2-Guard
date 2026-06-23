# 🌌 CO2-Guard Project Ecosystem

[![Project Status](https://img.shields.io/badge/Status-Development-green.svg)](#)
[![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)](#)
[![Database](https://img.shields.io/badge/Database-Firebase-yellow.svg)](#)

Selamat datang di repository **CO2-Guard**. Project ini adalah sistem ekosistem IoT yang mengintegrasikan perangkat keras (ESP32), database real-time (Firebase), dan pengolahan data otomatis menggunakan Google Sheets.

---

## 📑 Daftar Isi

1.  **[📍 Prototype (ESP32 & Firebase)](./prototype)**
    - Berisi kode sumber (`.ino`), file rahasia, dan panduan teknis alat.
    - Saat ini hanya berisi perihal menghubungkan dengan firebase dan spreadsheet
2.  **[📊 Data Visualization](#-visualisasi-data)**
    - Penjelasan alur data dari Firebase ke Google Sheets.
3.  **[🛠️ Hardware Requirements](#️-kebutuhan-perangkat-keras)**
    - Daftar komponen yang dibutuhkan.
4.  **[🚀 Quick Start](#-memulai)**
    - Cara menjalankan project ini dari nol.

---

## 📂 Struktur Repository

```text
CO2-Guard/
├── prototype/              # Folder utama sistem IoT
│   ├── prototype.ino       # Kode program ESP32
│   ├── arduino_secrets.h   # Konfigurasi WiFi & API (Private)
│   ├── README.md           # Dokumentasi detail alat & skema
│   └── img/                # Media foto dan video demo
├── research/               # Dokumen pendukung atau referensi
└── LICENSE                 # Lisensi penggunaan project# CO2-Guard
This repo containing material that we used to make our IoT Project
```

## 🏛️ Arsitektur Sistem

Sistem ini menggunakan metode **REST API** yang ringan untuk menghubungkan berbagai platform:

- **ESP32:** Membaca input tombol dan mengirim data HTTPS POST.
- **Firebase Realtime Database:** Sebagai pusat data dan sinkronisasi status.
- **Google Sheets:** Menarik data otomatis menggunakan Apps Script untuk keperluan diagram riwayat.

---

## 🛠️ Kebutuhan Perangkat Keras

| Nama Komponen        | Deskripsi                                       |
| :------------------- | :---------------------------------------------- |
| **ESP32 DevKit V1**  | Mikrokontroler utama dengan WiFi built-in.      |
| **SH1106 OLED 1.3"** | Menampilkan status koneksi dan respon HTTP.     |
| **Push Button**      | Sebagai pemicu (trigger) perubahan status data. |
| **Jumper Wires**     | Penghubung antar komponen.                      |

---

## 🚀 Memulai

Untuk mulai menggunakan project ini, ikuti langkah berikut:

1.  Buka folder **[prototype](./prototype)** untuk melihat instruksi pengkabelan dan library yang dibutuhkan.
2.  Siapkan **Firebase Realtime Database** dan atur _Rules_ menjadi `true` untuk akses baca/tulis selama fase testing.
3.  Gunakan **Google Apps Script** untuk menghubungkan database ke spreadsheet kamu.
4.  Upload kode ke ESP32 dan pantau melalui serial monitor atau layar OLED.

---

## 🤝 Kontribusi

Kami sangat terbuka untuk pengembangan lebih lanjut, terutama pada:

- Integrasi sensor CO2 (MH-Z19B atau MQ-135).
- Optimasi penggunaan RAM pada ESP32.
- Pembuatan dashboard berbasis web yang lebih interaktif.

---

**Maintained by Kelompok Orang Ganteng**  
_Update terakhir: Juni 2026_
