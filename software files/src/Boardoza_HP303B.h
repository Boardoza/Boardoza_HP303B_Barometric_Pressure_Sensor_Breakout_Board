#ifndef Boardoza_HP303B_h
#define Boardoza_HP303B_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <SPI.h>

// Register Adresleri
#define PSR_B2 0x00
#define PSR_B1 0x01
#define PSR_B0 0x02
#define TMP_B2 0x03
#define TMP_B1 0x04
#define TMP_B0 0x05
#define PRS_CFG 0x06
#define TMP_CFG 0x07
#define MEAS_CFG 0x08
#define CFG_REG 0x09
#define INT_STS 0x0A
#define FIFO_STS 0x0B
#define RESET 0x0C
#define PRODUCT_ID 0x0D

#define COEF_C0 0x10
#define COEF_C0C1 0x11
#define COEF_C1 0x12
#define COEF_C00 0x13
#define COEF_C10 0x16
#define COEF_C20 0x1C
#define COEF_C30 0x20
#define COEF_C01 0x18
#define COEF_C11 0x1A
#define COEF_C21 0x1E

#define SCALE_FACTOR_1

#define HP303B_I2C_ADDRESS 0x77

enum Mode {
    IDLE = 0x00,
    CMD_PRES = 0x01,
    CMD_TEMP = 0x02,
    INV_OP_CMD_BOTH = 0x03,
    INV_OP_CONT_NONE = 0x04,
    CONT_PRES = 0x05,
    CONT_TEMP = 0x06,
    CONT_BOTH = 0x07
};

extern int16_t prs_osr;
extern int16_t temp_osr;

class Boardoza_HP303B {
public:
    Boardoza_HP303B(void);
    
    // Begin fonksiyonları
    void begin(TwoWire &wire, uint8_t slaveAddress);
    void begin(uint8_t slaveAddress = HP303B_I2C_ADDRESS);
    void begin(SPIClass &spi, int32_t chipSelect);

    void end(void);
    
    int16_t readReg();
    int32_t measureRawTemp();
    int32_t measureTemperature();
    int32_t measureRawPres();
    int32_t measurePressure();
    
    int16_t setInterruptPolarity(uint8_t polarity);
    int16_t setInterruptSources(uint8_t fifoFull, uint8_t tempReady, uint8_t presReady);
    
    // Konfigürasyon ve Yardımcılar
    int16_t correctTemp(void);
    void setOpMode(Mode opMode);
    void configTemp(bool temp_ext, uint8_t temp_mr, uint8_t temp_osr);
    void configPressure(uint8_t prs_mr, uint8_t prs_osr);
    void configIntFIFO();
    
    // Düşük seviye okuma/yazma (Public yaptıkların)
    int16_t readByte(uint8_t regAddress);
    int16_t readBlock(uint8_t regAddress, uint8_t length, uint8_t *buffer);
    int16_t writeByte(uint8_t regAddress, uint8_t data);
    int16_t readByteBitfield(uint8_t regAddress, uint8_t mask, uint8_t shift);

private:
    uint8_t _chipSelect;
    TwoWire *_wire;
    SPIClass *_spi;
    uint8_t _i2cAddress;
    
    // Hangi protokolün kullanıldığını takip etmek için:
    bool _useSPI; 
};

#endif