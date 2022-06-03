This firmware demonstrates some features of the board:

The 8 LEDs connected to GPIO ports are animated using PWM, via a timer interrupt running at 10Khz
The realtime clock is set and prints to the serial terminal (115200bps 8n1)
The on-board i2C temperature and humidty sensor is polled for the current readings, which are also printed to serial
If either of the two user buttons are pressed, they will also appear in the serial output (updates at 1hz)

This example was put together using STM32Cube IDE version 1.8.0 on Windows.

You will probably need to adjust the output directory by right clocking on the project, then properties, then "Debug/Run settings". Then click on the launch config, and edit.
