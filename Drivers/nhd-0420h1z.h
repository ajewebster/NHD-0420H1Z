/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NHD-0420H1Z_H
#define __NHD-0420H1Z_H

#ifdef __cplusplus
 extern "C" {
#endif

// Exported functions
void init_lcd(void);
void clr_lcd(void);
void print_lcd(const char *string);
void shift_out(int data);
void write_cmd(int cmd);
void write_data(char c);
void write_4bit(int data);
void write_8bit(int data);
