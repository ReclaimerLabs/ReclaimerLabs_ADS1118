// This #include statement was automatically added by the Particle IDE.
#include "ReclaimerLabs_ADS1118/ReclaimerLabs_ADS1118.h"

/*
 * Be sure to add the following libraries to your project
 *          ReclaimerLabs_ADS1118 (this one, should be added by default)
 *          ReclaimerLabs_Thermocouple
 *          SparkFunPhant
 */

const uint16_t pin_CS0 = A2;
const uint16_t pin_CS1 = D2;
ReclaimerLabs_ADS1118 therm01(pin_CS0);
ReclaimerLabs_ADS1118 therm23(pin_CS1);
char publishString[64];
volatile uint32_t counter;

/*
 * you need to fill out your own public and private key from SparkFun
 * Go to data.sparkfun.com to create your own stream and get keys
 * Create a stream with the following fields:
 *      tempRef_C
 *      temp0_C
 *      temp1_C
 *      temp2_C
 *      temp3_C
 */
const char server[] = "data.sparkfun.com"; // Phant destination server
const char publicKey[] = ""; // Phant public key
const char privateKey[] = ""; // Phant private key
Phant phant(server, publicKey, privateKey); // Create a Phant object

void readTemps();

void setup() {
    // use the onboard LED to indicate activity
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);
    
    // set chip select pins high
    pinMode(pin_CS0, OUTPUT);
    pinMode(pin_CS1, OUTPUT);
    digitalWrite(pin_CS0, HIGH);
    digitalWrite(pin_CS1, HIGH);
    delay(10);
    
    therm01.setGain(GAIN_SIXTEEN);
    therm23.setGain(GAIN_SIXTEEN);
    counter = 0;
    delay(6000);
}

void loop() {
    if (counter == 0) {
        readTemps();
        counter = 300; // ~ every five minutes
    } else {
        counter--;
    }
    
    delay(1000);
}

void readTemps() {
    int32_t error_code;
    float temp_mV, temp_C, tempRef_C;
    
    digitalWrite(D7, HIGH);
    
    // calculate reference temperature as average of both ADCs
    tempRef_C  = therm01.readTemp_C();
    tempRef_C += therm23.readTemp_C();
    tempRef_C  = tempRef_C / 2;
    sprintf(publishString, "%4.2f", tempRef_C);
    phant.add("tempref", tempRef_C);
    Particle.publish("tempRef_C", publishString);
    delay(500); // so as to not overwhelm the Particle cloud
    
    temp_mV = therm01.readADC_Diff_0_1_mV();
    error_code = ReclaimerLabs_Thermocouple::calc_temp(TYPE_K, temp_mV, tempRef_C, &temp_C);
    if (error_code == 0) {
        sprintf(publishString, "%4.2f", temp_C);
        phant.add("temp0", temp_C);
    } else {
        sprintf(publishString, "%ld", 1000*error_code);
        phant.add("temp0", 1000.0*error_code);
    }
    Particle.publish("temp0_C", publishString);
    delay(500); // so as to not overwhelm the Particle cloud
    
    temp_mV = therm01.readADC_Diff_2_3_mV();
    error_code = ReclaimerLabs_Thermocouple::calc_temp(TYPE_K, temp_mV, tempRef_C, &temp_C);
    if (error_code == 0) {
        sprintf(publishString, "%4.2f", temp_C);
        phant.add("temp1", temp_C);
    } else {
        sprintf(publishString, "%ld", 1000*error_code);
        phant.add("temp1", 1000.0*error_code);
    }
    Particle.publish("temp1_C", publishString);
    delay(500); // so as to not overwhelm the Particle cloud
    
    temp_mV = therm23.readADC_Diff_0_1_mV();
    error_code = ReclaimerLabs_Thermocouple::calc_temp(TYPE_K, temp_mV, tempRef_C, &temp_C);
    if (error_code == 0) {
        sprintf(publishString, "%4.2f", temp_C);
        phant.add("temp2", temp_C);
    } else {
        sprintf(publishString, "%ld", 1000*error_code);
        phant.add("temp2", 1000.0*error_code);
    }
    Particle.publish("temp2_C", publishString);
    delay(500); // so as to not overwhelm the Particle cloud
    
    temp_mV = therm23.readADC_Diff_2_3_mV();
    error_code = ReclaimerLabs_Thermocouple::calc_temp(TYPE_K, temp_mV, tempRef_C, &temp_C);
    if (error_code == 0) {
        sprintf(publishString, "%4.2f", temp_C);
        phant.add("temp3", temp_C);
    } else {
        sprintf(publishString, "%ld", 1000*error_code);
        phant.add("temp3", 1000.0*error_code);
    }
    Particle.publish("temp3_C", publishString);
    delay(500); // so as to not overwhelm the Particle cloud
    
    phant.particlePost();
    
    digitalWrite(D7, LOW);
    
    delay(10);
}
