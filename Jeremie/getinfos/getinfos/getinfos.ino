#include <Wire.h>
#include <Adafruit_INA219.h>

// Initialisation de l'objet INA219
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(115200);
  // Initialisation du bus I2C et du capteur INA219
  if (!ina219.begin()) {
    Serial.println("Impossible de trouver le capteur INA219 !");
    while (1); // Boucle infinie si le capteur n'est pas détecté
  }
  Serial.println("Capteur INA219 initialisé avec succès !");
}

void loop() {
  float shuntVoltage = ina219.getShuntVoltage_mV(); // Tension sur la résistance shunt (mV)
  float busVoltage = ina219.getBusVoltage_V();      // Tension sur le bus (V)
  float current_mA = ina219.getCurrent_mA();        // Courant mesuré (mA)
  float power_mW = ina219.getPower_mW();            // Puissance mesurée (mW)

  // Affichage des mesures
  Serial.print("Tension bus (V) : ");
  Serial.println(busVoltage);
  Serial.print("Tension shunt (mV) : ");
  Serial.println(shuntVoltage);
  Serial.print("Courant (mA) : ");
  Serial.println(current_mA);
  Serial.print("Puissance (mW) : ");
  Serial.println(power_mW);
  Serial.println("-----------------------------");

  delay(1000); // Attendre une seconde avant la prochaine mesure
}
