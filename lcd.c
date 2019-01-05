/**************************\
* Atikur Rahman Chitholian *
*      CSE 2015-16         *
* University of Chittagong *
\**************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

void trigger(void);

void init_display(){
    DDRC |= 0x30; // PC 4 is output for RS, 5 is output for Enable.
    DDRD = 0xFF; // PD 0-7 is 8 bit data output bus.

    send_command(0x01); // Clear all.
    send_command(0x0C); // Hide cursor.
    send_command(0x38); // 2 line mode.
    send_command(0x06); // Cursor auto increment.
    send_command(0x80); // Move to home.
}

void send_command(unsigned char command){
    PORTC &=~(0x10); // Reset PC4.
    PORTD = command;
    trigger();
}
void send_data(unsigned char data){
    PORTC |=0x10; // Set PC4.
    PORTD = data;
    trigger();
}

void trigger(){
    PORTC |= 0x20; // Set PC5.
    _delay_ms(15); // Delay 20 milliseconds.
    PORTC &=~(0x20); // Reset PC5.
}

void send_string(char *str){
    while(*str) send_data(*str++); // Send characters one by one.
}

void move_to(unsigned i, unsigned j){
    unsigned char line = i ? 0xC0 : 0x80;
    send_command(line + j);
}

void clear(){
    send_command(0x01);
}
