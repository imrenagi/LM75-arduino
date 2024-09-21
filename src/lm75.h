#ifndef LM75_H
#define LM75_H

#include <Arduino.h>
#include <Wire.h>

class LM75 {
  public:
    static const uint8_t default_i2c_address = 0x48; //0b01001000

  protected:

    enum ConfigurationBits {
      Shutdown        = 0,
      ComparatorMode  = 1,
      OSPolarity      = 2,
      FaultQueue      = 3,
    };

    TwoWire *bus;
    uint8_t i2c_address;
    uint8_t resolution;
    uint16_t resolution_mask;
    float temperature_frac_factor;

    int16_t readIntegerTemperatureRegister(uint8_t register_idx);
    void writeIntegerTemperatureRegister(uint8_t register_idx, int16_t value);

    int8_t readIntegerConfigurationRegister();
    void writeIntegerConfigurationRegister(int8_t value);

    void setConfigurationBit(uint8_t bit);
    void clearConfiguration(uint8_t bit);
    void setConfigurationBitValue(uint8_t value, uint8_t start, uint8_t width);

    LM75(TwoWire *bus, uint8_t i2c_address) {
      this->bus = bus;
      this->i2c_address = i2c_address;
      this->resolution = 9;
      this->resolution_mask = 0xFF80;
      this->temperature_frac_factor = 1.0 / (float)(1 << 8);
    }

    inline float convertCToF(float c) {
      return c * 9.0 / 5.0 + 32.0;
    }

    inline float convertFToC(float f) {
      return (f - 32.0) * 5.0 / 9.0;
    }

  public:
    inline float convertIntegerTemperature(int16_t value) {
      return (float)value * this->temperature_frac_factor;
    }

    inline int16_t convertTemperatureToInteger(float value) {
      return (int16_t)(value / this->temperature_frac_factor);
    }

    float readTemperature() {
      return this->convertIntegerTemperature(this->readIntegerTemperatureRegister(0x00));
    }

    float readOverTemperatureShutdown() {
      return this->convertIntegerTemperature(this->readIntegerTemperatureRegister(0x03));
    }

    float readHysteresisTemperature() {
      return this->convertIntegerTemperature(this->readIntegerTemperatureRegister(0x02));
    }

    void setOverTemperatureShutdown(float value) {
      this->writeIntegerTemperatureRegister(0x03, this->convertTemperatureToInteger(value));      
    }

    void setHysteresisTemperature(float value) {
      this->writeIntegerTemperatureRegister(0x02, this->convertTemperatureToInteger(value));
    }

    void shutdownOperation() {
      this->setConfigurationBit(bit(ConfigurationBits::Shutdown));
    }

    void normalOperation() {
      this->clearConfiguration(bit(ConfigurationBits::Shutdown));
    }

    void enableComparatorMode() {
      this->clearConfiguration(bit(ConfigurationBits::ComparatorMode));
    }

    void enableInterruptMode() {
      this->setConfigurationBit(bit(ConfigurationBits::ComparatorMode));
    }

    void setOSActiveLow() {
      this->clearConfiguration(bit(ConfigurationBits::OSPolarity));
    }

    void setOSActiveHigh() {
      this->setConfigurationBit(bit(ConfigurationBits::OSPolarity));
    }

    enum FaultQueueValues {
      FaultQueue1 = 0x0,
      FaultQueue2 = 0x1,
      FaultQueue4 = 0x2,
      FaultQueue6 = 0x3,
    };

    void setFaultQueue(enum FaultQueueValues value) {
      this->setConfigurationBitValue(value, 
      ConfigurationBits::FaultQueue, 2);
    }
};

class Concrete_LM75 : public LM75 {
public:
  Concrete_LM75(TwoWire *bus = &Wire, uint8_t i2c_address = default_i2c_address)
    : LM75(bus, i2c_address) { };

  Concrete_LM75(uint8_t i2c_address)
    : LM75(&Wire, i2c_address) { };
};

#endif