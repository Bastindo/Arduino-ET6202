# Arduino-ET6202
Arduino program to control a 5-digit 7-segment display connected to an ET6202 chip via a serial CLI

ET6202 is an old 7-segment display driver usually found in DVD players, set top boxes and the like. It supports a lot of different display configurations, so this project may not work with
your display. Tested with a 5-digit display marked J2005BG.

## Default pin configuration
```
#define PIN_STB 4   // Serial interface command mode port
#define PIN_DATA 3  // Data input and output port
#define PIN_CLK 2   // Clock input port
```

## How to build
This project uses PlatformIO.

Just run `pio run` and `pio run -t upload`. Or build with VSCode.

## How to use
Connect to your Arduino's serial console (baudrate is 115200) and you'll have a CLI.

```
Available commands:
  help - Displays this help message
  printnum <number> - Prints a number to the ET6202 display
  printstr <string> - Prints a string to the ET6202 display
  printchar <char> <position> - Prints a character to the ET6202 display at the specified position
  colon <pos> <on> - Turns the colon on or off at the specified position
```
