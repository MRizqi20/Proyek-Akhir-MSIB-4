  #define BLYNK_TEMPLATE_ID "TMPL6u2K-g_oP"
  #define BLYNK_TEMPLATE_NAME "Smart Monitoring"
  #define BLYNK_AUTH_TOKEN "DoiQ3RBVE7Nws0BCnbDJOVs3h54Z33fL"

  #define BLYNK_PRINT Serial
  #include <DHT.h>
  DHT dht(27,DHT11);
  const int LED_PIN2 = 4; // Pin yang digunakan untuk menghubungkan LED
  const int LDR_PIN = 35; // Pin yang digunakan untuk menghubungkan sensor LDR
  const int LED_PIN1 = 2; // Pin yang digunakan untuk menghubungkan LED
  const int PIR_PIN = 17; // Pin yang digunakan untuk menghubungkan sensor PIR
  const int PUMP_PIN = 25; // Pin yang digunakan untuk menghubungkan pompa
  const int SOIL_PIN = 34; // Pin yang digunakan untuk menghubungkan sensor kelembapan tanah
  const int apiPin = 5;
  const int buzzer_pin = 26;
  const int kipas = 18;

  const int moistureThreshold = 25;

  // Variabel global
  int ldrValue = 0;
  int ledState = LOW;

  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp32.h>

  char auth[] = BLYNK_AUTH_TOKEN ;
  char ssid[] = "iPhone";
  char pass[] = "11223344";

  void setup() {
    Serial.begin(9600); // Memulai komunikasi serial pada baud rate 9600
    pinMode(LED_PIN1, OUTPUT); // Konfigurasi pin sebagai output
    pinMode(LDR_PIN, INPUT); // Konfigurasi pin sebagai input
    pinMode(LED_PIN2, OUTPUT); // Konfigurasi pin sebagai output
    pinMode(PIR_PIN, INPUT); // Konfigurasi pin sebagai input
    pinMode(PUMP_PIN, OUTPUT); // Konfigurasi pin sebagai output
    pinMode(SOIL_PIN, INPUT); // Konfigurasi pin sebagai input
    pinMode (buzzer_pin,OUTPUT);
    pinMode (kipas,OUTPUT);
    dht.begin();
    delay(200);
    // Menghubungkan ESP32 ke WiFi
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    // Menghubungkan ESP32 ke Blynk
    Blynk.begin(auth, ssid, pass);   // Mulai koneksi Blynk
  }

  void loop() {
    int LED_value1 = digitalRead(LED_PIN1);  // Baca nilai sensor LED1
    int led_value2 = digitalRead(LED_PIN2);  // Baca nilai sensor LED1
    int PUMP_value = digitalRead (PUMP_PIN); 
  
    int pir_value = digitalRead(PIR_PIN);
    if (pir_value == HIGH) { // Jika sensor PIR mendeteksi gerakan
      digitalWrite(LED_PIN1, HIGH); // Menghidupkan LED  
      Serial.println("PIR detected motion!"); // Menampilkan pesan pada Serial Monitor
      Blynk.virtualWrite(V0, "PIRDetected");
      Blynk.virtualWrite(V3, "Hidup");
    }
    else {
      digitalWrite(LED_PIN1, LOW); // Mematikan LED
      Serial.println("PIR did not detect motion."); // Menampilkan pesan pada Serial Monitor
      Blynk.virtualWrite(V0, "PIRNoDetected");
      Blynk.virtualWrite(V3, "Mati");
    }
    int ldrValue = analogRead(LDR_PIN); // Membaca nilai analog dari sensor LDR
    Blynk.virtualWrite(V1, ldrValue);
    Serial.print("LDR sensor value: "); // Menampilkan pesan pada Serial Monitor
    Serial.println(ldrValue); // Menampilkan nilai sensor pada Serial Monitor
    
    if (ldrValue < 500) { // Jika nilai sensor LDR kurang dari 500
      digitalWrite(LED_PIN2, HIGH); // Menghidupkan LED
      Serial.println("LED ON!"); // Menampilkan pesan pada Serial Monitor
      Blynk.virtualWrite(V4, "Hidup");
 
    } else {
      digitalWrite(LED_PIN2, LOW); // Mematikan LED
      Serial.println("LED OFF!"); // Menampilkan pesan pada Serial Monitor
      Blynk.virtualWrite(V4, "Mati");
    }
    int soilValue = analogRead(SOIL_PIN); // Membaca nilai analog dari sensor kelembapan tanah
    int moisture = map(soilValue, 4095, 0 , 0, 100);
    Blynk.virtualWrite(V2, moisture);
    Serial.print("Soil moisture level: "); // Menampilkan pesan pada Serial Monitor
    Serial.print(moisture); // Menampilkan nilai sensor pada Serial Monitor
    Serial.println("%");
    if (moisture < moistureThreshold) { // Jika nilai sensor kelembapan tanah kurang dari 500
      digitalWrite(PUMP_PIN, HIGH); // Menghidupkan pompa
      Serial.println("Pump ON!"); // Menampilkan pesan pada Serial Monitor
      Blynk.virtualWrite (V5, "Hidup");
    } else {
      digitalWrite(PUMP_PIN, LOW); // Mematikan pompa
      Serial.println("Pump OFF!"); // Menampilkan pesan pada Serial Monitor
      Blynk.virtualWrite (V5, "Mati");
    }
    int apiValue = digitalRead(apiPin);
  if (apiValue == LOW){
      Serial.print (apiValue);
      Serial.println(" - Tidak terdeteksi Api");
      Blynk.virtualWrite (V7, "Aman");
      Blynk.virtualWrite(V10, "Mati");
      digitalWrite(buzzer_pin, LOW);
      digitalWrite(kipas, LOW); 
      }
      else
    {
      Serial.print (apiValue);
      Serial.println(" - Api terdeteksi ");
    digitalWrite(buzzer_pin, HIGH); 
    digitalWrite(kipas, HIGH);   
      Blynk.virtualWrite(V7, "Kebakaran");
      Blynk.virtualWrite(V10, "Hidup"); 
    }
    float temp =dht.readTemperature();
    float humidity = dht.readHumidity();
    Blynk.virtualWrite(V8, temp);
    Blynk.virtualWrite(V9, humidity);
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" % ");
    delay(1000); // Menunggu selama 1 detik
  }