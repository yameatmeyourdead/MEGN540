#include "Battery_Monitor.h"

#include "SerialIO.h"

static const float BITS_TO_BATTERY_VOLTS = 0.004882813f;
float numerator[]                        = { 4.37268879781997e-06, 1.74907551912799e-05, 2.62361327869198e-05, 1.74907551912799e-05, 4.37268879781997e-06 };
float denominator[]                      = { 1, -3.75376275667119, 5.29115258416348, -3.31893860475052, 0.781618740279003 };

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{
    // setup 128 prescalar
    ADCSRA |= ( 1 << ADPS0 );
    ADCSRA |= ( 1 << ADPS1 );
    ADCSRA |= ( 1 << ADPS2 );

    ADMUX |= ( 1 << REFS0 );
    ADMUX |= ( 1 << REFS1 );

    // enable ADC
    ADCSRA |= ( 1 << ADEN );

    // initialize filter
    Filter_Init( &filt, numerator, denominator, 4 );
    Filter_SetTo( &filt, 5.0f );
}

/**
 * Function Battery_Voltage initiates the A/D measurement and returns the result for the battery voltage.
 */
float Battery_Voltage()
{
    // wait for ADC to be free
    while( ADCSRA & ( 1 << ADSC ) ) {};

    // set ADC to read from 6
    ADMUX &= ~( 1 << MUX5 );
    ADMUX &= ~( 1 << MUX4 );
    ADMUX &= ~( 1 << MUX3 );
    ADMUX |= ( 1 << MUX2 );
    ADMUX |= ( 1 << MUX1 );
    ADMUX &= ~( 1 << MUX0 );

    // start conversion
    ADCSRA |= ( 1 << ADSC );

    // wait for ADC to complete
    while( ADCSRA & ( 1 << ADSC ) ) {};

    // A Union to assist with reading the LSB and MSB in the  16 bit register
    union {
        struct {
            uint8_t LSB;
            uint8_t MSB;
        } split;
        uint16_t value;
    } data = { .value = 0 };

    data.split.LSB = ADCL;
    data.split.MSB = ADCH;

    float bat_volt_measured = data.value * BITS_TO_BATTERY_VOLTS;

    return Filter_Value( &filt, bat_volt_measured );
}
