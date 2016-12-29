#include "application.h"
#include "ReclaimerLabs_ADS1118.h"

static uint32_t writeSPI32(uint32_t data_in, uint16_t pin_CS) {
    uint32_t data_out=0;
    
    SPI.begin(SPI_MODE_MASTER);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1);
    // max clock speed is 4 MHz, but we'll use 2 MHz to be safe
    SPI.setClockSpeed(2, MHZ);
    
    digitalWrite(pin_CS, LOW);
    delayMicroseconds(1);
    data_out  = ((SPI.transfer((data_in>>24) & 0xFF)) << 24);
    data_out |= ((SPI.transfer((data_in>>16) & 0xFF)) << 16);
    data_out |= ((SPI.transfer((data_in>>8) & 0xFF)) << 8);
    data_out |=  (SPI.transfer( data_in     & 0xFF));
    delayMicroseconds(1);
    digitalWrite(pin_CS, HIGH);
    
    SPI.end();
    delayMicroseconds(1);
    
    return data_out;
}

static uint16_t writeSPI16(uint16_t data_in, uint16_t pin_CS) {
    uint16_t data_out=0;
    
    SPI.begin(SPI_MODE_MASTER);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1);
    // max clock speed is 4 MHz, but we'll use 2 MHz to be safe
    SPI.setClockSpeed(2, MHZ);
    
    digitalWrite(pin_CS, LOW);
    delayMicroseconds(1);
    data_out |= ((SPI.transfer((data_in>>8) & 0xFF)) << 8);
    data_out |=  (SPI.transfer( data_in     & 0xFF));
    delayMicroseconds(1);
    digitalWrite(pin_CS, HIGH);
    
    SPI.end();
    delayMicroseconds(1);
    
    return data_out;
}

ReclaimerLabs_ADS1118::ReclaimerLabs_ADS1118(uint16_t pin_CS) {
    m_pin_CS = pin_CS;
    m_gain   = GAIN_SIXTEEN;
}

void ReclaimerLabs_ADS1118::setGain(adsGain_t gain)
{
  m_gain = gain;
}

adsGain_t ReclaimerLabs_ADS1118::getGain()
{
  return m_gain;
}

uint16_t ReclaimerLabs_ADS1118::readADC_SingleEnded(uint8_t channel) {
    int16_t return_val;
    
    if (channel > 3) {
        return 0;
    }
    
    uint16_t config = ADS1118_CONFIG_SS_START_ON |  // turn on single shot mode
                      ADS1118_CONFIG_MODE_SINGLE |  // power down after sample
                      ADS1118_CONFIG_DR_128SPS   |  // 128 samples per sec (default)
                      ADS1118_CONFIG_TS_MODE_ADC |  // read ADC, not temp
                      ADS1118_CONFIG_PULL_UP_ON  |  // pull-ups on (default)
                      ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
                      ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
    
    config |= this->m_gain;
    
    switch (channel) {
        case(0):
            config |= ADS1118_CONFIG_MUX_SINGLE_0;
            break;
        case(1):
            config |= ADS1118_CONFIG_MUX_SINGLE_1;
            break;
        case(2):
            config |= ADS1118_CONFIG_MUX_SINGLE_2;
            break;
        case(3):
            config |= ADS1118_CONFIG_MUX_SINGLE_3;
            break;
    }
    
    writeSPI16(config, this->m_pin_CS);
    
    // Set a timeout for 16 ms, twice what is required
    // Monitor DOUT/nDRDY after pulling CS low
    // nDRDY low indicates data is ready
    int i;
    pinMode(this->m_pin_CS, OUTPUT);
    pinMode(A4, INPUT);
    for (i=16; i>=0; i--) {
        int j;
        ATOMIC_BLOCK() {
            pinResetFast(this->m_pin_CS);
            delayMicroseconds(1);       // at least 100 ns to meet t_CSDOD
            j = pinReadFast(A4);
            pinSetFast(this->m_pin_CS);
        }
        if (!j) {
            break;
        } else {
            delay(1);
        }
    }
    if (i<0) { //timeout
        return 0;
    }
    
    return_val = writeSPI16(config, this->m_pin_CS);
    delayMicroseconds(2400);
    return return_val;
}

int16_t ReclaimerLabs_ADS1118::readADC_Differential_0_1() {
    int16_t return_val;
    
    uint16_t config = ADS1118_CONFIG_SS_START_ON |  // turn on single shot mode
                      ADS1118_CONFIG_MODE_SINGLE |  // power down after sample
                      ADS1118_CONFIG_DR_860SPS   |  // 860 samples per sec (fastest)
                      ADS1118_CONFIG_TS_MODE_ADC |  // read ADC, not temp
                      ADS1118_CONFIG_PULL_UP_ON  |  // pull-ups on (default)
                      ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
                      ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
    
    config |= this->m_gain;

    config |= ADS1118_CONFIG_MUX_DIFF_0_1;
    
    writeSPI16(config, this->m_pin_CS);
    
    // Set a timeout for 16 ms, twice what is required
    // Monitor DOUT/nDRDY after pulling CS low
    // nDRDY low indicates data is ready
    int i;
    pinMode(this->m_pin_CS, OUTPUT);
    pinMode(A4, INPUT);
    for (i=16; i>=0; i--) {
        int j;
        ATOMIC_BLOCK() {
            pinResetFast(this->m_pin_CS);
            delayMicroseconds(1);       // at least 100 ns to meet t_CSDOD
            j = pinReadFast(A4);
            pinSetFast(this->m_pin_CS);
        }
        if (!j) {
            break;
        } else {
            delay(1);
        }
    }
    if (i<0) { //timeout
        return 0;
    }
    
    return_val = writeSPI16(config, this->m_pin_CS);
    delayMicroseconds(2400);
    return return_val;
}

// 14-bit read, 2's complement
// units are 0.03125 C per bit
int16_t ReclaimerLabs_ADS1118::readADC_Differential_2_3() {
    int16_t return_val;
    
    uint16_t config = ADS1118_CONFIG_SS_START_ON |  // turn on single shot mode
                      ADS1118_CONFIG_MODE_SINGLE |  // power down after sample
                      ADS1118_CONFIG_DR_128SPS   |  // 128 samples per sec (default)
                      ADS1118_CONFIG_TS_MODE_ADC |  // read ADC, not temp
                      ADS1118_CONFIG_PULL_UP_ON  |  // pull-ups on (default)
                      ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
                      ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
    
    config |= this->m_gain;

    config |= ADS1118_CONFIG_MUX_DIFF_2_3;
    
    writeSPI16(config, this->m_pin_CS);

    // Set a timeout for 16 ms, twice what is required
    // Monitor DOUT/nDRDY after pulling CS low
    // nDRDY low indicates data is ready
    int i;
    pinMode(this->m_pin_CS, OUTPUT);
    pinMode(A4, INPUT);
    for (i=16; i>=0; i--) {
        int j;
        ATOMIC_BLOCK() {
            pinResetFast(this->m_pin_CS);
            delayMicroseconds(1);       // at least 100 ns to meet t_CSDOD
            j = pinReadFast(A4);
            pinSetFast(this->m_pin_CS);
        }
        if (!j) {
            break;
        } else {
            delay(1);
        }
    }
    if (i<0) { //timeout
        return 0;
    }

    return_val = writeSPI16(config, this->m_pin_CS);
    delayMicroseconds(2400);
    return return_val;
}

int16_t ReclaimerLabs_ADS1118::readTemp_Raw() {
    int16_t return_val;
    
    uint16_t config = ADS1118_CONFIG_SS_START_ON |  // turn on single shot mode
                      ADS1118_CONFIG_MODE_SINGLE |  // power down after sample
                      ADS1118_CONFIG_DR_860SPS   |  // 860 samples per sec
                      ADS1118_CONFIG_PULL_UP_ON  |  // pull-ups on (default)
                      ADS1118_CONFIG_NOP_VALID   |  // this is valid data (default)
                      ADS1118_CONFIG_RESV;          // reserved bits must be set to 1
    
    config |= this->m_gain;
    
    config |= ADS1118_CONFIG_TS_MODE_TEMP;
    
    writeSPI16(config, this->m_pin_CS);
    
    // Set a timeout for 16 ms, twice what is required
    // Monitor DOUT/nDRDY after pulling CS low
    // nDRDY low indicates data is ready
    int i;
    pinMode(this->m_pin_CS, OUTPUT);
    pinMode(A4, INPUT);
    for (i=16; i>=0; i--) {
        int j;
        ATOMIC_BLOCK() {
            pinResetFast(this->m_pin_CS);
            delayMicroseconds(1);       // at least 100 ns to meet t_CSDOD
            j = pinReadFast(A4);
            pinSetFast(this->m_pin_CS);
        }
        if (!j) {
            break;
        } else {
            delay(1);
        }
    }
    if (i<0) { //timeout
        return 0;
    }
    
    return_val = writeSPI16(config, this->m_pin_CS);
    // Temp sensor is 14 bit, so we divide by 4
    if (return_val < 0) {
        return_val *= -1;
        return_val >>= 2;
        return_val *= -1;
    } else {
        return_val >>= 2;
    }
    
    delayMicroseconds(2400);
    return return_val;
}

float ReclaimerLabs_ADS1118::readTemp_C() {
    return (ADS1118_CONST_C_PER_BIT * this->readTemp_Raw());
}

float ReclaimerLabs_ADS1118::readADC_Diff_0_1_mV() {
    int16_t rawADC = this->readADC_Differential_0_1();
    float return_val=0;
    switch (this->m_gain) {
        case(GAIN_TWOTHIRDS): // ADS1118_CONFIG_PGA_6_144V
            return_val = (ADS1118_CONST_6_144V_LSB_mV * rawADC);
            break;
        case(GAIN_ONE):       // ADS1118_CONFIG_PGA_4_096V
            return_val = (ADS1118_CONST_4_096V_LSB_mV * rawADC);
            break;
        case(GAIN_TWO):       // ADS1118_CONFIG_PGA_2_048V
            return_val = (ADS1118_CONST_2_048V_LSB_mV * rawADC);
            break;
        case(GAIN_FOUR):      // ADS1118_CONFIG_PGA_1_024V
            return_val = (ADS1118_CONST_1_024V_LSB_mV * rawADC);
            break;
        case(GAIN_EIGHT):     // ADS1118_CONFIG_PGA_0_512V
            return_val = (ADS1118_CONST_0_512V_LSB_mV * rawADC);
            break;
        case(GAIN_SIXTEEN):   // ADS1118_CONFIG_PGA_0_256V
            return_val = (ADS1118_CONST_0_256V_LSB_mV * rawADC);
            break;
    }
    
    return return_val;
}

float ReclaimerLabs_ADS1118::readADC_Diff_2_3_mV() {
    int16_t rawADC = this->readADC_Differential_2_3();
    float return_val=0;
    switch (this->m_gain) {
        case(GAIN_TWOTHIRDS): // ADS1118_CONFIG_PGA_6_144V
            return_val = (ADS1118_CONST_6_144V_LSB_mV * rawADC);
            break;
        case(GAIN_ONE):       // ADS1118_CONFIG_PGA_4_096V
            return_val = (ADS1118_CONST_4_096V_LSB_mV * rawADC);
            break;
        case(GAIN_TWO):       // ADS1118_CONFIG_PGA_2_048V
            return_val = (ADS1118_CONST_2_048V_LSB_mV * rawADC);
            break;
        case(GAIN_FOUR):      // ADS1118_CONFIG_PGA_1_024V
            return_val = (ADS1118_CONST_1_024V_LSB_mV * rawADC);
            break;
        case(GAIN_EIGHT):     // ADS1118_CONFIG_PGA_0_512V
            return_val = (ADS1118_CONST_0_512V_LSB_mV * rawADC);
            break;
        case(GAIN_SIXTEEN):   // ADS1118_CONFIG_PGA_0_256V
            return_val = (ADS1118_CONST_0_256V_LSB_mV * rawADC);
            break;
    }
    
    return return_val;
}
