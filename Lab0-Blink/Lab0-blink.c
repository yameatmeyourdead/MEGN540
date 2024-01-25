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

#ifndef F_CPU
#    define F_CPU 16000000
#endif

#include "led_interface.h"

/**
 * Function blink_string takes a c-string and blinks the morse code corresponding to the characters provided.
 * @param str pointer to a null-germinated c-string object
 * @param dot_length_ms The characteristic length of a dot in the morse-code pattern.
 */
void blink_morse_str( char* str, uint16_t dot_length_ms )
{
    // iterate through str until null terminator or index == 255
    char character;
    int i = 0;
    while( ( ( character = str[i] ) != 0 ) && i < 255 ) {
        // blink the morse code corresponding to the character at index i
        blink_morse_char( character, dot_length_ms );
        // increment index
        i++;
    }
}

int main()
{
    initialize_LED();
    const uint16_t dot_length_ms = 100;

    while( 1 ) {
        // Blink "hello world "
        blink_morse_str( "Hello World ", dot_length_ms );
    }
}
