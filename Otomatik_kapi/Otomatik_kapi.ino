#include <Servo.h> // Servo motor kütüphanesini dahil ediyoruz

Servo bariyerMotoru; // Motorumuza bir isim veriyoruz

// Pin Tanımlamaları
int trigPin = 11;
int echoPin = 12;
int motorPin = 9;
int kirmiziLed = 8; // Kırmızı LED'i 8. pine tanımlıyoruz
int yesilLed = 7;    // Yeşil LED'i 7. pine tanımlıyoruz


// Bariyerin fiziksel duruşuna göre bu açıları değiştirebilirsin
int kapaliAci = 90; // Bariyerin inik (yatay) olduğu derece
int acikAci = 0;    // Bariyerin kalkık (dikey) olduğu derece


// Mesafe ölçümü için değişkenler
long sure;
int mesafe;

void setup() {
  // Sensör pinleri
  pinMode(trigPin, OUTPUT); // Ses dalgasını gönderen ağız
  pinMode(echoPin, INPUT);  // Yankıyı duyan kulak
  
  // LED pinleri
  pinMode(kirmiziLed, OUTPUT); // Kırmızı LED'i çıkış olarak ayarla
  pinMode(yesilLed, OUTPUT);    // Yeşil LED'i çıkış olarak ayarla
  
  // Motorumuzu 9. pine bağlıyoruz
  bariyerMotoru.attach(motorPin);
  
  // Sistem ilk açıldığında bariyer inik olsun ve kırmızı LED yansın
  bariyerMotoru.write(kapaliAci); 
  digitalWrite(kirmiziLed, HIGH); // Kırmızı LED'i yak
  digitalWrite(yesilLed, LOW);    // Yeşil LED'i söndür
  
  // Arıza tespiti için bilgisayara haberleşme başlatıyoruz
  Serial.begin(9600); 
}

void loop() {
  // --- 1. AŞAMA: MESAFEYİ ÖLÇME ---
  
  // Sensörü temizleyip yeni bir ses dalgası gönderiyoruz
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Dönen dalganın süresini hesaplıyoruz
  sure = pulseIn(echoPin, HIGH);
  
  // Süreyi santimetreye çeviren standart fizik formülü (Sesin havadaki hızı)
  mesafe = sure * 0.034 / 2; 

  // Bilgisayardan anlık mesafeyi görebilmek için yazdırıyoruz (Araçlar > Seri Port Ekranı)
  Serial.print("Mesafe: ");
  Serial.print(mesafe);
  Serial.println(" cm");

  // --- 2. AŞAMA: BEYİN VE KARAR MEKANİZMASI ---
  
  // Eğer araç 10 cm'den daha yakına gelirse bariyeri aç
  if (mesafe > 0 && mesafe < 10) { 
    digitalWrite(kirmiziLed, LOW);   // Kırmızı LED'i söndür
    digitalWrite(yesilLed, HIGH);   // Yeşil LED'i yak
    bariyerMotoru.write(acikAci);   // Bariyeri kaldır
    delay(3000);                    // Arabanın geçmesi için 3 saniye bekle
  } 
  // Araç yoksa veya uzaklaştıysa bariyeri kapalı tut
  else {
    digitalWrite(kirmiziLed, HIGH);  // Kırmızı LED'i yak
    digitalWrite(yesilLed, LOW);     // Yeşil LED'i söndür
    bariyerMotoru.write(kapaliAci);  // Bariyeri indir
  }
  
  delay(100); // Sistemin çok yorulmaması için iki ölçüm arası minik bir mola
}