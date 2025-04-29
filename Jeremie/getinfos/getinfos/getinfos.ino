#include <Wire.h>
#include <Adafruit_INA219.h>

#define FUEL_GAUGE_ADDR 0x55

// Commandes I2C de la fuel gauge
#define CMD_STATE_OF_CHARGE 0x02  // État de charge (%)
#define CMD_VOLTAGE         0x08  // Tension (mV)
#define CMD_TEMP            0x0C  // Température (0.1 K)

#define DEBUG true

// Initialisation du capteur INA219
Adafruit_INA219 ina219;

// Variables pour la gestion des intervalles non bloquants
unsigned long previousMillisFuel = 0;
const unsigned long fuelInterval = 5000; // Mise à jour de la fuel gauge toutes les 5000 ms

unsigned long previousMillisCurrent = 0;
const unsigned long currentInterval = 1000; // Mise à jour du courant toutes les 1000 ms

// Envoi d'une commande de contrôle (par exemple IT_ENABLE)
void sendControlCommand(uint16_t subcmd) {
  Wire.beginTransmission(FUEL_GAUGE_ADDR);
  Wire.write(0x00);  // Adresse du registre Control()
  Wire.write(subcmd & 0xFF);         // LSB
  Wire.write((subcmd >> 8) & 0xFF);    // MSB
  Wire.endTransmission();
  if(DEBUG) {
    Serial.print("Commande de contrôle (0x");
    Serial.print(subcmd, HEX);
    Serial.println(") envoyée");
  }
}

// Lecture d'un octet (utilisé pour le SoC)
uint8_t readFuelGaugeByte(uint8_t cmd) {
  Wire.beginTransmission(FUEL_GAUGE_ADDR);
  Wire.write(cmd);
  if(Wire.endTransmission(false) != 0) {
    if(DEBUG) Serial.println("Erreur de communication (byte)");
    return 0xFF;
  }
  Wire.requestFrom(FUEL_GAUGE_ADDR, (uint8_t)1);
  if(Wire.available() < 1) {
    if(DEBUG) Serial.println("Pas assez de données (byte)");
    return 0xFF;
  }
  return Wire.read();
}

// Lecture de 2 octets en format little endian (utilisé pour tension, température, etc.)
uint16_t readFuelGaugeWord(uint8_t cmd) {
  Wire.beginTransmission(FUEL_GAUGE_ADDR);
  Wire.write(cmd);
  if(Wire.endTransmission(false) != 0) {
    if(DEBUG) Serial.println("Erreur de communication (word)");
    return 0xFFFF;
  }
  Wire.requestFrom(FUEL_GAUGE_ADDR, (uint8_t)2);
  if(Wire.available() < 2) {
    if(DEBUG) Serial.println("Pas assez de données (word)");
    return 0xFFFF;
  }
  uint16_t lsb = Wire.read();
  uint16_t msb = Wire.read();
  return (msb << 8) | lsb;
}

// Fonction regroupant les lectures de la fuel gauge
void updateFuelGauge() {
  uint8_t soc = readFuelGaugeByte(CMD_STATE_OF_CHARGE);
  if(soc != 0xFF) {
    Serial.print("Etat de charge : ");
    Serial.print(soc);
    Serial.println("%");
  } else {
    Serial.println("Erreur lors de la lecture de l'état de charge");
  }
  
  uint16_t voltage = readFuelGaugeWord(CMD_VOLTAGE);
  if(voltage != 0xFFFF) {
    Serial.print("Tension (mV) : ");
    Serial.println(voltage);
  } else {
    Serial.println("Erreur lors de la lecture de la tension");
  }
  
  uint16_t temp = readFuelGaugeWord(CMD_TEMP);
  if(temp != 0xFFFF) {
    float temperatureC = (temp * 0.1) - 273.15;
    Serial.print("Température (°C) : ");
    Serial.println(temperatureC);
  } else {
    Serial.println("Erreur lors de la lecture de la température");
  }
  
  Serial.println("-----------------------------");
}



void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialisation du capteur INA219
  if (!ina219.begin()) {
    Serial.println("Impossible de trouver le capteur INA219 !");
    while (1);
  }
  Serial.println("Capteur INA219 initialisé avec succès !");
  
  // Délai initial uniquement lors du setup (peut être raccourci si possible)
  delay(300);
  
  // Activation du suivi d'impédance avec IT_ENABLE
  sendControlCommand(0x0021);
  if(DEBUG) Serial.println("Commande IT_ENABLE envoyée.");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Mise à jour non bloquante de la lecture du courant via INA219
  if(currentMillis - previousMillisCurrent >= currentInterval) {
    previousMillisCurrent = currentMillis;
    float current_mA = ina219.getCurrent_mA();
    Serial.print("Courant (mA) : ");
    Serial.println(current_mA);
  }
  
  // Mise à jour non bloquante des lectures de la fuel gauge
  if(currentMillis - previousMillisFuel >= fuelInterval) {
    previousMillisFuel = currentMillis;
    updateFuelGauge();
  }
}
