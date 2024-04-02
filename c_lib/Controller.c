#include "Controller.h"

// TODO: FIND
#define CPR         909.7f           // Counts / Rev
#define D           38.5f            // MM
#define W           84.2f            // MM
#define COUNT_PER_M 7521.207882114f  // Counts / meter (CPR / D / W * 1000)

void Initialize_Controller( Controller_t* p_cont, float kp, float* num, float* den, uint8_t order, float update_period )
{
    Filter_Init( &p_cont->controller, num, den, order );
    p_cont->kp            = kp;
    p_cont->update_period = update_period;
}

void Controller_Set_Target_Velocity( Controller_t* p_cont, float vel )
{
    p_cont->target_vel = vel;
}

void Controller_Set_Target_Position( Controller_t* p_cont, float pos )
{
    p_cont->target_pos = pos;
    p_cont->target_vel = 0;
}

float Controller_Update( Controller_t* p_cont, float measurement, float dt )
{
    static float measurement_last = 0;
    float to_return               = 0;

    // is it a velocity controller?
    if( p_cont->target_vel != 0 ) {
        // controller's input is target velocity - measured velocity
        p_cont->target_pos += p_cont->target_vel * p_cont->update_period;
        to_return =
            p_cont->kp * ( p_cont->target_pos - Filter_Value( &p_cont->controller, measurement / COUNT_PER_M + p_cont->target_vel * p_cont->update_period ) );
    } else {
        // controller's input is target position - measured position
        to_return = p_cont->kp * ( p_cont->target_pos - Filter_Value( &p_cont->controller, measurement / COUNT_PER_M ) );
    }

    measurement_last = measurement;
    return to_return;
}

float Controller_Last( Controller_t* p_cont )
{
    return Filter_Last_Output( &p_cont->controller );
}

void Controller_SetTo( Controller_t* p_cont, float measurement )
{
    Filter_SetTo( &p_cont->controller, measurement );
}

void Controller_ShiftBy( Controller_t* p_cont, float measurement )
{
    Filter_ShiftBy( &p_cont->controller, measurement );
}
