/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2023.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2023 Andrew Petruska at Colorado School of Mines

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

#include "Filter.h"
#include <stdio.h>

/**
 * Function Filter_Init initializes the filter given two float arrays and the order of the filter.  Note that the
 * size of the array will be one larger than the order. (First order systems have two coefficients).
 *
 *  1/A_0*( SUM( B_i * input_i )  -   SUM( A_i * output_i) )
 *         i=0..N                    i=1..N
 *
 *  Note a 5-point moving average filter has coefficients:
 *      numerator_coeffs   = { 5 0 0 0 0 };
 *      denominator_coeffs = { 1 1 1 1 1 };
 *      order = 4;
 *
 * @param p_filt pointer to the filter object
 * @param numerator_coeffs The numerator coefficients (B/beta traditionally)
 * @param denominator_coeffs The denominator coefficients (A/alpha traditionally)
 * @param order The filter order
 */
void Filter_Init( Filter_Data_t* p_filt, float* numerator_coeffs, float* denominator_coeffs, uint8_t order )
{   
    // initialize ring buffers
    rb_initialize_F(&p_filt->numerator);
    rb_initialize_F(&p_filt->denominator);
    rb_initialize_F(&p_filt->out_list);
    rb_initialize_F(&p_filt->in_list);

    // fill the numerator and denominator ring buffers
    for(uint8_t i=0; i<=order; i++) {
        rb_push_back_F(&p_filt->numerator, numerator_coeffs[i]);
    }
    
    for(uint8_t i=0; i<=order; i++) {
        rb_push_back_F(&p_filt->denominator, denominator_coeffs[i]);
    }

    // zero the initial in and out lists
    for(uint8_t i=0; i<=order; i++) {
        rb_push_back_F(&p_filt->in_list, 0.0f);
    }
    
    for(uint8_t i=0; i<=order; i++) {
        rb_push_back_F(&p_filt->out_list, 0.0f);
    }

    return;
}

/**
 * Function Filter_ShiftBy shifts the input list and output list to keep the filter in the same frame. This especially
 * useful when initializing the filter to the current value or handling wrapping/overflow issues.
 * @param p_filt
 * @param shift_amount
 */
void Filter_ShiftBy( Filter_Data_t* p_filt, float shift_amount )
{
    // add shift amount to each element of input and output ring buffers
    for(uint8_t i=0; i<rb_length_F(&p_filt->in_list); i++) {
        rb_set_F(&p_filt->in_list, i, shift_amount + rb_get_F(&p_filt->in_list, i));
    }
    
    for(uint8_t i=0; i<=rb_length_F(&p_filt->out_list); i++) {
        rb_set_F(&p_filt->out_list, i, shift_amount + rb_get_F(&p_filt->out_list, i));
    }
    return;
}

/**
 * Function Filter_SetTo sets the initial values for the input and output lists to a constant defined value. This
 * helps to initialize or re-initialize the filter as desired.
 * @param p_filt Pointer to a Filter_Data sturcture
 * @param amount The value to re-initialize the filter to.
 */
void Filter_SetTo( Filter_Data_t* p_filt, float amount )
{
    rb_initialize_F(&p_filt->in_list);
    rb_initialize_F(&p_filt->out_list);
    uint8_t order = rb_length_F(&p_filt->numerator) -1;
    for(uint8_t i=0; i<=order; i++) {
        rb_push_back_F(&p_filt->in_list, amount);
    }
    
    for(uint8_t i=0; i<=order; i++) {
        rb_push_back_F(&p_filt->out_list, amount);
    }
    return;
}

/**
 * Function Filter_Value adds a new value to the filter and returns the new output.
 * @param p_filt pointer to the filter object
 * @param value the new measurement or value
 * @return The newly filtered value
 */
float Filter_Value( Filter_Data_t* p_filt, float value )
{
    // add new measurement, discard old
    rb_push_back_F(&p_filt->in_list, value);
    rb_pop_front_F(&p_filt->in_list);
    // intermediate calculation storage
    double sumBs = 0.0;
    double sumAs = 0.0;
    // SUM( B_i * input_i )
    // i=0..N
    for(uint8_t i=0; i<rb_length_F(&p_filt->numerator); i++) {
        sumBs += rb_get_F(&p_filt->numerator, i) * rb_get_F(&p_filt->in_list, i);
    }
    // SUM( B_i * output_i )
    // i=1..N
    for(uint8_t i=1; i<rb_length_F(&p_filt->denominator); i++) {
        sumAs += rb_get_F(&p_filt->denominator, i) * rb_get_F(&p_filt->out_list, i-1);
        
    }
    // add to output ring buffer, discard old output
    rb_push_front_F(&p_filt->out_list, 1.0/rb_get_F(&p_filt->denominator, 0) * (sumBs-sumAs));
    rb_pop_back_F(&p_filt->out_list);
    return rb_get_F(&p_filt->out_list, 0);
}

/**
 * Function Filter_Last_Output returns the most up-to-date filtered value without updating the filter.
 * @return The latest filtered value
 */
float Filter_Last_Output( Filter_Data_t* p_filt )
{
    // we already did the calculation, output the value
    return rb_get_F(&p_filt->out_list, 0);
}
