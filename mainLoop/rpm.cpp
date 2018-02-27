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

static volatile int rpm_hit_count; //Counter incremented in interrupt service routine


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
}


/*
 * Run this code at some point in the loop
 * to calculate rpm since the last loop
 * 
 */
uint16_t rpm_calculate(void) {
    static uint16_t rpm_current; //Store rpm between calls
    static unsigned long rpm_time; //Keep track of time since last calculation
 
    if (millis() - rpm_time >= TIME_THRESHOLD) {
        unsigned long tmp_time = rpm_time;
        rpm_time = millis();
        #if (NUM_RPM_SENSORS == 2)
            uint32_t rpm = (( 60000 * rpm_hit_count) / (rpm_time - tmp_time)) / 2; 
        #else
            uint32_t rpm = (( 60000 * rpm_hit_count) / (rpm_time - tmp_time)); 
        #endif
        rpm_hit_count = 0;         
        rpm_current = (uint16_t)rpm;
    } 
    return rpm_current;
}


/*
 * Function called only when rpm_interrupt occurs
 * DO NOT CALL THIS FROM ANY POINT IN THE CODE!!!
 */
void rpm_count_increment(void){
    //Serial.print("hit "); //For testing only
    rpm_hit_count++;
}
