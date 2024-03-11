#include <SPI.h>
#include <LoRa.h>
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868000000)) {
    Serial.println("Erreur lors de l'initialisation LoRa.");
    while (1);
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Lecture du contenu du paquet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.println();
  }
}