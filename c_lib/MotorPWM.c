#include "MotorPWM.h"

void Initialize_MotorPWM( uint16_t MAX_PWM )
{
    // Timer1 configuration. Set to PWM mode.
    ICR1 = MAX_PWM;                                   // Set TOP to 400.
    TCCR1A |= ( ( 1 << COM1A1 ) | ( 1 << COM1B1 ) );  // Set to: PWM, Phase and Frequency Correct, clear on OC1A, OC1B match.
    TCCR1B |= ( 1 << WGM13 ) | ( 1 << CS10 );         // Set to: PWM, Phase and Frequency Correct, No prescaling

    // Set port pins as outputs
    DDRB |= ( 1 << DDB2 ) | ( 1 << DDB1 ) | ( 1 << DDB6 ) | ( 1 << DDB5 );

    // Set initial speeds to zero.
    OCR1A = 0;
    OCR1B = 0;
}

void MotorPWM_Enable( bool enable )
{
    if( enable ) {
        TCCR1B |= ( 1 << CS10 );
    } else {
        TCCR1B &= ~( 1 << CS10 );
    }
}

bool MotorPWM_Is_Enabled()
{
    return TCCR1A & ( 1 << COM1A1 ) && TCCR1A & ( 1 << COM1B1 );
}

void MotorPWM_Set_Left( int16_t pwm )
{
    // Left motor PWM on port B pin 6 -> PB6
    OCR1B = 0;
    // Left motor directionality on port B pin 2 -> PB2
    PORTB &= ~( 1 << PB2 );
    PORTB |= ( (bool)( pwm & 0x8000 ) << PB2 );
    // Set new value.
    if( pwm & 0x8000 ) {
        OCR1B = (uint16_t)( ~pwm + 1 );
    } else {
        OCR1B = pwm;
    }
}

void MotorPWM_Set_Right( int16_t pwm )
{
    // Right motor PWM on port B pin 5 -> PB5
    OCR1A = 0;
    // Right motor directionality on port B pin 1 -> PB1
    PORTB &= ~( 1 << PB1 );
    PORTB |= ( (bool)( pwm & 0x8000 ) << PB1 );
    if( pwm & 0x8000 ) {
        OCR1A = (uint16_t)( ~pwm + 1 );
    } else {
        OCR1A = pwm;
    }
}

int16_t MotorPWM_Get_Left()
{
    return OCR1B;
}

int16_t MotorPWM_Get_Right()
{
    return OCR1A;
}

uint16_t MotorPWM_Get_Max()
{
    return ICR1;
}

void MotorPWM_Set_Max( uint16_t MAX_PWM )
{
    ICR1 = MAX_PWM;
}
