#include <Wire.h>
#define address 0x40
#define DEVADDR "260BEBCF"
#define NWKKEY "C977BC29A57A4E2E042ED8DBE8A6A0ED"
#define APPSKEY "6CA38B407FE0D8FAC8ABCD3D2F281714"

char dtaUart[15];
char dtaLen = 0;
uint8_t Data[100] = { 0 };
uint8_t buff[100] = { 0 };

HardwareSerial mySerial1(1);  // Déclaration d'un objet pour la communication série matérielle

int rxPin = 20;  // Broche RX
int txPin = 21;  // Broche TX

void setup() {
  Serial.begin(115200);  // Initialisation de la communication série pour le moniteur série
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
  mySerial1.println("AT+NJM=0");  // Configuration du mode ABP
  delay(300);
  flush_serial_AT();
  mySerial1.println("AT+NWM=1");  // Configuration du mode LoRaWAN
  delay(300);
  flush_serial_AT();
  mySerial1.println("AT+BAND=4");  // Configuration de la bande de fréquences EU868
  delay(300);
  flush_serial_AT();
  mySerial1.print("AT+DEVADDR=");
  mySerial1.println(DEVADDR);  // Configuration de l'adresse du dispositif
  delay(300);
  flush_serial_AT();
  mySerial1.print("AT+NWKSKEY=");
  mySerial1.println(NWKKEY);  // Configuration de la clé de session réseau
  delay(300);
  flush_serial_AT();
  mySerial1.print("AT+APPSKEY=");
  mySerial1.println(APPSKEY);  // Configuration de la clé d'application
  delay(300);
  flush_serial_AT();

  while (mySerial1.available()) {
    Serial.write(mySerial1.read());  // Affichage des messages de configuration
  }
  delay(1000);
}

uint8_t buf[4] = { 0 };
uint16_t data, data1;
float temp;
float hum;

void loop() {
  readReg(0x00, buf, 4);
  data = buf[0] << 8 | buf[1];
  data1 = buf[2] << 8 | buf[3];
  temp = ((float)data * 165 / 65535.0) - 40.0;
  hum = ((float)data1 / 65535.0) * 100;
  Serial.print("temp(C):");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("hum(%RH):");
  Serial.println(hum);
  delay(500);
  uint16_t t = 100 * temp;
  unsigned char temp_data[4];
  uint16_t h = 100 * hum;
  temp_data[0] = t >> 8;
  temp_data[1] = t & 0xFF;
  temp_data[2] = h >> 8;
  temp_data[3] = h & 0xFF;

  char temp_str[32] = "";
  array_to_string(temp_data, 4, temp_str);
  flush_serial_AT();

  mySerial1.print("AT+SEND=3:");
  mySerial1.println(temp_str);  // Envoi de la température
  delay(10000);

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

void flush_serial_AT() {
  while (mySerial1.available()) {
    Serial.write(mySerial1.read());  // Affichage des messages de configuration
  }
  delay(100);
}