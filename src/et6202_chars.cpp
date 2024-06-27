#include "et6202_chars.h"

byte char_to_segments(char c) 
{
    c = toupper(c);

    switch (c) {
        case '0': return B00111111;
        case '1': return B00000110;
        case '2': return B01011011;
        case '3': return B01001111;
        case '4': return B01100110;
        case '5': return B01101101;
        case '6': return B01111101;
        case '7': return B00000111;
        case '8': return B01111111;
        case '9': return B01101111;

        case 'A': return B01110111;
        case 'B': return B01111100;
        case 'C': return B00111001;
        case 'D': return B01011110;
        case 'E': return B01111001;
        case 'F': return B01110001;
        case 'G': return B00111101;
        case 'H': return B01110110;
        case 'I': return B00000110;
        case 'J': return B00011110;
        case 'K': return B01110110;
        case 'L': return B00111000;
        case 'M': return B01010101;
        case 'N': return B01010100;
        case 'O': return B01011100;
        case 'P': return B01110011;
        case 'Q': return B01100111;
        case 'R': return B01010000;
        case 'S': return B01101101;
        case 'T': return B01111000;
        case 'U': return B00111110;
        case 'V': return B00011100;
        case 'W': return B01111110;
        case 'X': return B01110110;
        case 'Y': return B01101110;
        case 'Z': return B01011011;
        
        case ' ': return B00000000;
        case '-': return B01000000;
        case '_': return B00001000;
        case '=': return B01001000;
        case '*': return B01100011;
        case '/': return B01010010;
        case '\\': return B01100100;
        case '|': return B00110000;
        case '^': return B00100011;
        case '"': return B00100001;

        default: return B00000000;
    }
}