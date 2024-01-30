#include "Lab1_Tasks.h"

// This function takes two values, multiplies them, and sends the result over serial back to the Host
void Multiply_And_Send( float value_left, float value_right )
{
    // Do the thing you need to do. Here we want to multiply
    float ret_val = value_left * value_right;

    // send response right here if appropriate.
    USB_Send_Msg( "cf", '*', &ret_val, sizeof( ret_val ) );
}

// This function takes two values, divides them, and sends the result over serial back to the Host
void Divide_And_Send( float value_left, float value_right )
{
    float ret_val = value_left / value_right;
    USB_Send_Msg( "cf", '/', &ret_val, sizeof( ret_val ) );
}

// This function takes two values, adds them, and sends the result over serial back to the Host
void Add_And_Send( float value_left, float value_right )
{
    float ret_val = value_left + value_right;
    USB_Send_Msg( "cf", '+', &ret_val, sizeof( ret_val ) );
}

// This function takes two values, subtracts them, and sends the result over serial back to the Host
void Subtract_And_Send( float value_left, float value_right )
{
    float ret_val = value_left - value_right;
    USB_Send_Msg( "cf", '-', &ret_val, sizeof( ret_val ) );
}