# 🤖 Rex Discovery Web Kontrollü Robot Kol

Bu proje, [Robotistan Rex Discovery Serisi Pleksi Robot Kol](https://akademi.robotistan.com/rex-discovery-serisi-pleksi-robot-kol-arduino-uyumlu/) şasesi kullanılarak geliştirilmiş, çok eksenli (4-DOF) bir robotik koldur. Projenin temel amacı, servo motorların web tabanlı bir arayüz üzerinden anlık açılarla (slider) senkronize bir şekilde kontrol edilmesini sağlamaktır.

## 📸 Proje Görünümü

Aşağıdaki görselde projenin tamamlanmış fiziksel kurulumunu ve arka planda motor açılarını kontrol eden web arayüzünü görebilirsiniz:

![Robot Kol Tam Hali](./tamhali.JPG)

## 🛠️ Kullanılan Donanımlar

* **Mekanik İskelet:** Rex Discovery Serisi Pleksi Robot Kol
* **Kontrolcü Kart:** Arduino / ESP Serisi Geliştirme Kartı
* **Eyleyiciler:** 4 Adet Servo Motor (Taban, Omuz, Dirsek ve Tutucu/Gripper için)
* **Güç Kaynağı:** Harici Pil Kutusu (Servo motorların anlık akım çekimlerinde kartı resetlememesi ve stabil çalışması için harici besleme kullanılmıştır.)
* **Diğer:** Breadboard ve Jumper Kablolar

## ⚙️ Çalışma Mantığı ve Özellikler

1. **Web Tabanlı Kontrol:** Bilgisayar ekranında görülen arayüz üzerinden her bir eksen için (Taban, Omuz, Dirsek, Kıskaç) birer kaydırıcı (slider) bulunur.
2. **Anlık İletişim:** Arayüzdeki slider'lar hareket ettirildiğinde, belirlenen açı değeri (`0° - 180°`) anlık olarak mikrokontrolcüye iletilir.
3. **PWM Sinyali:** Gelen açı verileri, mikrokontrolcü tarafından PWM sinyallerine dönüştürülerek ilgili servo motora aktarılır ve fiziksel hareket sağlanır.
4. **Harici Besleme:** Servo motorların kararlı çalışabilmesi için motorların `VCC` ve `GND` hatları Arduino'dan değil, doğrudan harici güç kaynağından beslenmektedir (Sadece ortak GND bağlantısı yapılmıştır).

## 🚀 Kurulum Adımları

1. Robot kolun pleksi montajını gerçekleştirin ve servo motorları yerlerine sabitleyin.
2. `robotkol.ino` dosyasını Arduino IDE üzerinden açın.
3. Kontrolcü kartınıza uygun port ve kart seçimini yapıp kodu yükleyin.
4. Motorların bağlantılarını yaparken harici güç kaynağı kullanmayı ve **GND hatlarını birleştirmeyi** unutmayın.
5. Kodu çalıştırdıktan sonra web arayüzüne bağlanarak slider'lar ile kolu test edin.

---
[🏠 Ana Sayfaya Dön](../README.md)
