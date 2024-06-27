#include <Arduino.h>

#include "cmd.h"
#include "et6202.h"
#include "log.h"

void setup() {
    Serial.begin(115200);

    setupLog();
    log("Starting up...");
    et6202_init();
    setupCmd();

    et6202_print_int(1234);
}

void loop() {
    cmdLoop();
    et6202_loop();
}