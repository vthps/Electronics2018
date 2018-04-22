#include "Arduino.h"
#include "common.h"
#include "pins.h"


#define NUM_RPM_SENSORS 2 //Using 1 or 2 rpm sensors


static void rpm_interrupt_a(void); 
#if (NUM_RPM_SENSORS == 2)
static void rpm_interrupt_b(void); 
#endif


static volatile unsigned long rpmA;
static volatile unsigned long rpmB;
static volatile uint8_t counter; //If this gets too high set rpm to 0

/*
 * Run this code at some point in the
 * setup method to initialize RPM sensor
 * interrupts
 */
void rpm_sensor_setup(void) {
    attachInterrupt(digitalPinToInterrupt(PIN_RPM_INTERRUPT_1), rpm_interrupt_a, RISING);
    #if (NUM_RPM_SENSORS == 2)
        attachInterrupt(digitalPinToInterrupt(PIN_RPM_INTERRUPT_2), rpm_interrupt_b, RISING);
    #endif
} 

//Run this function in loop to get current rpm
uint16_t rpm_calculate() {

    if (counter >= 3) {
        return 0;    
    }
    
    #if (NUM_RPM_SENSORS == 2)
        return (rpmA + rpmB) / 2; //Return average
    #else
        return rpmA;
    #endif

    counter++;    
}




/*
 * interrupt handlers called only when rpm_interrupt occurs
 * DO NOT CALL THESE FROM ANY POINT IN THE CODE!!!
 */
static void rpm_interrupt_a(void) {
    static unsigned long lastTime = 0;
    unsigned long thisTime = millis();
    unsigned long period = thisTime - lastTime;
    rpmA = (60000 / period);    
    lastTime = thisTime;
    counter = 0;
}


#if (NUM_RPM_SENSORS == 2)
static void rpm_interrupt_b(void) {
    static unsigned long lastTime = 0;
    unsigned long thisTime = millis();
    unsigned long period = thisTime - lastTime;
    rpmB = (60000 / period);
    lastTime = thisTime;
    counter = 0;
}
#endif


