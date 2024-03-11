#include <Wire.h>
#define address 0x40
#define DEVADDR "260BEBCF"
#define NWKKEY "C977BC29A57A4E2E042ED8DBE8A6A0ED"
#define APPSKEY "6CA38B407FE0D8FAC8ABCD3D2F281714"
#define solPin 0  // port de connexion du capteur

char dtaUart[15];
char dtaLen = 0;
uint8_t Data[100] = { 0 };
uint8_t buff[100] = { 0 };

HardwareSerial mySerial1(1);  // Déclaration d'un objet pour la communication série matérielle

int rxPin = 20;  // Broche RX
int txPin = 21;  // Broche TX

void setup() {
  Serial.begin(115200);  // Initialisation de la communication série pour le moniteur série
  pinMode(solPin, INPUT);
  delay(1000);
  Serial.println("ABP test");
  Wire.begin();
  pinMode(txPin, OUTPUT);  // Configuration de la broche de transmission comme sortie
  pinMode(rxPin, INPUT);   // Configuration de la broche de réception comme entrée
  pinMode(10, OUTPUT);     // Activation du module Rak
  pinMode(4, OUTPUT);      // Activation de la LED
  digitalWrite(10, HIGH);  // Activer le module RAK
  delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);  // Initialisation de la communication série matérielle
  delay(1000);
  Serial.println("Setup at command");

  // Configuration du mode ABP
  sendCommand("AT+NJM=0");
  // Configuration du mode LoRaWAN
  sendCommand("AT+NWM=1");
  // Configuration de la bande de fréquences EU868
  sendCommand("AT+BAND=EU868");
  // Configuration de l'adresse du dispositif
  sendCommand("AT+DEVADDR=" DEVADDR);
  // Configuration de la clé de session réseau
  sendCommand("AT+NWKSKEY=" NWKKEY);
  // Configuration de la clé d'application
  sendCommand("AT+APPSKEY=" APPSKEY);

  delay(1000);

  // Configuration de la fréquence LoRa
  sendCommand("AT+RFREQ=868000000");
}

uint8_t buf[4] = { 0 };
uint16_t data, data1;
float temp;
float hum;
int solValue;  // Capteur humidité sol

void loop() {
  readReg(0x00, buf, 4);
  data = buf[0] << 8 | buf[1];
  data1 = buf[2] << 8 | buf[3];
  temp = ((float)data * 165 / 65535.0) - 40.0;
  hum = ((float)data1 / 65535.0) * 100;
  solValue = analogRead(solPin);  // Valeur du capteur analogique

  Serial.print("sol = ");
  Serial.println(solValue);  // Afficher la variable du capteur analogique
  Serial.print("temp(C):");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("hum(%):");
  Serial.println(hum);
  delay(500);

  // Encode data for LoRa transmission
  unsigned char temp_data[6];
  uint16_t t = 100 * temp;
  uint16_t h = 100 * hum;
  uint16_t hs = solValue;

  temp_data[0] = t >> 8;
  temp_data[1] = t & 0xFF;
  temp_data[2] = h >> 8;
  temp_data[3] = h & 0xFF;
  temp_data[4] = hs >> 8;
  temp_data[5] = hs & 0xFF;

  char temp_str[32] = "";
  array_to_string(temp_data, 6, temp_str);

  mySerial1.print("AT+SEND=3:");
  mySerial1.println(temp_str);                  // Envoi de la température
  Serial.println("Données envoyées en LoRa.");  // Message de débogage
  delay(10000);
}

void sendCommand(String cmd) {
  mySerial1.println(cmd);
  delay(100);
  while (mySerial1.available()) {
    Serial.write(mySerial1.read());
  }
  delay(100);
}

uint8_t readReg(uint8_t reg, const void* pBuf, size_t size) {
  if (pBuf == NULL) {
    Serial.println("pBuf ERROR!! : null pointer");  // Gestion de l'erreur de pointeur nul
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  Wire.beginTransmission(address);
  Wire.write(&reg, 1);
  if (Wire.endTransmission() != 0) {
    return 0;
  }
  delay(20);
  Wire.requestFrom(address, (uint8_t)size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  return size;
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;  // Conversion en caractères hexadécimaux
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;  // Conversion en caractères hexadécimaux
  }
  buffer[len * 2] = '\0';  // Termination de la chaîne de caractères
}