# NHD-0420H1Z
Newhaven LCD NHD-0420H1Z driver library

Based on code from Embedded Systems Essentials with Arm:
Get Practical with Hardware
LAB 1
Introductory Serial Communication

This library is designed to be used with MBED OS.

# Usage
This library can be used in 3 ways.

1.  Using the default SPI ports (D11, D12, D13) to send 4-bit data to a 74HC595N shift register which is then connected with 4 wires to the NHD-0420H1Z LCD.

2.  Using a BusOut object to address the data and control lines. You can either use a 7-bit or 11-bit BusOut object to call the LCDDriver. Either way you should start with the used data lines from low to high and then define the R/W, RS & E lines.

# Known issues
When using the library with the 4-bit BusOut the display doesn't always seems to want to reset properly. Multiple resets can be needed.

# // TODO
Update the readme
