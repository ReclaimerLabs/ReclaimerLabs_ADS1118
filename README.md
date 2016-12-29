Copyright (c) 2015 Jason Cerundolo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Reclaimer Labs ADS1118
==================

The ADS1118 is a small, low power, 16-bit ADC with a built-in temperature 
sensor. It is ideally suited for measuring thermocouples with cold junction 
compensation. 

Usually, you will want to use the readADC_Diff_0_1_mV() and 
readADC_Diff_2_3_mV() functions with the readTemp_C() function. This provides 
everything you need to accurately measure temperature with a thermocouple. 

The default gain is a full scale voltage of 256 mV. This is a good range for 
thermocouples. You can change it with the setGain() function. The options are 
    GAIN_TWOTHIRDS  for full scale of 6.144 V
    GAIN_ONE        for full scale of 4.096 V
    GAIN_TWO        for full scale of 2.048 V
    GAIN_FOUR       for full scale of 1.024 V
    GAIN_EIGHT      for full scale of 0.512 V
    GAIN_SIXTEEN    for full scale of 0.256 V

Also check out the Reclaimer Labs Thermocouple library. 
