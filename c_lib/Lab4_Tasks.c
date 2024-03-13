#include "Lab4_Tasks.h"

#include "Lab3_Tasks.h"

void StopPWMAndDisable()
{
    MotorPWM_Set_Left( 0 );
    MotorPWM_Set_Right( 0 );
    MotorPWM_Enable( false );
}

void StopPWM( float _time_since_last )
{
    MotorPWM_Set_Left( 0 );
    MotorPWM_Set_Right( 0 );
    Task_Cancel( &task_time_pwm );
}

void Send_SII_Message( char message_type )
{
    struct __attribute__( ( __packed__ ) ) {
        float time;
        int16_t pwm_l;
        int16_t pwm_r;
        int16_t enc_l;
        int16_t enc_r;
    } data;

    data.time  = Timing_Get_Time_Sec();
    data.pwm_l = MotorPWM_Get_Left();
    data.pwm_r = MotorPWM_Get_Right();
    data.enc_l = Encoder_Counts_Left();
    data.enc_r = Encoder_Counts_Right();

    USB_Send_Msg( "cf4h", message_type, &data, sizeof( data ) );
}

void Send_SII_Message_Repeat( float _time_since_last )
{
    Send_SII_Message( 'Q' );
}