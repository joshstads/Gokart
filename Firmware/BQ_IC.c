#include <Wire.h>

#define I2C_SDA_PIN 21 
#define I2C_SCL_PIN 22 
#define BQ769x2_ADDR 0x08


uint16_t readVoltage(uint8_t command);
void readAllVoltages();
void BQ769x2_Init();

uint16_t CellVoltage[16] = {0};

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  BQ769x2_Init();
}

void loop() {
  readAllVoltages();

  for (int i = 0; i < 16; i++) {
    Serial.print("Cell ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(CellVoltage[i]);
    Serial.println(" mV");
  }
  delay(1000);
}

void BQ769x2_Init() {
  Wire.beginTransmission(BQ769x2_ADDR);
  // Initialization for BQ769x2, configuration commands, change settings
  Wire.endTransmission();

}

uint16_t readVoltage(uint8_t command) {
  uint8_t data[2] = {0};

  Wire.beginTransmission(BQ769x2_ADDR);
  Wire.write(command); 
  Wire.endTransmission();
  Wire.requestFrom(BQ769x2_ADDR, 2); 
  if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read(); 
  }

  return (data[1] << 8) | data[0]; 
}

void readAllVoltages() {
  uint8_t cellVoltageCommand = 0x14; 

  for (int i = 0; i < 16; i++) {
    CellVoltage[i] = readVoltage(cellVoltageCommand);
    cellVoltageCommand += 2;
  }
}
