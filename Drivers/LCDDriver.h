#ifndef LCDDRIVER_H
#define LCDDRIVER_H
#endif

#include "mbed.h"

namespace nhd0420h1z {

#define ENABLE 0x08 // ORed in to data value to strobe E bit
#define ENABLE_BUS_4                                                           \
  0x0040 // ORed in to data value to strobe E bit with BusInOut
#define ENABLE_BUS_8                                                           \
  0x0400 // ORed in to data value to strobe E bit with BusInOut
#define COMMAND_MODE 0x0000 // clears LCD RS line, for command transfer
#define COMMAND_MODE_BUS_4                                                     \
  0x0000 // clears LCD RS line, for command transfer with BusInOut
#define COMMAND_MODE_BUS_8                                                     \
  0x0000 // clears LCD RS line, for command transfer with BusInOut
#define DATA_MODE 0x0004 // sets LCD RS line to 1, for data transfer
#define DATA_MODE_BUS_4                                                        \
  0x0020 // sets LCD RS line to 1, for data transfer with BusInOut
#define DATA_MODE_BUS_8                                                        \
  0x0200 // sets LCD RS line to 1, for data transfer with BusInOut
#define READ_MODE_4 0x0010  // sets R/W to read
#define READ_MODE_8 0x0100  // sets R/W to read
#define WRITE_MODE_4 0x0000 // sets R/W to write
#define WRITE_MODE_8 0x0000 // sets R/W to write

class LCDDriver {

public:
  /** Create an LCDDriver for use with the shift register
   *
   *    @param chip_select DigitalOut object used as CS line
   *    @param serial_port SPI object to communicate with the shift register
   */
  LCDDriver(DigitalOut *chip_select, SPI *serial_port)
      : m_CS{chip_select}, m_serialPort{serial_port} {}

  /** Create an LCDDriver with a BusInOut
   *
   *    @param busInOut BusInOut object consisting of 4 or 8 data and RW/RS/E
   * lines
   */
  LCDDriver(BusOut *busOut) : m_busOut{busOut} {
    m_useBus = 1;
    if (m_busOut->mask() == 0x007F) {
      m_dataLines = 4;
    } else if (m_busOut->mask() == 0x07FF) {
      m_dataLines = 8;
    } else {
      printf("Invalid numer of datalines");
    }
  }

  void init() {
    if (!m_useBus) {
      init_4();
    } else if (m_dataLines == 4) {
      init_4_bus();
    } else if (m_dataLines == 8) {
      init_8_bus();
    }
  }

  void write_cmd(int cmd) { // Configures LCD command word
    if (!m_useBus) {
      write_4bit(cmd, COMMAND_MODE);
    } else {
      if (m_dataLines == 4) {
        write_4bit_bus(cmd, COMMAND_MODE_BUS_4);
      } else if (m_dataLines == 8) {
        write_8bit_bus(cmd, COMMAND_MODE_BUS_8);
      }
    }
  }
  void write_data(char c) { // Configures LCD data word
    if (!m_useBus) {
      write_4bit(c, DATA_MODE);
    } else {
      if (m_dataLines == 4) {
        write_4bit_bus(c, DATA_MODE_BUS_4);
      } else if (m_dataLines == 8) {
        write_8bit_bus(c, DATA_MODE_BUS_8);
      }
    }
  }
  void clr_lcd(void) { // Clears display and waits required time
    write_cmd(0x01);   // display clear
    wait_us(1520);
  }
  void print_lcd(const char *string) { // Sends character string to LCD
    while (*string) {
      write_data(*string++);
      wait_us(40);
    }
  }

private:
  DigitalOut *m_CS;
  SPI *m_serialPort;
  BusOut *m_busOut;
  uint8_t m_dataLines;
  bool m_useBus;

  void init_4() { // follow designated procedure in data sheet
    // bus_out(0x00);
    thread_sleep_for(100);
    write_cmd(0x20); // command 0x20 = Wake up
    wait_us(30);
    write_cmd(0x20); // command 0x20 = Wake up
    wait_us(10);
    write_cmd(0x20); // command 0x20 = Wake up
    wait_us(10);
    write_cmd(0x20); // function set
    wait_us(10);
    write_cmd(0x28); // function set
    wait_us(37);
    write_cmd(0x10); // function set
    wait_us(37);
    write_cmd(0x0F); // display ON/OFF
    wait_us(37);
    // write_cmd(0x01); // display clear
    // wait_us(1520);
    write_cmd(0x06); // entry-mode set
    wait_us(37);
  }
  void init_4_bus() { // follow designated procedure in data sheet
    // bus_out(0x00);
    thread_sleep_for(100);
    write_cmd(0x20); // command 0x20 = Wake up
    wait_us(30);
    write_cmd(0x20); // command 0x20 = Wake up
    wait_us(10);
    write_cmd(0x20); // command 0x20 = Wake up
    wait_us(10);
    write_cmd(0x20); // function set
    wait_us(10);
    write_cmd(0x28); // function set
    wait_us(37);
    write_cmd(0x10); // function set
    wait_us(37);
    write_cmd(0x0F); // display ON/OFF
    wait_us(37);
    // write_cmd(0x01); // display clear
    // wait_us(1520);
    write_cmd(0x06); // entry-mode set
    wait_us(37);
  }
  void init_8_bus(void) {
    bus_out(0x0000);
    thread_sleep_for(100);
    write_cmd(0x0030); // command 0x30 = Wake up
    wait_us(100);
    write_cmd(0x0030); // command 0x30 = Wake up
    wait_us(37);
    write_cmd(0x0030); // command 0x30 = Wake up
    wait_us(37);
    write_cmd(0x0038); // function set: 8-bit/2-line
    wait_us(37);
    write_cmd(0x0010); // Set cursor
    wait_us(37);
    write_cmd(0x000c); // display ON; cursor ON
    wait_us(37);
    // write_cmd(0x01); // display clear
    // wait_us(1520);
    write_cmd(0x0006); // entry-mode set
    wait_us(37);
  }

  void write_4bit(int data, int mode) { // mode is RS line, cmd=0, data=1
    int hi_n;
    int lo_n;
    hi_n = (data & 0xF0); // form the two 4-bit nibbles that will be sent
    lo_n = ((data << 4) & 0xF0);
    // send each word twice, strobing the Enable line
    shift_out(hi_n | ENABLE | mode);
    wait_us(1);
    shift_out(hi_n & ~ENABLE);
    shift_out(lo_n | ENABLE | mode);
    wait_us(1);
    shift_out(lo_n & ~ENABLE);
  }
  void write_4bit_bus(int data, int mode) { // mode is RS line, cmd=0, data=1
    printf("data: %4x, mode: %4x\r\n", data, mode);
    int hi_n;
    int lo_n;
    hi_n = ((data >> 4) & 0x0F); // form the two 4-bit nibbles that will be sent
    // printf("hi_n: %4x\r\n", hi_n);
    lo_n = (data & 0x0F);
    // printf("lo_n: %X\r\n", lo_n);
    //  send each word twice, strobing the Enable line
    bus_out(hi_n | ENABLE_BUS_4 | WRITE_MODE_4 | mode);
    wait_us(1);
    bus_out(hi_n & ~ENABLE_BUS_4 | WRITE_MODE_4 | mode);
    wait_us(1);
    bus_out(lo_n | ENABLE_BUS_4 | WRITE_MODE_4 | mode);
    wait_us(1);
    bus_out(lo_n & ~ENABLE_BUS_4 | WRITE_MODE_4 | mode);
  }
  void write_8bit_bus(int data, int mode) {
    bus_out(data | ENABLE_BUS_8 | WRITE_MODE_4 | mode);
    wait_us(1);
    bus_out(data & ~ENABLE_BUS_8 | WRITE_MODE_4 | mode);
  }
  void shift_out(int data) { // Sends word to SPI port
    *m_CS = 0;
    m_serialPort->write(data);
    *m_CS = 1;
  }
  void bus_out(int data) {
    // printf("Data is: %4X\r\n", data);
    m_busOut->write(data);
  }
};
} // namespace nhd0420h1z
