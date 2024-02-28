/*******************************************
********************************************
***** Code Actionneur Serre Connecté *******
************ Version 28/02/2024 ************
********************************************
*******************************************/

#include <Servo.h>

Servo myservo_droit;  // create servo object to control a servo
Servo myservo_gauche;  // En regardant devant l'ouverture 
#define Ventilator 2
#define Pompe 3


int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  myservo_droit.attach(9);    // En regardant devant l'ouverture 
  myservo_droit.write(0);     // Position Initiale
  myservo_gauche.attach(10);  //  En regardant devant l'ouverture 
  myservo_gauche.write(90);   // Position Initiale
  pinMode(Ventilator, OUTPUT);
  pinMode(Pompe, OUTPUT);
  //pompe();
}

void loop() {
  if (Serial.available() > 0) {
    int choix_action = Serial.read();
    if(choix_action == '1')
    {
      activate_ventilation(20);
    }
    else if(choix_action == '2')
    {
      pompe(4);
    }
    else if(choix_action == '3')
    {
      activate_ventilation(20);
      pompe(4);
    }
    else
    {
      Serial.println("Commande non comprise");
    }
  }
}

void pompe(int temps_pompe)
{
  digitalWrite (Pompe, HIGH); // "NO" est passant;
  delay(temps_pompe*1000);
  digitalWrite (Pompe, LOW); // "NC" est passant;
}


void activate_ventilation(int temps_ventilation)
{
  digitalWrite(Ventilator, HIGH);
  for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo_droit.write(pos);              // tell servo to go to position in variable 'pos'
    myservo_gauche.write(90-pos);
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  delay(temps_ventilation*1000);
  digitalWrite(Ventilator, LOW);
  for (pos = 60; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo_droit.write(pos);              // tell servo to go to position in variable 'pos'
    myservo_gauche.write(90-pos);
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  delay(1000);
}
