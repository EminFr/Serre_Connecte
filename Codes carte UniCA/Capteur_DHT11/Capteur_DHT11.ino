// Insertion de la librairie Adafruit_DHT
#include "DHT.h"
 
// déclaration de la broche d'entrée
#define DHTPIN 10   
 
// initialisation du capteur
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
 
void setup() 
{
  Serial.begin(9600);
  Serial.println("Test du module KY-015 Test - Temperature et humidite:");
 
  // début de la mesure
  dht.begin();
}
 
// Boucle de programme principale
// Début du programme et lecture des mesures
// Une pause de 2 secondes est insérée entre 2 mesures
void loop() {
 
  // délai de 2 secondes entre 2 mesures
  delay(2000);
 
  // mesure de l'humidité
  float h = dht.readHumidity();
  // mesure de la température
  float t = dht.readTemperature();
   
  // on vérifie si les mesures sont exécutées sans faute
  // Lors de la détection d'une erreur, affichage d'un message d'erreur
  if (isnan(h) || isnan(t)) {
    Serial.println("Erreur de lecture du capteur");
    return;
  }
 
  // Envoi dans la console série
  Serial.println("-----------------------------------------------------------");
  Serial.print("Humidite: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(char(186)); //Affiche le symbole <°>
  Serial.println("C ");
  Serial.println("-----------------------------------------------------------");
  Serial.println(" ");
}