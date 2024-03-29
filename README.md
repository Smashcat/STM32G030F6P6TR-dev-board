# STM32G030F6P6TR development board
Small breadboard friendly development board for the 32bit STM32G030F6P6TR microcontroller (currently stocked and priced at just over $1 on JLC!) with the following available features:

- 32Kb Flash
- 8Kb RAM.
- 12 bit ADC (16 channels)
- 2 x USART
- 2 x i2C
- i2S
- 2 x SPI
- 17 x GPIOs
- 5 DMA channels for above peripherals, plus memory to memory transfers
- Realtime clock with calendar and alarm with interrupt generation
- 8 x timers
- 2 x watchdogs

Note that many of the peripherals share pins.

This board includes 8 USER LEDs, 2 user buttons, UART to USB bridge, SHT30-DIS-B temperature and humidity sensor and all pins broken out for development use. Pullups on i2C pins. On-board 3.3V regulator from USB supply, with header pins for 3.3V and 5V available for use.

The V1.1 version also includes a LSM6DS3TR IMU device on the i2C bus. Other than this, it is identical.

All files required for full PCBA production at JLCPCB are included in the "Production files" folder.

Example firmware added to show use of i2C sensor, plus LEDs and internal clock.

V1.0
![Render](./V1.0/Images/render.png?raw=true)

V1.1 (with IMU)
![Render](./V1.1/Images/render.png?raw=true)
