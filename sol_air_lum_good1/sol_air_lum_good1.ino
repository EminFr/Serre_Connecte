void loop() {
  readReg(0x00, buf, 4);
  data = buf[0] << 8 | buf[1];
  data1 = buf[2] << 8 | buf[3];
  temp = ((float)data * 165 / 65535.0) - 40.0;
  hum = ((float)data1 / 65535.0) * 100;
  solValue = analogRead(solPin);  // Valeur du capteur analogique
  int lumValue = getLightData();  // Récupérer la luminosité
  
  Serial.print("sol = ");
  Serial.println(solValue);
  Serial.print("temp(C):");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("hum(%):");
  Serial.println(hum);
  Serial.print("lum:");
  Serial.println(lumValue);
  delay(500);

  unsigned char temp_data[8]; 
  uint16_t t = 100 * temp;
  uint16_t h = 100 * hum;
  uint16_t hs = solValue;
  uint16_t lum = lumValue;

  temp_data[0] = t >> 8;
  temp_data[1] = t & 0xFF;
  temp_data[2] = h >> 8;
  temp_data[3] = h & 0xFF;
  temp_data[4] = hs >> 8;
  temp_data[5] = hs & 0xFF;
  temp_data[6] = lum >> 8;
  temp_data[7] = lum & 0xFF;

  char temp_str[32] = "";
  array_to_string(temp_data, 8, temp_str);
  flush_serial_AT();

  mySerial1.print("AT+SEND=3:");
  mySerial1.println(temp_str);  // Envoi de la température, humair, sol et luminosité
  delay(10000);
}

int getLightData() {
  mySerial1.println("AT+LUM"); // Commande pour récupérer les données de luminosité
  flush_serial_AT(true);
  delay(1000);
  int lumValue = 0;
  if (mySerial1.available()) {
    String response = mySerial1.readStringUntil('\n'); // Lire la réponse
    lumValue = response.toInt(); // Convertir la réponse en entier
  }
  return lumValue;
}
