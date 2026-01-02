#include <Wire.h>

const int MPU_addr = 0x68; // Adresse I2C du MPU-6050 (0x68 par défaut)
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  Wire.begin();
  
  // Initialisation du MPU-6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // Registre de gestion d'alimentation (PWR_MGMT_1)
  Wire.write(0);     // Mettre à 0 réveille le MPU-6050
  Wire.endTransmission(true);
  
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // Préparation de la lecture des données
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // Adresse de départ des données (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  
  // Demande de 14 octets (Accéléromètre + Température + Gyroscope)
  Wire.requestFrom(MPU_addr, 14, true);
  
  // Lecture des registres (décalage de bits pour assembler les 16 bits)
  if(Wire.available() == 14) {
    AcX = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
    // Affichage des valeurs brutes
    Serial.print(AcX);
    Serial.print(" | "); Serial.print(AcY);
    Serial.print(" | "); Serial.print(AcZ);
    Serial.print(" | "); Serial.print(GyX);
    Serial.print(" | "); Serial.print(GyY);
    Serial.print(" | "); Serial.println(GyZ);
  } else {
    Serial.println("Erreur de connexion ou module non detecte !");
  }
  
  delay(500); // Pause de 500ms pour lisibilité
}