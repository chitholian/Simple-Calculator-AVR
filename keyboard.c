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
    DDRC &= ~(0x0F); // PC 0-3 input for columns.
}

unsigned char scan_key() {
    while (1) {
        int row = -1, col = -1;
        PORTB &= 0xF0;
        PORTC |= 0x0F;
        while ((PINC & 0x0F) == 0x0F) _delay_ms(20);
        for (int i = 0; i < 4; i++) {
            if (((0x01 << i) & (PINC & 0x0F)) == 0x00) {
                col = i;
                break;
            }
        }
        for (int i = 0; i < 4; i++) {
            PORTB = ~(0x01 << i);
            if ((PINC & 0x0F) != 0x0F) {
                row = i;
                break;
            }
        }
        _delay_ms(250);
        return (row != -1 && col != -1) ? keys[row][col] : 'U';
    }
}
