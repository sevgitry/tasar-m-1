#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

int buttonPin1 = 2; 
int buttonPin2 = 3;
const int potPin = A0;
const int ledR = 1;       // Kırmızı LED pin
const int ledG = 10;       // Yeşil LED pin
const int ledB = 11;       // Mavi LED pin

// Diğer değişkenler
int potValue;             // Potansiyometre değeri
int pompaHizi=50;            // Pompa hızı
int suMiktari;          // Toplam içilen su miktarı (ml)
int kiloKisi1=70;
int kiloKisi2=60;
float suMiktari1=0; // Kişi 1'in kilosuna bağlı su miktarını hesapla (kg x 35 ml)
float suMiktari2=0.00; // Kişi 2'nin kilosuna bağlı su miktarını hesapla (kg x 35 ml)       // B kişisinin içilen su miktarı (ml)
float suTMiktari1=kiloKisi1 * 0.035;; // Kişi 1'in kilosuna bağlı TOPLAM su miktarını hesapla (kg x 35 ml)
float suTMiktari2=kiloKisi2 * 0.035;;
int sure=0;
unsigned long previousMillis = 0;  // Önceki zamanı saklamak için değişken
unsigned long currentMillis = 0;
unsigned long interval = 1000;     // Her bir saniye (1000 milisaniye) için interval değeri               // Pompa hızı (ml/s)

void setup() {
  Serial.begin(9600); // Seri monitörü başlat
  // Pin modları
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  
  // LCD başlatma
  lcd.begin(16, 2);
  
  // RGB LED pinleri
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  // Başlangıçta yeşil LED yanacak
  digitalWrite(ledG, HIGH);

  // Başlangıç ekranını göster
  displayInitialScreen();
}

void loop() {
  
  // Potansiyometre değerini oku
  potValue = analogRead(potPin);
  pompaHizi = map(potValue, 0, 1023, 50, 200); // Pompa hızını potansiyometre değerine göre ayarla
  
  // Butonları kontrol et ve içen kişiyi belirle
  while(digitalRead(buttonPin1)==HIGH){
   currentMillis = millis();  // Geçerli zamanı al
    
    // Belirli bir süre geçti mi kontrol et
    if (currentMillis - previousMillis >= interval) {
      // Bir saniye geçtiği için içilen su miktarını hesapla
      previousMillis = currentMillis;  // Şu anki zamanı önceki zaman olarak kaydet
      sure++;
      suMiktari1 = suMiktari1+(pompaHizi/1000.00);            // İçilen su miktarını artır
      Serial.print("Icilen su miktari: ");
      Serial.print(suMiktari1);
      Serial.println(" (L)");       
    }
  } 
  
 while(digitalRead(buttonPin2)==HIGH){
   currentMillis = millis();  // Geçerli zamanı al
    
    // Belirli bir süre geçti mi kontrol et
    if (currentMillis - previousMillis >= interval) {
      // Bir saniye geçtiği için içilen su miktarını hesapla
      previousMillis = currentMillis;  // Şu anki zamanı önceki zaman olarak kaydet
      sure++;
      suMiktari2 = suMiktari2+(pompaHizi/1000.00);            // İçilen su miktarını artır
      Serial.print("Icilen su miktari: ");
      Serial.print(suMiktari2);
      Serial.println(" (L)");  
  }    
 }
  
  // LCD ekranına bilgileri yazdır
  displayInformation();
  
  // Su miktarına göre RGB LED rengini güncelle
  updateLed();

  // Belirli bir süre sonra LCD'yi temizle
  sure=0;
  delay(5000);
}

// RGB LED rengini güncelleme fonksiyonu
void updateLed() {
  // Hedefe ulaşıldı mı kontrol et ve LED rengini güncelle
  if (suMiktari1 >= suTMiktari1) {
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
  } 
 else {
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
  } 
 // Hedefe ulaşıldı mı kontrol et ve LED rengini güncelle
  if (suMiktari2 >= suTMiktari2) {
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
  }   
  else {
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
  }
}
    
// Başlangıç ekranını gösterme fonksiyonu
void displayInitialScreen() {
  lcd.setCursor(0, 0);
  lcd.print("A hedef:");
  lcd.print(suTMiktari1); // Kişi 1'in günlük su hedefini ekrana yazdır
  lcd.print("L");
  lcd.setCursor(0, 1);
  lcd.print("B hedef:");
  lcd.print(suTMiktari2); // Kişi 2'in günlük su hedefini ekrana yazdır
  lcd.print("L");
  delay(3000);
}

// Bilgi ekranını gösterme fonksiyonu
 void displayInformation() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Akisisi:" );
  lcd.print(suMiktari1);
  lcd.print("L");

  lcd.setCursor(0, 1);
  lcd.print("BKisisi:");
  lcd.print(suMiktari2);
  lcd.print("L");
}