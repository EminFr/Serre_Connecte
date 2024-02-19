#include <Wire.h>

#define address 0x40

HardwareSerial mySerial1(1);

int rxPin = 20;
int txPin = 21;

String devAddr = "260BEBCF";
String nwkkey = "C977BC29A57A4E2E042ED8DBE8A6A0ED";
String appskey = "6CA38B407FE0D8FAC8ABCD3D2F281714";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ABP test");
  
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(10, OUTPUT); // Rak enable
  pinMode(4, OUTPUT); // LED

  digitalWrite(4, HIGH);   // turn the LED on
  delay(1000);                       
  digitalWrite(4, LOW);    // turn the LED off
  delay(1000);  

  digitalWrite(10, HIGH); // Switch on RAK
  delay(1000);
  mySerial1.begin(115200, SERIAL_8N1, rxPin, txPin);
  delay(1000);  

  Serial.println("Setup at command");
  mySerial1.println("AT+NJM=0"); // Set ABP
  delay(300);
  flush_serial_AT();
  mySerial1.println("AT+NWM=1"); // Set LoRaWan mode
  delay(300); 
  flush_serial_AT();
  mySerial1.println("AT+BAND=4");// Set EU868 frequency band
  delay(300);
  flush_serial_AT();

  mySerial1.print("AT+DEVADDR=");
  mySerial1.println(devAddr);
  delay(300);
  flush_serial_AT();
  mySerial1.print("AT+NWKSKEY=");
  mySerial1.println(nwkkey);
  delay(300);
  flush_serial_AT();  
  mySerial1.print("AT+APPSKEY=");
  mySerial1.println(appskey);
  delay(300);
  flush_serial_AT();

  while (mySerial1.available()) {
    Serial.write(mySerial1.read());
  }
  delay(1000);
}

void loop() {
  uint16_t t = 100 * measure_temp(); 
  unsigned char mydata[4];
  mydata[0] = 0x1; // CH1
  mydata[1] = 0x67; // Temp
  mydata[2] = t >> 8;
  mydata[3] = t & 0xFF;

  char str[32] = "";
  array_to_string(mydata, 4, str);
  flush_serial_AT();

  mySerial1.print("AT+SEND=3:");
  mySerial1.println(str);
  delay(5300);
  
  while (mySerial1.available()) { 
    Serial.write(mySerial1.read());
  }
  Serial.println("AT set complete with downlink");
  delay(3000);
  Serial.println("Uplink");
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
  }
  buffer[len * 2] = '\0';
}

float measure_temp() {
  flush_serial_AT(); 
  
  mySerial1.println("ATC+TEMP=?"); 
  String temperature;
  delay(300);

  if (mySerial1.available()) {
    temperature = mySerial1.readStringUntil('\n');
    Serial.print("Temperature:");
    Serial.println(temperature);
  }
  
  return temperature.toFloat();
}

void flush_serial_AT() {
  while (mySerial1.available()) {
    Serial.write(mySerial1.read());
  }
  delay(100);
}