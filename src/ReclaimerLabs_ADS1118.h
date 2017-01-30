#ifndef RECLAIMERLABS_ADS1118_H
#define RECLAIMERLABS_ADS1118_H

#include <application.h>

#define ADS1118_CONFIG_SS_START_MASK 0x8000
#define ADS1118_CONFIG_SS_START_ON   0x8000
#define ADS1118_CONFIG_SS_START_OFF  0x0000  // default

#define ADS1118_CONFIG_MUX_MASK      0x7000
#define ADS1118_CONFIG_MUX_DIFF_0_1  0x0000  // default
#define ADS1118_CONFIG_MUX_DIFF_0_3  0x1000
#define ADS1118_CONFIG_MUX_DIFF_1_3  0x2000
#define ADS1118_CONFIG_MUX_DIFF_2_3  0x3000
#define ADS1118_CONFIG_MUX_SINGLE_0  0x4000
#define ADS1118_CONFIG_MUX_SINGLE_1  0x5000
#define ADS1118_CONFIG_MUX_SINGLE_2  0x6000
#define ADS1118_CONFIG_MUX_SINGLE_3  0x7000

#define ADS1118_CONFIG_PGA_MASK      0x0E00
#define ADS1118_CONFIG_PGA_6_144V    0x0000
#define ADS1118_CONFIG_PGA_4_096V    0x0200
#define ADS1118_CONFIG_PGA_2_048V    0x0400  // default
#define ADS1118_CONFIG_PGA_1_024V    0x0600
#define ADS1118_CONFIG_PGA_0_512V    0x0800
#define ADS1118_CONFIG_PGA_0_256V    0x0A00

#define ADS1118_CONFIG_MODE_MASK     0x0100
#define ADS1118_CONFIG_MODE_CONTIN   0x0000
#define ADS1118_CONFIG_MODE_SINGLE   0x0100

#define ADS1118_CONFIG_DR_MASK       0x00E0
#define ADS1118_CONFIG_DR_8SPS       0x0000
#define ADS1118_CONFIG_DR_16SPS      0x0020
#define ADS1118_CONFIG_DR_32SPS      0x0040
#define ADS1118_CONFIG_DR_64SPS      0x0060
#define ADS1118_CONFIG_DR_128SPS     0x0080  // default
#define ADS1118_CONFIG_DR_250SPS     0x00A0
#define ADS1118_CONFIG_DR_475SPS     0x00C0
#define ADS1118_CONFIG_DR_860SPS     0x00E0

#define ADS1118_CONFIG_TS_MODE_MASK  0x0010
#define ADS1118_CONFIG_TS_MODE_ADC   0x0000  // default
#define ADS1118_CONFIG_TS_MODE_TEMP  0x0010

#define ADS1118_CONFIG_PULL_UP_MASK  0x0008
#define ADS1118_CONFIG_PULL_UP_ON    0x0008  // default
#define ADS1118_CONFIG_PULL_UP_OFF   0x0000

#define ADS1118_CONFIG_NOP_MASK      0x0006
#define ADS1118_CONFIG_NOP_VALID     0x0002  // default
#define ADS1118_CONFIG_NOP_INVALID   0x0006

#define ADS1118_CONFIG_RESV          0x0001  // default, required

#define ADS1118_CONST_C_PER_BIT      (0.03125)
#define ADS1118_CONST_BIT_PER_C      32

#define ADS1118_CONST_6_144V_LSB_mV  (0.1875)
#define ADS1118_CONST_4_096V_LSB_mV  (0.125)
#define ADS1118_CONST_2_048V_LSB_mV  (0.0625)
#define ADS1118_CONST_1_024V_LSB_mV  (0.03125)
#define ADS1118_CONST_0_512V_LSB_mV  (0.015625)
#define ADS1118_CONST_0_256V_LSB_mV  (0.0078125)

typedef enum
{
  GAIN_TWOTHIRDS    = ADS1118_CONFIG_PGA_6_144V,
  GAIN_ONE          = ADS1118_CONFIG_PGA_4_096V,
  GAIN_TWO          = ADS1118_CONFIG_PGA_2_048V,
  GAIN_FOUR         = ADS1118_CONFIG_PGA_1_024V,
  GAIN_EIGHT        = ADS1118_CONFIG_PGA_0_512V,
  GAIN_SIXTEEN      = ADS1118_CONFIG_PGA_0_256V
} adsGain_t;

class ReclaimerLabs_ADS1118 {
    public:
        ReclaimerLabs_ADS1118(uint16_t pin_CS);
        void begin(void);
        uint16_t  readADC_SingleEnded(uint8_t channel);
        int16_t   readADC_Differential_0_1(void);
        int16_t   readADC_Differential_2_3(void);
        float     readADC_Diff_0_1_mV();
        float     readADC_Diff_2_3_mV();
        int16_t   readTemp_Raw(void);
        float     readTemp_C(void);
        void      setGain(adsGain_t gain);
        adsGain_t getGain(void);
    
    private:
        uint16_t  m_pin_CS;
        adsGain_t m_gain;
};

#endif /* RECLAIMERLABS_ADS1118_H */
