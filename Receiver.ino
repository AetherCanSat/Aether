#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;
const int resetPin = 9;
const int irqPin = 2;

unsigned long lastReceiveTime = 0; // Zmienna do przechowywania ostatniego czasu odbioru
unsigned long checkInterval = 10000; // Interwał czasowy do sprawdzenia w ms (10 sekund)

void setup() {
  Serial.begin(9600);
  
  // Initialize LoRa module
  LoRa.setPins(csPin, resetPin, irqPin);
  
  if (!LoRa.begin(433E6)) { // 433 MHz frequency
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");

  lastReceiveTime = millis(); // Zainicjuj czas pierwszego odbioru
}

void loop() {
  // Check if there's a LoRa packet available
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Read packet
    String incoming = "";
    
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    // Print received message
    Serial.print("Received: ");
    Serial.println(incoming);

    // Zaktualizuj czas ostatniego odbioru
    lastReceiveTime = millis();
  }

  // Sprawdź, czy sygnał nie przestał przychodzić przez 10 sekund
  if (millis() - lastReceiveTime > checkInterval) {
    Serial.println("No signal received for a while. Restarting LoRa...");

    // Zrestartuj moduł LoRa, jeśli sygnał nie przychodzi
    LoRa.end();
    if (!LoRa.begin(433E6)) { // 433 MHz frequency
      Serial.println("LoRa restart failed!");
      while (1);
    } else {
      Serial.println("LoRa restarted successfully!");
    }

    lastReceiveTime = millis(); // Zresetuj czas po restarcie
  }
}
