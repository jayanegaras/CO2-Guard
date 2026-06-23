// --- KONFIGURASI GLOBAL ---
const firebaseUrl = "";
const apiKey = "";

function ambilDataFirebase() {
  var sekarang = new Date();

  // 1. Hitung batas waktu mundur dalam milidetik (Epoch Time MS)
  var satuJamLalu = sekarang.getTime() - 1 * 60 * 60 * 1000;
  var satuHariLalu = sekarang.getTime() - 24 * 60 * 60 * 1000;
  var satuMingguLalu = sekarang.getTime() - 7 * 24 * 60 * 60 * 1000; // Typo 'screenshots' sudah dibersihkan

  // 2. Eksekusi penarikan data untuk masing-masing sheet secara mandiri
  prosesDanIsiSheet("jam", satuJamLalu);
  prosesDanIsiSheet("hari", satuHariLalu);
  prosesDanIsiSheet("minggu", satuMingguLalu);
}

function prosesDanIsiSheet(namaSheet, batasWaktuMundur) {
  // Perbaikan Krusial: Menggunakan encodeURIComponent agar tanda kutip ganda di "waktu_ms" tidak bikin error URL
  var queryOrderBy = encodeURIComponent('"waktu_ms"');
  var url =
    firebaseUrl +
    "?auth=" +
    apiKey +
    "&orderBy=" +
    queryOrderBy +
    "&startAt=" +
    batasWaktuMundur;

  try {
    var response = UrlFetchApp.fetch(url);
    var isiJSON = JSON.parse(response.getContentText());

    var ss = SpreadsheetApp.getActiveSpreadsheet();
    var sheet = ss.getSheetByName(namaSheet);

    if (!sheet) {
      Logger.log(
        "Waduh, Sheet dengan nama '" + namaSheet + "' gak ketemu, bro!",
      );
      return;
    }

    // Bersihkan isi sheet lama agar grafik otomatis me-refresh data terbaru
    sheet.clearContents();

    // Tulis Header Baru
    sheet.appendRow(["Waktu", "Kualitas Udara (%)", "Nilai PPM"]);

    var rows = [];

    if (isiJSON && Object.keys(isiJSON).length > 0) {
      for (var key in isiJSON) {
        var data = isiJSON[key];
        if (data.waktu && data.nilai !== undefined) {
          var ppmVal = data.ppm !== undefined ? data.ppm : 0;
          rows.push([data.waktu, data.nilai, ppmVal]);
        }
      }

      // Urutkan data berdasarkan waktu terkuno ke terbaru demi kestabilan diagram
      rows.sort(function (a, b) {
        return new Date(a[0]) - new Date(b[0]);
      });

      // Masukkan semua baris sekaligus
      if (rows.length > 0) {
        sheet.getRange(2, 1, rows.length, 3).setValues(rows);
      }
    }

    Logger.log(
      "Sukses memperbarui sheet '" +
        namaSheet +
        "' dengan " +
        rows.length +
        " data.",
    );
  } catch (err) {
    Logger.log(
      "Gagal memproses sheet " + namaSheet + ". Error: " + err.message,
    );
  }
}
