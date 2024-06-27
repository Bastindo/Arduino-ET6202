#include "et6202.h"

static byte image[IMAGE_LEN];   // Image array

/**
 * Send a byte via SPI interface.
 * 
 * @param data: byte to send.
 */
void send_data(byte data)
{
    shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, data);
}

/**
 * Set display mode (Order 1).
 * 
 * @param mode: DisplayMode to set
 */
void set_display_mode(byte mode)
{
    digitalWrite(PIN_STB, LOW);
    send_data(ORDER_SET_DISPLAY_MODE | mode);
    digitalWrite(PIN_STB, HIGH);
}

/**
 * Set data flags (Order 2).
 * 
 * @param testmode: enable test mode
 * @param autoincrement: enable auto address increment
 * @param readmode: enable reading the key scan data
 */
void set_data_flags(bool testmode, bool autoincrement, bool readmode)
{
    digitalWrite(PIN_STB, LOW);
    send_data(ORDER_SET_DATA | (B00001000 * testmode) | (B00000100 * autoincrement) | (B00000010 * readmode));
    digitalWrite(PIN_STB, HIGH);
}

/**
 * Set address (Order 3).
 * 
 * @param address: address to set (0x00 .. 0x0D, default 0x00).
 * 
 * @hint: address will change automatically when autoincrement was set in set_data_order command.
 */
void set_address(byte address)
{
    digitalWrite(PIN_STB, LOW);
    send_data(ORDER_SET_ADDRESS | (B00001111 & address));
    digitalWrite(PIN_STB, HIGH);
}

/**
 * Set display control flags (Order 4).
 * 
 * @param enable: enable display
 * @param brightness: set dimmer brightness (0x00 .. 0x07)
 */
void set_display_control(bool enable, byte brightness)
{
    digitalWrite(PIN_STB, LOW);
    send_data(ORDER_SET_DISPLAY_CONTROL | (B00001000 * enable) | (B00001111 & brightness));
    digitalWrite(PIN_STB, HIGH);
}

/**
 * Send byte array at address.
 * 
 * @param address: address to set (0x00 .. 0x0D, default 0x00).
 * @param data: array of bytes to send.
 * @param len: length of array.
 */
void send_array_at_address(byte address, byte *data, byte len)
{
    digitalWrite(PIN_STB, LOW);
    send_data(ORDER_SET_ADDRESS | (B00001111 & address));
    for (byte i = 0; i < len; i++)
    {
        send_data(data[i]);
    }    
    digitalWrite(PIN_STB, HIGH);
}

/**
 * Refresh display of image array.
 */
void refresh_display()
{
    set_display_mode(DISPLAY_MODE);
    set_data_flags(false, false, false);
    send_array_at_address(0, image, IMAGE_LEN);
    set_display_control(true, 7);
}

/**
 * Poll key/button memory from chip.
 */
byte poll_keys()
{
    set_display_control(false, 7);
    digitalWrite(PIN_STB, LOW);
    send_data(ORDER_SET_DATA | B00000010);
    pinMode(PIN_DATA, INPUT);

    const byte keys1 = shiftIn(PIN_DATA, PIN_CLK, LSBFIRST);
    const byte keys2 = shiftIn(PIN_DATA, PIN_CLK, LSBFIRST);

    digitalWrite(PIN_STB, HIGH);
    pinMode(PIN_DATA, OUTPUT);
    set_display_control(true, 7);

    return keys1 | keys2;
}

/**
 * Select segments of a seven segment digit.
 * 
 * @param pos: position of digit
 * @param a: segment a
 * @param b: segment b
 * @param c: segment c
 * @param d: segment d
 * @param e: segment e
 * @param f: segment f
 * @param g: segment g
 */
void et6202_show_digit(byte pos, bool a, bool b, bool c, bool d, bool e, bool f, bool g)
{
    pos = 4 - pos;
    image[pos*2] = 0;

    if (a) image[pos*2] |= B00000001;
    if (b) image[pos*2] |= B00000010;
    if (c) image[pos*2] |= B00000100;
    if (d) image[pos*2] |= B00001000;
    if (e) image[pos*2] |= B00010000;
    if (f) image[pos*2] |= B00100000;
    if (g) image[pos*2] |= B01000000;
}

/**
 * Displays a raw digit value at a specific position on the ET6202 display.
 *
 * @param pos The position of the digit (0-3).
 * @param value The raw value to be displayed.
 */
void et6202_show_digit_raw(byte pos, byte value)
{
    pos = 4 - pos;
    image[pos*2] = value;
}

/**
 * Sets the state of the colon symbol at the specified position.
 *
 * @param pos The position of the colon symbol.
 * @param on  The state of the colon symbol (true for on, false for off).
 */
void et6202_show_colon(byte pos, bool on)
{
    image[pos*4+4] |= on ? B10000000 : B00000000;
}

/**
 * Print a character as a seven segment display image.
 * 
 * @param c: character to print
 * @param pos: position to display character on
 */
void et6202_print_char(char c, byte pos)
{
    et6202_show_digit_raw(pos, char_to_segments(c));
}

/**
 * Prints a string to the ET6202 device.
 *
 * This function iterates over each character in the given string and calls the
 * `et6202_print_char` function to print each character to the device.
 *
 * @param str The string to be printed.
 */
void et6202_print_string(const char* str)
{
    for (unsigned int i = 0; i < strlen(str); i++)
    {
        et6202_print_char(str[i], i);
    }
}

/**
 * Prints an integer number to the ET6202 device.
 *
 * @param number The integer number to be printed.
 */
void et6202_print_int(long number)
{
    log(("Printing int: " + String(number)).c_str());
    if (number < 0)
    {
        et6202_print_char('-', 0);
        number = -number;
    }
    else
    {
        et6202_print_char('0'+((number / 10000) % 10), 0);
    }

    et6202_print_char('0'+((number / 1000) % 10), 1);
    et6202_print_char('0'+((number / 100) % 10), 2);
    et6202_print_char('0'+((number / 10) % 10), 3);
    et6202_print_char('0'+(number % 10), 4);
}

/**
 * @brief Initializes the ET6202 module.
 * 
 * This function sets the pin modes for STB, CLK, and DATA as OUTPUT.
 * It then sets the initial values for STB, CLK, and DATA pins.
 * After a delay of 200 ms, it configures the data flags, address, display mode, and display control.
 * Finally, it logs a message indicating that the ET6202 module has been initialized.
 */
void et6202_init() 
{
    pinMode(PIN_STB, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_DATA, OUTPUT);

    digitalWrite(PIN_STB, HIGH);
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_DATA, LOW);
    
    delay(200); // Delay 200 ms according to ET6202 manual

    set_data_flags(false, false, false);
    set_address(0);
    set_display_mode(DISPLAY_MODE);
    set_display_control(true, 7);

    log("ET6202 initialized");
}

/**
 * The et6202_loop function is responsible for refreshing the display and polling for keys.
 * 
 * It calls the refresh_display function to update the display.
 * Then, it polls for keys using the poll_keys function.
 * If a key is pressed, it logs the key value and adds a delay of 200 milliseconds.
 */
void et6202_loop() 
{
    refresh_display();

    const byte key = poll_keys();
    if (key)
    {
        log(("--- Key: " + String(key) + " ---").c_str());
        delay(200);
    }
}