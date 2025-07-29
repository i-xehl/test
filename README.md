# SST26VF064B Flash Test driver for STM32

This repository provides a driver for the SST26VF064B SPI NOR flash memory, intended for use with STM32 microcontrollers.

## Features

- Chip, sector, and block erase
- Page write and read verification
- JEDEC ID and Unique ID read
- All results printed using SEGGER RTT

## Requirements

- STM32 development board (any with SPI support)
- SST26VF064B flash chip connected to SPI
- SEGGER RTT for log output (J-Link, ST-Link, etc.)
- STM32CubeMX/HAL or compatible SPI HAL

## Usage

1. **Clone this repository.**
2. Wire up the SST26VF064B to your STM32 board's SPI port.
3. Build and flash the firmware to your STM32.
4. Run the test by calling `flash2click_test_all_extended()` in your `main()` function.
5. Open SEGGER RTT Viewer or a compatible terminal to view results.

## Notes

- The `.ioc` file (STM32CubeMX project) should be kept under version control.
- Build output and user-specific IDE files are excluded by `.gitignore`.
- See comments in the test code for each test step and expected behavior.
- **Most of the driver functions are not yet tested.**
