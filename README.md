# Custom ATmega328P Watch
<img width="1352" height="1288" alt="image" src="https://github.com/user-attachments/assets/01a85437-5e72-4bb8-9071-c2a6e92456a6" />

## System Overview

This project is a custom-designed watch built around the ATmega328P-MU microcontroller and RV-3028 real-time clock. The design integrates timekeeping, display, battery charging, audio output, user input, and USB programming onto a single PCB.

The watch is designed as a self-contained embedded system with all peripherals integrated directly onto the board.

<img width="818" height="1210" alt="image" src="https://github.com/user-attachments/assets/e635294f-1364-45de-92b6-c6ff8c7088fc" />

## Hardware Architecture

The PCB integrates the following subsystems:

* Microcontroller: ATmega328P-MU (8-bit AVR architecture)
* Display: 0.96" 128×64 SPI OLED
* RTC: RV-3028 ultra-low-power real-time clock
* Power system: Rechargeable LiPo battery with on-board charging circuit (TP4056)
* USB interface: CH340C USB-to-UART bridge for programming and debugging
* Audio output: On-board speaker for alarms and notifications
* User input: 5-way navigation switch

### System Topology

* SPI bus used for OLED communication
* I2C bus used for RTC communication
* USB-UART interface used for programming and serial debugging
* ISP pads exposed for direct programming and fuse configuration
* GPIO used for user input, speaker drive, and RTC interrupt handling

## PCB Design

The PCB was designed from scratch in EasyEDA Pro.

* Custom schematic and PCB layout
* Four-layer PCB (2 routing and plane layers)
* Dedicated ground plane
* Dedicated power plan
* Compact watch-sized layout
* Exposed programming and debugging pads

This project includes complete schematic capture, footprint selection, PCB layout, and manufacturing preparation.

## Firmware Behaviour

The firmware is written for the ATmega328P and focuses on low-power operation while maintaining responsive user interaction.

### Timekeeping

* RV-3028 provides primary timekeeping
* Time is read over I2C
* RTC alarms can wake the microcontroller from sleep from the RTC's INT pin to the ATMega328P's pin 2

### Power Management

* ATmega328P sleep modes used during inactivity (between screen updates, waking, etc.)
* Wake-up via RTC alarm or user input
* RTC maintains accurate time independently of MCU operation

### Display System

* SPI OLED display
* Menu-driven user interface
* Software-adjustable display brightness

### Audio Output

* Speaker driven using GPIO and 32KHz PWM (pin3)
* Used for alarms, notifications, and feedback tones

## Battery and Power System

The watch operates directly from a rechargeable LiPo battery.

Functional features include:

* USB charging support via TP4056
* Battery voltage monitoring via ADC6
* Low-power sleep operation

## PCB Source Files

Included files:

* EasyEDA Pro project source
* Gerber manufacturing files
* Bill of Materials (BOM)
* Pick-and-place files
* Schematic exports

## Goals (DONE)
* Compact component placement beneath the OLED display
* Integration of USB programming, using correct components, charging, RTC, display, and user input on a single side of the PCB for cheaper PCBA
* Low-power operation using RTC-assisted sleep modes
* Battery voltage measurement without a dedicated regulated reference
* Four-layer PCB design and routing


<img width="768" height="1194" alt="Screenshot 2026-06-22 202952" src="https://github.com/user-attachments/assets/b1ebbcbe-72b6-4d61-ab15-f06a325539d0" />


## Future Improvements

* Optional microSD support
* Further power consumption optimisation

## Notes

This project represents a complete custom watch platform built around the ATmega328P. Both the hardware and firmware (bare minimum time with sleep has been done) are developed specifically for this design, allowing full control over every aspect of the system.
