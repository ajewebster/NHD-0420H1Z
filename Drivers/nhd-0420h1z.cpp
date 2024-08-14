#include "nhd-0420h1z.h"

#define ENABLE 0x08         // ORed in to data value to strobe E bit
#define COMMAND_MODE 0x00   //clears LCD RS line, for command transfer
#define DATA_MODE 0x04      //sets LCD RS line to 1, for data transfer

DigitalOut* Chip_Sel;         //SPI Chip Select
SPI* Ser_Port; //Initialise SPI, using default settings

//----------------- LCD functions---------------------//
void init_lcd(DigitalOut* chip_sel, SPI* serial_port){      //follow designated procedure in data sheet
    Chip_Sel = chip_sel;
    Ser_Port = serial_port;
    thread_sleep_for (40);
    shift_out(0x30);    //function set 8-bit
    wait_us(37);
    write_cmd(0x20);    //function set
    wait_us(37);
    write_cmd(0x20);    //function set
    wait_us(37);
    write_cmd(0x0C);    //display ON/OFF
    wait_us(37);
    write_cmd(0x01);    //display clear
    wait_us(1520);
    write_cmd(0x06);    //entry-mode set
    wait_us(37);
    write_cmd(0x28);    //function set
    wait_us(37);
}
void write_4bit(int data, int mode){ //mode is RS line, cmd=0, data=1
    int hi_n;
    int lo_n;
    hi_n = (data & 0xF0);  //form the two 4-bit nibbles that will be sent
    lo_n = ((data << 4) &0xF0);
//send each word twice, strobing the Enable line
    shift_out(hi_n | ENABLE | mode); 
    wait_us(1);
    shift_out(hi_n & ~ENABLE);
    shift_out(lo_n | ENABLE | mode);
    wait_us(1);
    shift_out(lo_n & ~ENABLE);
}
void shift_out(int data){        //Sends word to SPI port
    *Chip_Sel = 0;
    Ser_Port->write(data);
    *Chip_Sel = 1; 
}
void write_cmd(int cmd){         //Configures LCD command word
    write_4bit(cmd, COMMAND_MODE);
}
void write_data(char c){        //Configures LCD data word
    write_4bit(c, DATA_MODE);    //1 for data mode
}
void clr_lcd(void){      //Clears display and waits required time
    write_cmd(0x01);    //display clear
    wait_us(1520);
}
void print_lcd(const char *string){  //Sends character string to LCD
    while(*string) {
        write_data(*string++);
        wait_us(40);
    }
}
