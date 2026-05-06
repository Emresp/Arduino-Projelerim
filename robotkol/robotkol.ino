#include <WiFi.h>
#include <Servo.h>

// Robotun kuracağı ağın bilgileri
char ap_ssid[] = "Robotkol_yonetim";
char ap_password[] = "12345678"; 

WiFiServer server(80);

// Servo Nesneleri
Servo sTaban, sSol, sSag, sKiskac;

// Pozisyon Kontrol Değişkenleri
int target[4]  = {90, 90, 90, 40};  // Web'den gelen hedef açılar
int current[4] = {90, 90, 90, 40};  // Motorun o anki gerçek açıları
unsigned long lastMoveTime = 0;
int moveDelay = 5; // Hız ayarı (ms). Değer büyüdükçe robot yavaşlar. 20-30 çok yavaştır.

void setup() {
  Serial.begin(115200);
  
  // Pin Tanımlamaları
  sTaban.attach(9);
  sSol.attach(10);
  sSag.attach(11);
  sKiskac.attach(12);
  
  // Başlangıç Pozisyonuna Gönder
  for(int i=0; i<4; i++) {
    if(i==0) sTaban.write(current[i]);
    if(i==1) sSol.write(current[i]);
    if(i==2) sSag.write(current[i]);
    if(i==3) sKiskac.write(current[i]);
  }

  Serial.println("Erisim Noktasi kuruluyor...");
  if (WiFi.beginAP(ap_ssid, ap_password) != WL_AP_LISTENING) {
    Serial.println("Hata: Ag kurulamadi!");
    while (true); 
  }

  delay(2000);
  Serial.print("Robot IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // 1. ADIM: Yumuşak Hareket Yönetimi (Non-blocking)
  if (millis() - lastMoveTime > moveDelay) {
    lastMoveTime = millis();
    
    for (int i = 0; i < 4; i++) {
      if (current[i] < target[i]) current[i]++;
      else if (current[i] > target[i]) current[i]--;
      
      // Fiziksel Yazma
      if(i == 0) sTaban.write(current[i]);
      else if(i == 1) sSol.write(current[i]);
      else if(i == 2) sSag.write(current[i]);
      else if(i == 3) sKiskac.write(current[i]);
    }
  }

  // 2. ADIM: Web Sunucu Yönetimi
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // URL'den gelen Hedef Değerleri Güncelle (Kıskac s4)
    if (request.indexOf("GET /?s1=") != -1) {
      int pos = request.indexOf("s1=") + 3;
      target[0] = request.substring(pos, request.indexOf("&", pos)).toInt();
    }
    if (request.indexOf("s2=") != -1) {
      int pos = request.indexOf("s2=") + 3;
      target[1] = request.substring(pos, request.indexOf("&", pos)).toInt();
    }
    if (request.indexOf("s3=") != -1) {
      int pos = request.indexOf("s3=") + 3;
      target[2] = request.substring(pos, request.indexOf("&", pos)).toInt();
    }
    if (request.indexOf("s4=") != -1) {
      int pos = request.indexOf("s4=") + 3;
      target[3] = request.substring(pos, request.indexOf(" ", pos)).toInt();
    }

    // Arayüz Çıktısı
    client.println("HTTP/1.1 200 OK\nContent-Type: text/html\n");
    client.println("<!DOCTYPE HTML><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>body{font-family:sans-serif; text-align:center; background:#eee; padding:10px;} .slider{width:85%; margin:15px;} .aci{font-weight:bold; color:#d35400;}</style></head><body>");
    client.println("<h2>Robot Kol Projesi</h2>");
    
    // Sürgülerin oluşturulması
    String motorNames[] = {"Taban", "Sol (Omuz)", "Sag (Dirsek)", "Kiskac"};
    for(int i=1; i<=4; i++) {
      int curVal = target[i-1];
      client.print("<p>" + motorNames[i-1] + ": <span class='aci' id='v" + String(i) + "'>" + String(curVal) + "</span>&deg;</p>");
      client.print("<input type='range' class='slider' onchange='send(" + String(i) + ", this)' oninput='u(" + String(i) + ", this.value)' min='0' max='180' value='" + String(curVal) + "'>");
    }

    client.println("<script>");
    client.println("function u(i, v){document.getElementById('v'+i).innerText=v;}");
    client.println("function send(id, el){");
    client.println("  var val = parseInt(el.value);");
    client.println("  if(id == 4 && (val < 40 || val > 130)){");
    client.println("    if(!confirm('Fiziksel sinir asiliyor, emin misiniz?')){");
    client.println("      val = (val < 40) ? 40 : 130; el.value = val; u(id, val);");
    client.println("    }");
    client.println("  }");
    client.println("  var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/?s'+id+'='+val+'&', true); xhttp.send();");
    client.println("}");
    client.println("</script></body></html>");
    client.stop();
  }
}