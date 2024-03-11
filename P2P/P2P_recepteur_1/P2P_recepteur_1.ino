#include <HardwareSerial.h>

// Déclaration du port série pour la communication avec le module LoRa
HardwareSerial LoRaSerial(1);

void setup() {
  Serial.begin(9600);
  LoRaSerial.begin(9600, SERIAL_8N1, 20, 21);
  delay(1000);

  // Configuration LoRa avec commandes AT
  configureLoRa();

  // Configuration de la carte récepteur
  configureReceiver();

  Serial.println("Reception LoRa réussi !");
}

void loop() {
  // Attendre et afficher les données reçues
  if (LoRaSerial.available()) {
    Serial.write(LoRaSerial.read());
    //Serial.println("Reception LoRa réussi 2 !");
  }
}

void configureLoRa() {
  // Configuration LoRa avec commandes AT
  LoRaSerial.println("$startLoRa");  // Démarrer le mode LoRa
  delay(1000);
  LoRaSerial.println("AT+PFREQ=866000000");
  delay(1000);
  LoRaSerial.println("AT+PBW=0");
  delay(1000);
  LoRaSerial.println("AT+PSF=12");
  delay(1000);
}

void configureReceiver() {
  // Configuration de la carte récepteur
  LoRaSerial.println("$startRx");
}