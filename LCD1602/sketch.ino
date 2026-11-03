#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ganti dengan WiFi Anda jika dipakai di ESP32 asli.
// Untuk simulasi Wokwi, biarkan seperti ini.
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// Zona waktu: WIB = 7, WITA = 8, WIT = 9
const long GMT_OFFSET_SEC = 7 * 3600;
const int  DST_OFFSET_SEC = 0;
const char* NTP_SERVER = "pool.ntp.org";

LiquidCrystal_I2C lcd(0x27, 16, 2);   // alamat I2C: 0x27 (atau 0x3F)

const char* HARI[] = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);                 // SDA = GPIO21, SCL = GPIO22
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Menghubungkan...");
  WiFi.begin(WIFI_SSID, WIFI_PASS, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sinkron waktu...");
  configTime(GMT_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER);

  struct tm t;
  while (!getLocalTime(&t)) {
    delay(300);
  }
  lcd.clear();
}

void loop() {
  struct tm t;
  if (getLocalTime(&t)) {
    char baris1[17];
    char baris2[17];

    // Baris 1: hari dan tanggal
    snprintf(baris1, sizeof(baris1), "%s %02d/%02d/%04d",
             HARI[t.tm_wday], t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);

    // Baris 2: jam dengan titik dua berkedip
    char sep = (t.tm_sec % 2) ? ' ' : ':';
    snprintf(baris2, sizeof(baris2), "   %02d%c%02d%c%02d   ",
             t.tm_hour, sep, t.tm_min, sep, t.tm_sec);

    lcd.setCursor(0, 0);
    lcd.print(baris1);
    lcd.setCursor(0, 1);
    lcd.print(baris2);
  }
  delay(500);
}
