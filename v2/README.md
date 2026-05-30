Arduino Watch with On-Board BMS

Revision 2

Hardware:

* ATmega328P-MU
* CH340C USB-UART
* SPI OLED
* RV-3028-C7 RTC
* LiPo battery
* Battery voltage measurement on ADC6
* 5 buttons
* Sleep mode support

Current Goals:

* RTC wake-up
* Button wake-up
* Low power operation
* Single-sided PCB layout

Known Risks:

* First PCB revision
* Sleep and interrupt firmware not yet tested
* RTC integration not yet tested on hardware

Changes from Revision 1

Added:

1. RV-3028-C7 ultra-low-power RTC

   * Datasheet: https://www.microcrystal.com/fileadmin/Media/Products/RTC/Datasheet/RV-3028-C7.pdf

Removed:

1. MT3608 boost converter
2. Coin vibration motor

Changed:

1. Switch SW1 moved from ATmega328P ADC0/PC0 (pin 23) to PB0 (pin 12, Arduino D8).
2. ATmega328P PD2/INT0 (pin 4) connected to RV-3028-C7 INT output for RTC wake-up interrupts.
3. ATmega328P PC4/SDA and PC5/SCL dedicated to RV-3028-C7 I²C communication.
4. ADC0, ADC1, ADC2, ADC3, ADC5, and ADC7 reserved as spare test points / expansion pins and should be exposed where practical.

Design Notes:

* System now runs directly from the LiPo battery.
* RTC will be used for accurate timekeeping and wake-up events.
* Deep sleep operation is planned using RTC interrupts and button wake-up.
* OLED brightness will be controlled in software.
* SPI bus may be shared with future peripherals such as a microSD card.
