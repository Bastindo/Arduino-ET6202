#include "log.h"

void setupLog() {
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.println("Log setup");
}

/**
 * @brief Prints a log message to the serial monitor.
 * 
 * This function prints a log message to the serial monitor, along with the current timestamp.
 * 
 * @param msg The message to be logged.
 */
void log(const char *msg) {
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.println(msg);
}