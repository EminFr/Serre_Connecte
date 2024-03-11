#include <HardwareSerial.h>
#include <Wire.h>
#include "DHT.h"

#define solPin 0   // Port de connexion du capteur d'humidité du sol
#define DHTPIN 10  // Capteur hum et temp

// initialisation du capteur
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Déclaration du port série pour la communication avec le module LoRa
HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(solPin, INPUT);  //Capteur himidité
  dht.begin();             // Debut de la mesure
  // Initialisation de la communication série avec le module LoRa
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);  // Pins RX, TX
  // Attendre un moment pour que le module LoRa démarre
  delay(1000);
  // Configuration du module LoRa
  configureLoRa();
  delay(2000);
}
void loop() {
  int solValue = analogRead(solPin);  // Valeur du capteur humidité du sol
  float h = dht.readHumidity();       // mesure de l'humidité
  float t = dht.readTemperature();    // mesure de la température

  Serial.println(solValue);
  Serial.println(h);
  Serial.println(t);
  
  sendLoRaMessage(solValue);  // Envoi de la valeur de l'humidité SOL
  delay(1000);
  sendLoRaMessage(h);  // Envoi de la valeur de l'humidité AIR
  delay(1000);
  sendLoRaMessage(t);  // Envoi de la valeur de la temperature AIR
  delay(4000);
}

void configureLoRa() {
  // Configuration du LoRa avec les commandes AT appropriées
  mySerial1.println("$startLoRa");  // Démarrer le mode LoRa
  flush_serial_AT(true);
  delay(10000);
  mySerial1.println("AT+NWM=0"); // Set P2P_LORA
  Serial.println("AT+NWM=0 // Set P2P_LORA"); // Set P2P_LORA
  flush_serial_AT(true);
  delay(1000);
  mySerial1.println("AT+PFREQ=868000000");// Set frequency @868MHz 
  Serial.println("AT+PFREQ=868000000 // Set frequency @868MHz ");// Set frequency 
  flush_serial_AT(true);
  delay(200);
  mySerial1.println("AT+PSF=7");// Set Spreading Factor @12 
  Serial.println("AT+PSF=7 // Set Spreading Factor[5;12] ");// Set SF 
  flush_serial_AT(true);
  delay(200);
  mySerial1.println("AT+PREAMBLELENGTH=7");// Set Peamble Length @7 
  Serial.println("AT+PREAMBLELENGTH=7 // Set Peamble Length");// Set SF 
  flush_serial_AT(true);
  delay(200);
  mySerial1.println("AT+PBW=0");// Set Freq. Bandwidth [0:9] 
  Serial.println("AT+PBW=0 // Set Freq. Bandwidth 0 = 125");// Set BW
 flush_serial_AT(true);
  delay(200);
  mySerial1.println("AT+PTP=10"); // Set Tx Power [5:22] 
  Serial.println("AT+PTP=10 // Set Tx Power [5:22]  "); // Set Tx Pow
  flush_serial_AT(true);
  delay(6000);
}

void sendLoRaMessage(int value) {
  mySerial1.println("$sendTXT");  // Commande pour envoyer un message
  flush_serial_AT(true);
  mySerial1.print("AT+PSEND=");
  flush_serial_AT(true);
  mySerial1.println(String(value));
  flush_serial_AT(true);
  Serial.print("AT+PSEND=");
  Serial.println(value);
  flush_serial_AT(true);
  delay(1000);
}

String stringToHex(String input) {
  // Convertir une chaîne de caractères en hexadéimal
  String output = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    output += String(c, HEX);
  }
  return output;
}

void flush_serial_AT(bool print) {
  if (mySerial1.available()) {  // If anything comes in Serial1 (pins 4 & 5)
    while (mySerial1.available())
      if (print) {
        Serial.write(mySerial1.read());  // read it and send it out Serial (USB)
      } else {
        mySerial1.read();
      }
  }
  delay(100);
}