#ifndef LCD_H
#define LCD_H
void init_display(void);
void send_command(unsigned char);
void send_data(unsigned char);
void send_string(char*);
void clear(void);
void move_to(unsigned, unsigned);
#endif
