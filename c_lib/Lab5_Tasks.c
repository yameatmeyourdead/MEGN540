#include "Lab5_Tasks.h"

#include "Encoder.h"
#include "MotorPWM.h"

void Update_Controllers( float _time_since_last )
{
    // get encoder values
    uint32_t encoder_left  = Encoder_Counts_Left();
    uint32_t encoder_right = Encoder_Counts_Right();

    // update controllers L/R
    float controller_outL = Controller_Update( &Controller_L, encoder_left, _time_since_last );
    float controller_outR = Controller_Update( &Controller_R, encoder_right, _time_since_last );

    // Saturate
    float controller_outL = Saturate( controller_outL, MotorPWM_Get_Max() );
    float controller_outR = Saturate( controller_outR, MotorPWM_Get_Max() );

    // apply PWM
    MotorPWM_Set_Left( controller_outL );
    MotorPWM_Set_Right( controller_outR );
}