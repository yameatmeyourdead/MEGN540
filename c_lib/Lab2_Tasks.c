#include "Lab2_Tasks.h"

void Send_Time_Now( float _time_since_last )
{
    float time_s = Timing_Get_Time_Sec();
    USB_Send_Msg( "cf", 't', &time_s, sizeof( time_s ) );
}

void Send_Loop_Time( float _time_since_last )
{
    if( task_time_loop.run_period > 0 )
        _time_since_last -= task_time_loop.run_period;
    USB_Send_Msg( "cf", 'T', &_time_since_last, sizeof( _time_since_last ) );
}
