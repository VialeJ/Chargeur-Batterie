#include <Wire.h>
#include <Adafruit_INA219.h>

// Initialisation du capteur INA219
Adafruit_INA219 ina219;

// Adresse I2C de la fuel gauge BQ34Z100-G1 (vérifier avec un scanner I2C)
#define FUEL_GAUGE_ADDR 0x55

// Commandes I2C de la fuel gauge (16 bits, format Little Endian)
#define CMD_STATE_OF_CHARGE 0x02  // État de charge (%)
#define CMD_VOLTAGE         0x08  // Tension (mV)
#define CMD_TEMP            0x0C  // Température (0.1 K)

// Fonction pour lire une commande 16 bits depuis la fuel gauge
uint16_t readFuelGaugeCommand(uint8_t cmd) {
  Wire.beginTransmission(FUEL_GAUGE_ADDR);
  Wire.write(cmd); // Envoi de la commande
  if (Wire.endTransmission(false) != 0) { // Redémarrage sans STOP
    Serial.println("Erreur de communication avec la fuel gauge");
    return 0xFFFF; // Valeur d'erreur
  }

  // Demande 2 octets de réponse
  Wire.requestFrom(FUEL_GAUGE_ADDR, (uint8_t)2);
  
  if (Wire.available() < 2) {
    Serial.println("Pas assez de données reçues");
    return 0xFFFF; // Valeur d'erreur
  }

  // Lecture des deux octets (Little Endian -> LSB + MSB)
  uint16_t lsb = Wire.read();  
  uint16_t msb = Wire.read();
  uint16_t result = (msb << 8) | lsb;

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

  // Lecture de l'état de charge (State of Charge, SOC)
  uint16_t stateOfCharge = readFuelGaugeCommand(CMD_STATE_OF_CHARGE);
  if (stateOfCharge != 0xFFFF) {
    Serial.print("État de charge : ");
    Serial.print(stateOfCharge);
    Serial.println("%");
  } else {
    Serial.println("Erreur lors de la lecture de l'état de charge");
  }

  // Lecture de la tension de la batterie
  uint16_t voltage = readFuelGaugeCommand(CMD_VOLTAGE);
  if (voltage != 0xFFFF) {
    Serial.print("Tension (mV) : ");
    Serial.println(voltage);
  } else {
    Serial.println("Erreur lors de la lecture de la tension");
  }

  // Lecture de la température (convertie en °C)
  uint16_t temp = readFuelGaugeCommand(CMD_TEMP);
  if (temp != 0xFFFF) {
    float temperatureC = (temp * 0.1) - 273.15;  // Conversion de Kelvin à Celsius
    Serial.print("Température (°C) : ");
    Serial.println(temperatureC);
  } else {
    Serial.println("Erreur lors de la lecture de la température");
  }

  Serial.println("-----------------------------");
  delay(1000); // Pause de 1 seconde entre les lectures
}
