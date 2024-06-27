#include "cmd.h"

char buffer[256];
bool commandReceived = false;

char* trimWhitespace(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void processCommand(char* cmd) {
    cmd = trimWhitespace(cmd);
    Serial.println();

    if (strcmp(cmd, "help") == 0) {
        Serial.println("Available commands:");
        Serial.println("  help - Displays this help message");
        Serial.println("  printnum <number> - Prints a number to the ET6202 display");
        Serial.println("  printstr <string> - Prints a string to the ET6202 display");
        Serial.println("  printchar <char> <position> - Prints a character to the ET6202 display at the specified position");
        Serial.println("  colon <pos> <on> - Turns the colon on or off at the specified position");
    }
    else if (strncmp(cmd, "printnum", 8) == 0) {
        char* arg = cmd + 8;
        arg = trimWhitespace(arg);
        int number = atoi(arg);
        et6202_print_int(number);
    }
    else if (strncmp(cmd, "printstr", 8) == 0) {
        char* arg = cmd + 8;
        arg = trimWhitespace(arg);
        et6202_print_string(arg);
    }
    else if (strncmp(cmd, "printchar", 9) == 0) {
        char* arg = cmd + 9;
        arg = trimWhitespace(arg);
        char c = arg[0];
        arg = trimWhitespace(arg + 1);
        int pos = atoi(arg);
        et6202_print_char(c, pos);
    }
    else if (strncmp(cmd, "colon", 5) == 0) {
        char* arg = cmd + 5;
        arg = trimWhitespace(arg);
        int pos = atoi(arg);
        arg = trimWhitespace(arg + 1);
        bool on = atoi(arg);
        et6202_show_colon(pos, on);
    }
    else if (strcmp(cmd, "") == 0) {
        // Do nothing
    }
    else {
        Serial.println("Unknown command: " + String(cmd));
    }
}

void cmdLoop() {
    static unsigned int idx = 0; // Index for reading characters into the buffer

    if (Serial.available() > 0) {
        char ch = Serial.read();

        if (ch == 8 || ch == 127) { // ASCII values for backspace
            if (idx > 0) {
                idx--;
                Serial.print("\b \b"); // Move cursor back, overwrite with space, move back again
            }
        } else if (ch!= '\n' && ch!= '\r') {
                Serial.print(ch);
        }

        if (ch == '\n' || ch == '\r') {
            buffer[idx] = '\0';
            commandReceived = true;
            return;
        } else {
            // Only store printable ANSI characters
            if (ch >= 32 && ch <= 254) {
                buffer[idx++] = ch;
            }
        }
    }

    if (commandReceived) {
        processCommand(buffer);
        idx = 0;
        commandReceived = false;
        Serial.print("> ");
    }
}

void setupCmd() {
    Serial.println("Type 'help' for a list of commands");
    Serial.print("> ");
}