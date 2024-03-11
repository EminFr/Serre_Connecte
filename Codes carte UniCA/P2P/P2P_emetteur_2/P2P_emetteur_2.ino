#include <HardwareSerial.h>
#include <Wire.h>

// Déclaration du port série pour la communication avec le module LoRa
HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

void setup() {
  Serial.begin(115200);  // Initialisation de la communication série pour le moniteur série
  Wire.begin();
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  // Initialisation de la communication série avec le module LoRa
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);  // Pins RX, TX

  // Attendre un moment pour que le module LoRa démarre
  delay(1000);

  // Configuration du module LoRa
  configureLoRa();
}

void loop() {

  sendLoRaMessage("bienvenue");
  delay(3000);  // Attendre 3 secondes avant d'envoyer un nouveau message

}

void configureLoRa() {
  // Configuration du LoRa avec les commandes AT appropriées
  mySerial1.println("$startLoRa");  // Démarrer le mode LoRa
  delay(10000);
  mySerial1.println("AT+NWM=0)");  // Set P2P_LORA
  delay(2000);
  mySerial1.println("AT+PFREQ=868000000");  // Configuration de la fréquence (868 MHz)
  delay(1000);
  mySerial1.println("AT+PSF=7");  // Configuration du facteur d'étalement
  delay(1000);
  mySerial1.println("AT+PREAMBLELENGTH=7");  // Set Peamble Length @7
  delay(1000);
  mySerial1.println("AT+PBW=0");  // Set Freq. Bandwidth [0:9]
  delay(1000);
  mySerial1.println("AT+PTP=10");  // Set Tx Power [5:22]
  delay(1000);
  //mySerial1.println("AT+PSEND=48656C6C6F20576F726C64");  //Send Hello World
  //delay(1000);
  flush_serial_AT(true);
}

void sendLoRaMessage(String message) {
  mySerial1.println("$sendTXT");  // Commande pour envoyer un message
  String hex = stringToHex(message);
  mySerial1.print("AT+PSEND=");  // Send Txt
  mySerial1.println(hex);        // Send Txt
  Serial.print("AT+PSEND=");
  Serial.println(hex);
  flush_serial_AT(false);
}

String stringToHex(String input) {
  // Convertir une chaîne de caractères en hexadécimal
  String output = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    output += String(c, HEX);
  }
  return output;
}

void flush_serial_AT(bool print) {

  //while(mySerial1.available() == 0) {}

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