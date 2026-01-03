#include <Wire.h>

float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int loop_count;

float AngleYaw = 0.0;         
unsigned long previousTime;   
float dt;                     

void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A); Wire.write(0x05); Wire.endTransmission(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); Wire.write(0x8); Wire.endTransmission(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x43); Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
}

void setup() {
  Serial.begin(57600); 
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

   
  Serial.println("Calibration");
  
  for (loop_count = 0; loop_count < 2000; loop_count++) {
    gyro_signals();
    RateCalibrationYaw += RateYaw;
    delay(1);
  }
  
  RateCalibrationYaw /= 2000;
  
  Serial.println("Calibration terminee !");
  
  previousTime = micros(); 
}

void loop() {
  gyro_signals();

  // calibration
  RateYaw -= RateCalibrationYaw;

  unsigned long currentTime = micros();
  dt = (currentTime - previousTime) / 1000000.0; //seconds
  previousTime = currentTime;

  // angle 
  if (abs(RateYaw) > 0.5) { 
      AngleYaw += RateYaw * dt;
  }
  
  Serial.print("Ref_Zero:0");     
  Serial.print(0); 
  Serial.print(" ");
  Serial.print("Angle_Degres:");   
  Serial.println(AngleYaw); 

}