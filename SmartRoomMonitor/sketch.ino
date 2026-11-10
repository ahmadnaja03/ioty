#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definisi Pin dan Jenis Sensor
#define DHTPIN 4          // Pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22     // Menggunakan sensor DHT22
DHT dht(DHTPIN, DHTTYPE);

// Inisialisasi LCD I2C (Alamat 0x27 atau 0x3F, ukuran 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzerPin = 18; // Pin buzzer di GPIO 18
const int ledPin = 5;     // Pin LED di GPIO 5

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi komponen
  dht.begin();
  lcd.init();                      
  lcd.backlight();
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("IoT Smart Room");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Membaca kelembapan dan suhu
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // dalam Celsius

  // Cek jika pembacaan gagal
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    return;
  }

  // Menampilkan data ke Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" C  | Kelembapan: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Menampilkan data ke LCD I2C
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);  //1 angka desimal
  lcd.print(" C");            // spasi untuk menimpa sisa karakter

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %");

  // Logika Otomatisasi: Jika suhu > 33°C, buzzer dan LED menyala (peringatan)
  if (temperature > 33.0) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    Serial.println("PERINGATAN: Suhu Tinggi!");
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(2000); // Jeda 2 detik sebelum pembacaan berikutnya
}
