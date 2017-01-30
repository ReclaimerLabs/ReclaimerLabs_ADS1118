# Introduction

The [**ADS1118**](http://www.ti.com/product/ADS1118) is a small, low power, 16-bit ADC with a built-in temperature sensor. It is ideally suited for measuring thermocouples with cold junction compensation. 

Also check out the [**Reclaimer Labs Thermocouple library**](https://github.com/ReclaimerLabs/ReclaimerLabs_Thermocouple) and the [**Remote Thermal Monitor board**](https://github.com/ReclaimerLabs/Remote-Thermal-Monitor). 

# Example Usage

Usually, you will want to use the readADC_Diff_0_1_mV() and readADC_Diff_2_3_mV() functions with the readTemp_C() function. This provides everything you need to accurately measure temperature with a thermocouple. 

```
const uint16_t pin_CS0 = A2;
ReclaimerLabs_ADS1118 therm01(pin_CS0);

void setup() {
    // set chip select pins high
    pinMode(pin_CS0, OUTPUT);
    digitalWrite(pin_CS0, HIGH);
    
    therm01.setGain(GAIN_SIXTEEN);
}

void loop() {
	float temp_mV, tempRef_C, temp_C;
	int32 error_code;
	temp_mV = therm01.readADC_Diff_0_1_mV();
    error_code = ReclaimerLabs_Thermocouple::calc_temp(TYPE_K, temp_mV, tempRef_C, &temp_C);
}
```
# Other functions

### uint16_t readADC_SingleEnded(uint8_t channel);

Takes in a channel number (0 to 3) and returns a 16-bit ADC reading referencing GND and VDD as full-scale. 

### int16_t readADC_Differential_0_1(void);

Returns the raw 16-bit ADC value of the differential voltage of channel 0 minus 1. 

### int16_t readADC_Differential_2_3(void);

Returns the raw 16-bit ADC value of the differential voltage of channel 2 minus 3. 

### int16_t readTemp_Raw(void);

Returns the raw 14-bit ADC value of the on-chip temperature sensor. Negative numbers use twos complement. One 14-bit LSB equals 0.03125 degrees C. 

### void setGain(adsGain_t gain);

setGain() and getGain() use the enum adsGain_t. 
* GAIN_TWOTHIRDS, full-scale = 6.144 V
* GAIN_ONE, full-scale = 4.096 V
* GAIN_TWO, full-scale = 2.048 V
* GAIN_FOUR, full-scale = 1.024 V
* GAIN_EIGHT, full-scale = 0.512 V
* GAIN_SIXTEEN, full-scale = 0.256 V

# Questions and Comments

If you have questions or comments, you can email me directly at jason@reclaimerlabs.com. 
