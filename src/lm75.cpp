#include "lm75.h"

int16_t LM75::readIntegerTemperatureRegister(uint8_t register_idx) {
  this->bus->beginTransmission(this->i2c_address);
  this->bus->write(register_idx);
  this->bus->endTransmission(false);

  this->bus->requestFrom(this->i2c_address, 2);
  int16_t temperature = this->bus->read() << 8;
  temperature |= this->bus->read();
  this->bus->endTransmission();
  temperature &= this->resolution_mask;
  return temperature;
}

void LM75::writeIntegerTemperatureRegister(uint8_t register_idx, int16_t value) {
  this->bus->beginTransmission(this->i2c_address);
  this->bus->write(register_idx);
  this->bus->write(value >> 8);
  this->bus->write(value & 0xFF);
  this->bus->endTransmission();
}

int8_t LM75::readIntegerConfigurationRegister() {
  this->bus->beginTransmission(this->i2c_address);
  this->bus->write(0x01);
  this->bus->endTransmission(false);

  this->bus->requestFrom(this->i2c_address, 1);
  int8_t configuration = this->bus->read();
  this->bus->endTransmission();
  return configuration;
}

void LM75::writeIntegerConfigurationRegister(int8_t value) {
  this->bus->beginTransmission(this->i2c_address);
  this->bus->write(0x01);
  this->bus->write(value);
  this->bus->endTransmission();
}

void LM75::setConfigurationBit(uint8_t bit) {
  uint8_t configuration = this->readIntegerConfigurationRegister();
  configuration |= bit;
  this->writeIntegerConfigurationRegister(configuration);
}

void LM75::clearConfiguration(uint8_t bit) {
  uint8_t configuration = this->readIntegerConfigurationRegister();
  configuration &= ~bit;  
  this->writeIntegerConfigurationRegister(configuration);
}

void LM75::setConfigurationBitValue(uint8_t value, uint8_t start, uint8_t width) {
  uint8_t configuration = this->readIntegerConfigurationRegister();
  uint8_t mask = ((1 << width) - 1) << start;
  configuration &= ~mask;
  configuration |= (value << start);
  this->writeIntegerConfigurationRegister(configuration);
}
