#include "Lab3_Tasks.h"

void Send_Encoder_Message( char message_type )
{
    struct __attribute__( ( __packed__ ) ) {
        float left_count;
        float right_count;
    } data;

    data.left_count  = (float)Encoder_Counts_Left();
    data.right_count = (float)Encoder_Counts_Right();

    USB_Send_Msg( "cff", message_type, &data, sizeof( data ) );
}

void Send_Encoder_Counts( float _time_since_last )
{
    Send_Encoder_Message( 'E' );
}

void Send_Battery_Message( char message_type )
{
    float battery = Filter_Last_Output( &bat_filt );
    USB_Send_Msg( "cf", message_type, &battery, sizeof( battery ) );
}

void Update_Battery_Filter( float _time_since_last )
{
    Battery_Voltage();
}

void Send_Battery_Level( float _time_since_last )
{
    Send_Battery_Message( 'B' );
}

void Check_Battery_Level( float _time_since_last )
{
    float bat_volt = Filter_Last_Output( &bat_filt );
    struct __attribute__( ( __packed__ ) ) {
        char let[7];
        float volt;
    } msg = { .let = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .volt = bat_volt };

    // Send Warning to Serial that batteries need to be charged
    if( bat_volt < 3.6 ) {
        USB_Send_Msg( "c7sf", '!', &msg, sizeof( msg ) );
    }
}
