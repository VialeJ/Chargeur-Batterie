#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial); // Attendre que le port série soit prêt (utile pour les cartes comme Leonardo)
  Serial.println("Scan I2C en cours...");
}

void loop() {
  byte erreur, adresse;
  int nb_peripheriques = 0;

  for (adresse = 1; adresse < 127; adresse++) {
    Wire.beginTransmission(adresse);
    erreur = Wire.endTransmission();

    if (erreur == 0) {
      Serial.print("Peripherique I2C trouve a l'adresse 0x");
      if (adresse < 16) Serial.print('0');
      Serial.println(adresse, HEX);
      nb_peripheriques++;
    } else if (erreur == 4) {
      Serial.print("Erreur inconnue a l'adresse 0x");
      if (adresse < 16) Serial.print('0');
      Serial.println(adresse, HEX);
    }
  }

  if (nb_peripheriques == 0)
    Serial.println("Aucun peripherique I2C trouve.");
  else
    Serial.println("Scan termine.");

  delay(5000); // Attendre 5 secondes avant un nouveau scan
}
