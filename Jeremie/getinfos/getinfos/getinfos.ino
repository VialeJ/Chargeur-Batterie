#include <Wire.h>
#include <Adafruit_INA219.h>

// Initialisation de l'objet INA219
Adafruit_INA219 ina219;

// Adresse I2C de la fuel gauge
#define FUEL_GAUGE_ADDR 0x55

// Commandes pour la fuel gauge (exemples)
#define CMD_STATE_OF_CHARGE 0x1C // Lecture de l'état de charge (%)
#define CMD_VOLTAGE 0x04         // Lecture de la tension (mV)

// Fonction pour lire une commande de la fuel gauge
uint16_t readFuelGaugeCommand(uint8_t cmd) {
  Wire.beginTransmission(FUEL_GAUGE_ADDR);
  Wire.write(cmd); // Envoi de la commande
  if (Wire.endTransmission(false) != 0) { // Redémarrage sans STOP
    Serial.println("Erreur de communication avec la fuel gauge");
    return 0xFFFF; // Valeur d'erreur
  }
  Wire.requestFrom(FUEL_GAUGE_ADDR, (uint8_t)2);
  
  if (Wire.available() < 2) {
    Serial.println("Pas assez de données reçues");
    return 0xFFFF; // Valeur d'erreur
  }

  // Lecture des deux octets (Big Endian)
  uint16_t result = Wire.read() << 8;
  result |= Wire.read();

  return result;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Initialisation du bus I2C

  // Initialisation du capteur INA219
  if (!ina219.begin()) {
    Serial.println("Impossible de trouver le capteur INA219 !");
    while (1); // Boucle infinie si le capteur n'est pas détecté
  }
  Serial.println("Capteur INA219 initialisé avec succès !");
}

void loop() {
  // Lecture du courant via INA219
  float current_mA = ina219.getCurrent_mA();
  Serial.print("Courant (mA) : ");
  Serial.println(current_mA);

  // Lecture de l'état de charge (State of Charge, SOC) via fuel gauge
  uint16_t stateOfCharge = readFuelGaugeCommand(CMD_STATE_OF_CHARGE);
  if (stateOfCharge != 0xFFFF) { // Vérification si la lecture est valide
    Serial.print("État de charge (%) : ");
    Serial.println(stateOfCharge);
  } else {
    Serial.println("Erreur lors de la lecture de l'état de charge");
  }

  // Lecture de la tension de la batterie via fuel gauge
  uint16_t voltage = readFuelGaugeCommand(CMD_VOLTAGE);
  if (voltage != 0xFFFF) { // Vérification si la lecture est valide
    Serial.print("Tension (mV) : ");
    Serial.println(voltage);
  } else {
    Serial.println("Erreur lors de la lecture de la tension");
  }

  Serial.println("-----------------------------");
  delay(1000); // Pause de 1 seconde entre les lectures
}
