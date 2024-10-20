#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;
const int resetPin = 9;
const int irqPin = 2;

const int lm35Pin = A0;  // LM35 connected to analog pin A0
const int numSamples = 10;  // Liczba próbek do uśrednienia

void setup() {
  Serial.begin(9600);
  
  // Initialize LoRa module
  LoRa.setPins(csPin, resetPin, irqPin);
  
  if (!LoRa.begin(433E6)) { // 433 MHz frequency
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // Uśrednianie próbek
  float total = 0.0;
  for (int i = 0; i < numSamples; i++) {
    int analogValue = analogRead(lm35Pin);
    float voltage = analogValue * (5.0 / 1023.0);  // Konwersja wartości analogowej na napięcie
    float temperature = voltage * 100;  // LM35 daje 10mV na °C
    total += temperature;
    delay(100);  // Krótka przerwa między odczytami
  }

  float averageTemperature = total / numSamples;

  // Wyświetlanie średniej temperatury na monitorze szeregowym
  Serial.print("Średnia temperatura: ");
  Serial.print(averageTemperature);
  Serial.println(" °C");

  // Wysłanie średniej temperatury przez LoRa
  LoRa.beginPacket();
  LoRa.print(averageTemperature);
  LoRa.endPacket();

  // Opóźnienie przed kolejnym cyklem
  delay(1000);
}