#ifndef ET6202_H
#define ET6202_H

#include <Arduino.h>
#include <stdio.h>

#include "et6202_chars.h"
#include "log.h"

#define DISPLAY_MODE 3

#define ORDER_SET_DISPLAY_MODE 0
#define ORDER_SET_DATA 64
#define ORDER_SET_ADDRESS 192
#define ORDER_SET_DISPLAY_CONTROL 128

#define IMAGE_LEN 14    // Length of image array

#define PIN_STB 4   // Serial interface command mode port
#define PIN_DATA 3  // Data input and output port
#define PIN_CLK 2   // Clock input port

void et6202_show_digit(byte pos, bool a, bool b, bool c, bool d, bool e, bool f, bool g);
void et6202_show_digit_raw(byte pos, byte value);
void et6202_show_colon(byte pos, bool on);

void et6202_print_char(char c, byte pos);
void et6202_print_string(const char* str);
void et6202_print_int(long number);

void et6202_init();
void et6202_loop();

#endif