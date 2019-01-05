/**************************\
* Atikur Rahman Chitholian *
*      CSE 2015-16         *
* University of Chittagong *
\**************************/

#include <stdlib.h>
#include <util/delay.h>
#include "calculator.h"
#include "keyboard.h"
#include "lcd.h"

long double r, a, b; // a and b are operands, r is the result.
int count; // Number of characters displaying on first line.
unsigned short state; // Calculator and Monitor state.
unsigned char op; // Operator: [/, *, +, -].
static char line[] = "                "; // Single line, 16 characters long.
static char buffer[16];
static char overflow[] = "   Overflow   ";

void decide(unsigned char);
void send_digit(unsigned char);

void init_calculator() {
    _delay_ms(500);
    init_display();
    init_keyboard();

    reset();
    run();
}

double calculate(long double m, char operator, long double n) {
    switch (operator) {
    case '+':
        return r = m + n;
    case '-':
        return r = m - n;
    case '*':
        return r = m * n;
    case '/':
        return r = m / n;
    }
    return r = m;
}

void show_result() {
    move_to(1, 0);
    send_string("A=");
    char *data = buffer;
    if (r <= 999999999999)
        dtostrf(r, -14, 12, buffer);
    else data = overflow;
    send_string(data);
    state = 4;
}
void send_digit(unsigned char digit) {
    send_data(digit + '0');
}
void run() {
    while (1) {
        decide(scan_key());
    }
}

void reset() {
    clear();
    send_string("CSE 612: Group 7");
    move_to(1, 0);
    send_string("   Calculator");
    r = a = b = op = state = count = 0;
}

/* States:
0 - Displaying banner.
1 - Input first operand, a.
2 - Middle Operator pressed, op.
3 - Input second operand, b.
4 - Result displayed.
*/
void decide(unsigned char key) {
    // Check if it is a digit.
    if (key >= '0' && key <= '9')
    {
        unsigned char digit = key - '0'; // '2' --> 2
        switch (state) {
        case 0:
            if (digit) {
                clear(); // Clear screen if input is non-zero.
                state = 1;
            }
        case 1:
            if (count == 14) return; // You cannot fillup the screen with a single operand.
            a = a * 10 + digit; // append to a.
            if (a > 0) {
                send_data(key);
                count++;
            }
            break;
        case 2:
            if (digit) state = 3;
        case 3:
            b = b * 10 + digit; // append to b.
            if (b > 0) {
                send_data(key);
                count++;
            }
            break;
        case 4:
            if (digit) {
                line[0] = ' ';
                line[1] = ' ';
                move_to(0, 0);
                send_string(line); // Clear 1st line.
                move_to(0, 0);
                a = b = op = count = 0;
                state = 1;
                decide(key); // Recursively capture digit.
                return;
            }
            break;
        }
    } else {
        switch (key) {
        case '/':
        case '*':
        case '+':
        case '-':
            switch (state) {
            case 2:
                move_to(0, --count); // Modify the operand displayed.
            case 1:
                send_data(key);
                count++;
                state = 2;
                break;
            case 3:
                calculate(a, op, b);
                show_result();
            case 4:
                move_to(0, 0);
                line[0] = 'A'; // 'A' represents current result.
                line[1] = key; // Operator of the operation.
                send_string(line);
                move_to(0, 2);
                count = 2;
                a = r; // Put result into a.
                b = 0; // Clear b.
                state = 2;
                break;
            }
            op = key;
            break;
        case '=':
            if(state == 3){
                calculate(a, op, b);
                show_result();
            }
            break;
        case 'C': // "ON/C" button.
            if (state != 0) reset();
            break;
        }
    }
}
