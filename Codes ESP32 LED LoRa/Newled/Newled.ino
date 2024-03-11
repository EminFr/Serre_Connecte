#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
//#include "SSD1306.h"
#include <Adafruit_SSD1306.h> 

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String rssi = "RSSI --";
String packSize = "--";
String packet ;
int counter = 0;


void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high、
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);  
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");
  display.setTextSize(1); 
  display.clearDisplay();

  display.display();
  display.setTextColor(SSD1306_WHITE);
  display.println("YAHOUUUUUUUUU");
  display.display();
  delay(1500);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
        // received a packet
        counter ++;
        Serial.print("Received packet '");
        String recv = "";
        // read packet
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Donnees recu : ");
        display.print(recv);
        display.display();
        delay(10);
}
}
