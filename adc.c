#include <avr/io.h>


inline void ADC_waitToComplete()
{
    // wait for conversion to complete
    while (ADCSRA & (1<<ADSC)) {}
}

inline void ADC_doSingleConversion()
{
    // do a single conversion
    ADCSRA |= (1<<ADSC);
}

uint16_t ADC_getResult()
{
// Bit 5 – ADLAR: ADC Left Adjust Result (page 290)
// The ADLAR bit affects the presentation of the ADC conversion result in the ADC Data Register.
// Write one to ADLAR to left adjust the result. Otherwise, the result is right adjusted. Changing the
// ADLAR bit will affect the ADC Data Register immediately, regardless of any ongoing conversions.

// after ADCL is read, the ADC Data Register is not updated until ADCH is read
    uint16_t result;
    if (ADMUX & ADLAR)
    {
        // result is left adjusted
        result = ADCL >> 6;
        result |= ADCH << 2;
    }
    else
    {
        // result is right adjusted
        result = ADCL;
        result |= ADCH << 8;
    }
    return result;

}

void ADC_Init(void)
{
    // choose supply voltage AVcc as refernce, see atmega2560 spec, page 289:
    // ADMUX = (1<<REFS0);

    // or choose internal voltage reference 2.56V as reference for the ADC
    ADMUX = (1<<REFS1) | (1<<REFS0);

    // Bit ADFR ("free running") in ADCSRA is already zero --> single conversion

    // ADC Prescaler, see page 293
    ADCSRA = (1<<ADPS1) | (1<<ADPS0);

    // activate ADC
    ADCSRA |= (1<<ADEN);

    // after activation do a "Dummy-Readout", read the value and discard it to "warm up" the ADC

    // do an ADC conversion
    ADC_doSingleConversion();
    ADC_waitToComplete();

    // ADC result must be read once, else result for next conversion wont be applied
    (void) ADC_getResult();
}

/* @brief ADC single measurement
 *
 * performs a single measurement on the specified channel for single ended input
 *
 * @param[in] channel the Analog Channel to measure from, as described on page 290; valid values: see Table 26-4; common values: 0,1,2,...,7
 *
 * @return the result of the conversion
 */
uint16_t ADC_Read(const uint8_t channel)
{
    // select the  input source, without influencing other bits
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);

    ADC_doSingleConversion();
    ADC_waitToComplete();

    return ADC_getResult();
}

/* ADC Multiple measurements with averaging */
uint16_t ADC_Read_Avg(const uint8_t channel, const uint8_t nsamples)
{
// maximum value to be hold in sum is ((2^10)-1)*255 = 260865
// 255: max of nsamples
// 2^10-1: max val returned by ADC
    uint32_t sum = 0;
    uint8_t i;
    for (i = 0; i < nsamples; ++i)
    {
        sum += ADC_Read(channel);
    }

    return (uint16_t)(sum / nsamples);
}


/* Beispielaufrufe:
int main( void )
{
  uint16_t adcval;
  ADC_Init();

  while( 1 )
  {
    adcval = ADC_Read(0);  // Kanal 0
    // mach was mit adcval

    adcval = ADC_Read_Avg(2, 4);  // Kanal 2, Mittelwert aus 4 Messungen
    // mach was mit adcval
  }
}
*/
