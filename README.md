# Custom ATmega328P Watch

## System Overview

This project is a custom-designed watch built around the ATmega328P-MU microcontroller and RV-3028 real-time clock. The design integrates timekeeping, display, battery charging, audio output, user input, and USB programming onto a single PCB.

The watch is designed as a self-contained embedded system with all peripherals integrated directly onto the board.

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
* USB/ISP used for programming and serial debugging
* GPIO used for user input, speaker drive, and RTC interrupt handling

## PCB Design

The PCB was designed from scratch in EasyEDA Pro.

* Custom schematic and PCB layout
* Four-layer PCB (2 routing and planezone layers)
* Dedicated ground planezone
* Dedicated power planezone
* Compact watch-sized layout
* Exposed programming and debugging pads

This project includes complete schematic capture, footprint selection, PCB layout, and manufacturing preparation.

## Firmware Behaviour

The firmware is written for the ATmega328P and focuses on low-power operation while maintaining responsive user interaction.

### Timekeeping

* RV-3028 provides primary timekeeping
* Time is read over I2C
* RTC alarms can wake the microcontroller from sleep from the RTC's INT pin to the ATMega328P's pin2

### Power Management

* ATmega328P sleep modes used during inactivity
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

* USB charging support
* Battery voltage monitoring
* Low-power sleep operation
* RTC-backed timekeeping during sleep

## PCB Source Files

Included files:

* EasyEDA Pro project source
* Gerber manufacturing files
* Bill of Materials (BOM)
* Pick-and-place files
* Schematic exports

## Engineering Challenges

Key challenges addressed during development:

* Compact component placement beneath the OLED display
* Integration of USB programming, charging, RTC, display, and user input on a single PCB
* Low-power operation using RTC-assisted sleep modes
* Battery voltage measurement without a dedicated regulated reference
* Four-layer PCB design and routing

## Future Improvements

* Additional watch faces
* Stopwatch and timer functions
* Expanded alarm functionality
* Optional microSD support
* Further power consumption optimisation

## Notes

This project represents a complete custom watch platform built around the ATmega328P. Both the hardware and firmware are developed specifically for this design, allowing full control over every aspect of the system.
