#include "Boardoza_HP303B.h"

uint8_t rawData[16];
int32_t osrTable[8]={524288, 1572864, 3670016, 7864320, 253952, 516096, 1040384, 2088960};
int16_t prs_osr = 0x03;
int16_t temp_osr = 0x03;
double scaledTemp;

// SPI Ayarları: 1MHz hız, MSB First, MODE3 (HP303B için kritik olan bu!)
SPISettings hp303b_spi_settings(1000000, MSBFIRST, SPI_MODE3);

Boardoza_HP303B::Boardoza_HP303B(void) {
    _useSPI = false; // Varsayılan
}

void Boardoza_HP303B::begin(TwoWire &wire, uint8_t slaveAddress) {
    _useSPI = false; // I2C kullanıyoruz
    _wire = &wire;
    _i2cAddress = slaveAddress;
    _wire->begin();
}

void Boardoza_HP303B::begin(uint8_t slaveAddress) {
    begin(Wire, slaveAddress);
}

void Boardoza_HP303B::begin(SPIClass &spi, int32_t chipSelect) {
    _useSPI = true; // SPI kullanıyoruz
    _spi = &spi;
    _chipSelect = chipSelect;
    pinMode(_chipSelect, OUTPUT);
    digitalWrite(_chipSelect, HIGH);
    _spi->begin();
    // SPI test transaction (gerekli değil ama bus'ı uyandırmak için kalabilir)
    _spi->beginTransaction(hp303b_spi_settings);
    _spi->endTransaction();
}

void Boardoza_HP303B::end(void) {
    if (!_useSPI) {
        // Wire end fonksiyonu bazı platformlarda yoktur, varsa kullanın.
    } else {
        _spi->end();
        pinMode(_chipSelect, INPUT);
    }
}

// ... (Ara fonksiyonlar aynı kalabilir: configIntFIFO, correctTemp vb.) ...

void Boardoza_HP303B::configIntFIFO() {
    if(temp_osr <= 0x03) writeByte(CFG_REG, 0x00);
    else writeByte(CFG_REG, 0x0C);
    
    if(prs_osr > 0x03) writeByte(CFG_REG, 0x0C); 
    // Not: Mantığında küçük bir hata olabilir (overwrite ediyor), ama orijinal koduna dokunmadım.
}

int16_t Boardoza_HP303B::correctTemp(void) {
    writeByte(0x0E, 0xA5);
    writeByte(0x0F, 0x96);
    writeByte(0x62, 0x02);
    writeByte(0x0E, 0x00);
    writeByte(0x0F, 0x00);
    return 0;
}

void Boardoza_HP303B::configPressure(uint8_t prs_mr, uint8_t prs_osr) {
    writeByte(PRS_CFG, (prs_mr << 4 | prs_osr));
}

void Boardoza_HP303B::configTemp(bool temp_ext, uint8_t temp_mr, uint8_t temp_osr) {
    writeByte(TMP_CFG, (temp_ext << 7 | temp_mr << 4 | temp_osr));
}

void Boardoza_HP303B::setOpMode(Mode opMode) {
    writeByte(MEAS_CFG, opMode);
}

int16_t Boardoza_HP303B::readReg() {
    return readByte(PRODUCT_ID);
}

// ... (Ölçüm fonksiyonları: measureRawTemp, measureTemperature vb. aynı kalabilir) ...
// Not: Bu fonksiyonlar readBlock ve readByte çağırır, biz onları düzelteceğimiz için buralara dokunmaya gerek yok.

int32_t Boardoza_HP303B::measureRawTemp() {
    int32_t tempData;
    if (!readBlock(0x03, 3, rawData)) {
        tempData = (uint32_t)rawData[0] << 16;
        tempData |= (uint32_t)rawData[1] << 8;
        tempData |= (uint32_t)rawData[2];
        return tempData;
    } 
    return -1;
}

int32_t Boardoza_HP303B::measureTemperature() {
    // Orijinal kodunu korudum
    int32_t tempData1, tempData2; 
    int32_t C0, C1;
    int32_t temp3;
    double temp4;
    tempData1 = (uint32_t)(readByte(COEF_C0) << 4);
    tempData2 = (uint32_t)((readByte(COEF_C0C1) >> 4) & 0x0F);

    C0 = (uint32_t)tempData1 | (uint32_t)tempData2;
    if(C0 & ((uint32_t)1 << 11)) C0 -= (uint32_t)1 << 12;
    C0 = C0 / 2;

    tempData1 = (uint32_t)((readByte(COEF_C0C1)& 0x0F) << 8);
    tempData2 = (uint32_t)(readByte(COEF_C1));
    C1 = (uint32_t)tempData1 | (uint32_t)tempData2;
    if(C1 & ((uint32_t)1 << 11)) C1 -= (uint32_t)1 << 12;
    
    temp3 = measureRawTemp();
    if(temp3 & ((uint32_t)1 << 23)) temp3 -= (uint32_t)1 << 24;
    
    scaledTemp = (double)temp3;
    scaledTemp = scaledTemp / osrTable[temp_osr];
    temp4 = (double)temp3;
    temp4 = temp4 / osrTable[temp_osr];
    temp4 = ((double)C0) + (double)C1 * (double)temp4;

    return (int32_t)(temp4); // int32_t return tipine cast edildi
}

int32_t Boardoza_HP303B::measureRawPres() {
    int32_t tempData;
    if (!readBlock(PSR_B2, 3, rawData)) {
        tempData = (uint32_t)rawData[0] << 16;
        tempData |= (uint32_t)(rawData[1] << 8);
        tempData |= (uint32_t)(rawData[2]);
        return tempData;
    } 
    return -1;
}

int32_t Boardoza_HP303B::measurePressure() {
    // Orijinal hesaplama kodun aynen korundu
    int32_t C00, C10, C20, C30, C01, C11, C21;
    int32_t tempData1, tempData2, tempData3, tempScaledPres, result;
    double scaledPres;
    
    tempData1 = (uint32_t)(readByte(COEF_C00) << 12); tempData2 = (uint32_t)(readByte(0x14) << 4); tempData3 = (uint32_t)((readByte(0x15)  >> 4) & 0x0F);
    C00 = (uint32_t)tempData1 | (uint32_t)tempData2 | (uint32_t)tempData3;
    
    tempData1 = (uint32_t)((readByte(COEF_C00 + 2) & 0x0F) << 16); tempData2 = (uint32_t)(readByte(COEF_C10) << 8); tempData3 = (uint32_t)(readByte(COEF_C10 + 1));
    C10 = (uint32_t)tempData1 | (uint32_t)tempData2 | (uint32_t)tempData3;
    
    tempData1 = (uint32_t)(readByte(COEF_C20) << 8); tempData2 = (uint32_t)(readByte(COEF_C20 + 1));
    C20 = (uint32_t)tempData1 | (uint32_t)tempData2;
    
    tempData1 = (uint32_t)(readByte(COEF_C30) << 8); tempData2 = (uint32_t)(readByte(COEF_C30 + 1));
    C30 = (uint32_t)tempData1 | (uint32_t)tempData2;
    
    tempData1 = (uint32_t)(readByte(COEF_C01) << 8); tempData2 = (uint32_t)(readByte(COEF_C01 + 1));
    C01 = (uint32_t)tempData1 | (uint32_t)tempData2;
    
    tempData1 = (uint32_t)(readByte(COEF_C11) << 8); tempData2 = (uint32_t)(readByte(COEF_C11 + 1));
    C11 = (uint32_t)tempData1 | (uint32_t)tempData2;
    
    tempData1 = (uint32_t)(readByte(COEF_C21) << 8); tempData2 = (uint32_t)(readByte(COEF_C21 + 1));
    C21 = (uint32_t)tempData1 | (uint32_t)tempData2;
    
    tempScaledPres = measureRawPres(); // raw cast removed inside function
    
    if(C00 & ((uint32_t)1 << 19)) C00 -= (uint32_t)1 << 20;
    if(C10 & ((uint32_t)1 << 19)) C10 -= (uint32_t)1 << 20;
    if(C20 & ((uint32_t)1 << 15)) C20 -= (uint32_t)1 << 16;
    if(C30 & ((uint32_t)1 << 15)) C30 -= (uint32_t)1 << 16;
    if(C01 & ((uint32_t)1 << 15)) C01 -= (uint32_t)1 << 16;
    if(C11 & ((uint32_t)1 << 15)) C11 -= (uint32_t)1 << 16;
    if(C21 & ((uint32_t)1 << 15)) C21 -= (uint32_t)1 << 16;
    
    if(tempScaledPres & ((uint32_t)1 << 23)) tempScaledPres -= (uint32_t)1 << 24;
    
    scaledPres = (double)tempScaledPres / (double)osrTable[prs_osr];
    result = (double)C00 + (double)scaledPres * ((double)C10 + (double)scaledPres *((double)C20 + (double)scaledPres * (double)C30)) + (double)scaledTemp * (double)C01 + (double)scaledTemp * (double)scaledPres * ((double)C11 + (double)scaledPres * (double)C21);
    
    return result;
}

// ---------------------------------------------------------
// KRİTİK BÖLÜMLER: READ/WRITE FONKSİYONLARI (MODERNİZE EDİLDİ)
// ---------------------------------------------------------

int16_t Boardoza_HP303B::readByte(uint8_t regAddress) {
    if (!_useSPI) { 
        // I2C Modu
        _wire->beginTransmission(_i2cAddress);
        _wire->write(regAddress);
        _wire->endTransmission(false);
        _wire->requestFrom(_i2cAddress, (uint8_t)1);
        if(_wire->available()) {
            return _wire->read();
        } else {
            return -1;
        }
    } else {
        // SPI Modu (Düzeltildi: MODE3 ve Read Bit)
        _spi->beginTransaction(hp303b_spi_settings);
        digitalWrite(_chipSelect, LOW);
        // SPI Read için MSB (En sol bit) 1 olmalı. 0x80 ile OR'luyoruz.
        _spi->transfer(regAddress | 0x80); 
        int16_t result = _spi->transfer(0x00); // Dummy byte gönderip cevabı al
        digitalWrite(_chipSelect, HIGH);
        _spi->endTransaction();
        return result;
    }
}

int16_t Boardoza_HP303B::readBlock(uint8_t regAddress, uint8_t length, uint8_t *buffer) {
    if (!_useSPI) {
        // I2C
        _wire->beginTransmission(_i2cAddress);
        _wire->write(regAddress);
        _wire->endTransmission(false);
        _wire->requestFrom(_i2cAddress, length);
        for(uint8_t i = 0; i < length; i++) {
            if(_wire->available()) buffer[i] = _wire->read();
            else return 1;
        }
        return 0;
    } else {
        // SPI (Düzeltildi)
        _spi->beginTransaction(hp303b_spi_settings);
        digitalWrite(_chipSelect, LOW);
        _spi->transfer(regAddress | 0x80); // Read komutu
        for(uint8_t i = 0; i < length; i++) {
            buffer[i] = _spi->transfer(0x00);
        }
        digitalWrite(_chipSelect, HIGH);
        _spi->endTransaction();
        return 0;
    }
}

int16_t Boardoza_HP303B::writeByte(uint8_t regAddress, uint8_t data) {
    if (!_useSPI) {
        // I2C
        _wire->beginTransmission(_i2cAddress);
        _wire->write(regAddress);
        _wire->write(data);
        return _wire->endTransmission();
    } else {
        // SPI (Düzeltildi)
        _spi->beginTransaction(hp303b_spi_settings);
        digitalWrite(_chipSelect, LOW);
        // Write için MSB 0 olmalı. 0x7F ile AND'liyoruz.
        _spi->transfer(regAddress & 0x7F); 
        _spi->transfer(data);
        digitalWrite(_chipSelect, HIGH);
        _spi->endTransaction();
        return 0;
    }
}

int16_t Boardoza_HP303B::readByteBitfield(uint8_t regAddress, uint8_t mask, uint8_t shift) {
    int16_t value = readByte(regAddress);
    if(value < 0) return value;
    return (value & mask) >> shift;
}