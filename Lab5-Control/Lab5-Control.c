/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

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

*/

#include "Battery_Monitor.h"
#include "Controller.h"
#include "Encoder.h"
#include "Message_Handling.h"
#include "MotorPWM.c"
#include "SerialIO.h"
#include "Task_Management.h"
#include "Timing.h"

// put your initialization function here
void Initialize_Modules( float _time_not_used_ )
{
    // Initialize (reinitialize) all global variables

    // reset USB input buffers
    USB_Flush_Input_Buffer();

    // Initialize all modules except USB (it can only be called once without messing things up)
    Initialize_Timing( 0.0 );
    Initialize_Encoders();
    Initialize_Battery_Monitor();
    Initialize_MotorPWM( 400 );
    Initialize_Controller( &Controller_L, 1, numL, denL, sizeof( numL ) / sizeof( float ) - 1, 0.001f );  // left controller
    Initialize_Controller( &Controller_R, 1, numR, denR, sizeof( numR ) / sizeof( float ) - 1, 0.001f );  // right controller

    // Setup task handling
    Initialize_Task( &task_restart, &Initialize_Modules /*function pointer to call*/ );

    // Setup message handling to get processed at some desired rate.
    Initialize_Task( &task_message_handling, &Task_Message_Handling );

    // Lab 2 tasks
    Initialize_Task( &task_time_loop, &Send_Loop_Time );
    Initialize_Task( &task_send_time, &Send_Time_Now );
    Initialize_Task( &task_message_handling_watchdog, Task_Message_Handling_Watchdog );

    Task_Activate( &task_message_handling_watchdog, 0.25f );
    Task_Activate( &task_message_handling, 0.f );

    // Lab 3 tasks
    Initialize_Task( &task_send_battery_level, &Send_Battery_Level );
    Initialize_Task( &task_update_battery_filter, &Update_Battery_Filter );
    Initialize_Task( &task_send_encoder_counts, &Send_Encoder_Counts );
    Initialize_Task( &task_battery_level_watchdog, &Check_Battery_Level );
    Task_Activate( &task_update_battery_filter, 0.002f );
    Task_Activate( &task_battery_level_watchdog, 1 );

    // Lab 4 tasks
    Initialize_Task( &task_sii, &Send_SII_Message_Repeat );
    Initialize_Task( &task_time_pwm, &StopPWM );

    // Lab 5 tasks
    Initialize_Task( &task_update_controllers, &Update_Controllers );
}

int main()
{
    Initialize_Modules( 0.0 );
    // call initialization stuff

    Initialize_USB();
    Task_Activate( &task_message_handling_watchdog, 0.25f );
    Task_Activate( &task_message_handling, 0.f );

    while( true ) {
        Task_USB_Upkeep();

        Task_Run_If_Ready( &task_time_loop );
        Task_Run_If_Ready( &task_send_time );
        Task_Run_If_Ready( &task_message_handling );
        Task_Run_If_Ready( &task_restart );
        Task_Run_If_Ready( &task_send_encoder_counts );
        Task_Run_If_Ready( &task_update_battery_filter );
        Task_Run_If_Ready( &task_send_battery_level );
        Task_Run_If_Ready( &task_battery_level_watchdog );
        Task_Run_If_Ready( &task_sii );
        Task_Run_If_Ready( &task_update_controllers );
        Task_Run_If_Ready( &task_time_pwm );

        Task_Run_If_Ready( &task_message_handling_watchdog );
    }

    return 0;
}
