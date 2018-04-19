#include "common.h"

dataStruct data;
volatile uint16_t counter = 0; //Keep track of how many loops without a pulse from rpm sensors

void setup() {
    Serial.begin(57600);
    rpm_sensor_setup();
    datalogger_init();
    accel_init();
}

void loop() {
    data.velocity = get_velocity();
    data.depth = depth_function();
    getGyroPos(&(data.accelPos));
    logData(&data);
    delay(1000);
    counter++;
    
    if (counter >= 3) {
        data.rpm = 0;
        counter = 0;
    }
}
