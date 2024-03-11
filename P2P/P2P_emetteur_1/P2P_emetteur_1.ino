#include <Arduino.h>
// Déclaration du port série pour la communication avec le module LoRa
HardwareSerial LoRaSerial(1);

void setup() {
  Serial.begin(9600); // Initialisation du port série pour le moniteur série
  LoRaSerial.begin(9600, SERIAL_8N1, 20, 21); // Initialisation du port série pour la communication avec le module LoRa
  
  // Attendre un moment pour que le module LoRa démarre
  delay(1000);
  
  // Configuration du module LoRa
  configureLoRa();
}

void loop() {
  // Envoi du message "bienvenue_le_sang" en LoRa
  sendLoRaMessage("bienvenue_le_sang");
  delay(5000); // Attendre 5 secondes avant d'envoyer un nouveau message

  // Affichage d'un mot dans le moniteur série pour prouver que la communication fonctionne
  //Serial.println("Emission LoRa réussi !");
}

void configureLoRa() {
  // Configuration du LoRa avec les commandes AT appropriées
  LoRaSerial.println("$startLoRa");
  delay(1000);
  LoRaSerial.println("AT+PFREQ=868000000"); // Configuration de la fréquence (868 MHz)
  delay(1000);
  LoRaSerial.println("AT+PBW=0"); // Configuration de la bande passante
  delay(1000);
  LoRaSerial.println("AT+PSF=12"); // Configuration du facteur d'étalement
  delay(1000);
}

void sendLoRaMessage(String message) {
  // Envoi d'un message en LoRa
  LoRaSerial.print("$sendTXT"); // Commande pour envoyer un message
  LoRaSerial.println(stringToHex(message)); // Conversion du message en hexadécimal
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