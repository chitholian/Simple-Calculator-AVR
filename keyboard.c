/**************************\
* Atikur Rahman Chitholian *
*      CSE 2015-16         *
* University of Chittagong *
\**************************/

#include <avr/io.h>
#include <util/delay.h>

#include "keyboard.h"

unsigned char keys[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}
};

void init_keyboard() {
    DDRB |= 0x0F; // PB 0-3 output for rows.
    DDRC &= 0xF0; // PC 0-3 input for columns.
}

unsigned char scan_key() {
    while (1) {
        PORTC |= 0x0F;
        for(int i = 0;i<4;i++){
            PORTB = ~(0x01 << i);
            for(int j = 0;j < 4;j++){
                if(bit_is_clear(PINC, j)){
                    while(bit_is_clear(PINC, j)) _delay_ms(50);
                    return keys[i][j];
                }
            }
        }
    }
}
