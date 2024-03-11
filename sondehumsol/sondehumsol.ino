#define solPin 0// port de connexion du capteur

void setup(){
   Serial.begin(115200);
   pinMode(solPin, INPUT);
}

void loop(){
   Serial.print("sol = ");
   Serial.println(analogRead(solPin)); // afficher variable
   delay(1000);
}