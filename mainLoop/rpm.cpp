/* This file contains functions that will:
 * 1) Calculate RPM at any time 
 * 2) Setup the pin interrupts to allow said
 *    calculations to occur
 */

#include "Arduino.h"
#include "common.h"
#include "pins.h"



#define TIME_THRESHOLD 5000 //Threshold time (milliseconds)
#define NUM_RPM_SENSORS 2 //Using 1 or 2 rpm sensors

//Variables from mainLoop
extern dataStruct data;
extern uint16_t counter;


/*
 * Run this code at some point in the
 * setup method to initialize RPM sensor
 * interrupts
 */
 
void rpm_sensor_setup(void) {
    attachInterrupt(digitalPinToInterrupt(PIN_RPM_INTERRUPT_1), rpm_count_increment, RISING);
    #if (NUM_RPM_SENSORS == 2)
        attachInterrupt(digitalPinToInterrupt(PIN_RPM_INTERRUPT_2), rpm_count_increment, RISING);
    #endif
} //Should refactor this to seperate functions for each sensor



/*
 * Function called only when rpm_interrupt occurs
 * DO NOT CALL THIS FROM ANY POINT IN THE CODE!!!
 */
void rpm_count_increment(void){
    
    static unsigned long lastTime = 0;
    unsigned long thisTime = millis();
    unsigned long period = thisTime - lastTime;

    #if (NUM_RPM_SENSORS == 2)
        data.rpm = (30000 / period);
    #else
        data.rpm = (60000 / period);
    #endif
    
    lastTime = thisTime;
    counter = 0;
}
