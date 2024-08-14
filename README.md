# NHD-0420H1Z
Newhaven LCD NHD-0420H1Z driver library

Based on code from Embedded Systems Essentials with Arm:
Get Practical with Hardware
LAB 1
Introductory Serial Communication

# Hardware setup
At the moment the library is configured to use the default SPI ports (D11, D12, D13) to send 4-bit data to a 74HC595N shift register which is then connected with 4 wires to the NHD-0420H1Z LCD.

# // TODO
Update the readme and turn the library into a class so I can hide functions that should be private.
