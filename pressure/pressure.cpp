#include "Arduino.h"
#include "common.h"
#include "pins.h"

double get_velocity(void) {
    // this function can be called for in the race code and calculates velocity
    // the ftvelocity equation can be removed for the race code as it is for testing on the AOE tow tank
 
    //retrieve raw data
    uint16_t raw_static_pressure = analogRead(STATIC_SENSOR); // add/subtract constants to zero pressure sensors
    uint16_t raw_dynamic_pressure = analogRead(DYNAMIC_SENSOR);

    //convert from raw data to volts*1024 (5V = 1024 raw data increments)
    uint16_t static_voltage = (raw_static_pressure) * 5;
    uint16_t dynamic_voltage = (raw_dynamic_pressure) *5;

    //convert volts to Pascals (1V = 5500Pa)
    double static_pressure = (static_voltage * 5500) / 1024; //Add Pa to these to zero the sensors to the same Pa if needed
    double dynamic_pressure = (dynamic_voltage * 5500) / 1024;

    //calculated velocity in m/s using Bernoulli's Equation (see documentation)
    double velocity = sqrt((2 * abs(static_pressure - dynamic_pressure)) / 1000);
    return velocity;
}


double depth_function(void) {
  // this function can be called for in the race code and calculates depth

  //retrieve raw data
  double raw_static_pressure = analogRead(STATIC_SENSOR);

  //convert from raw data to volts (5V = 1024 raw data increments)
  double static_voltage = (raw_static_pressure/1024) * 5;

  //convert volts to Pascals (1V = 5500Pa)
  double static_pressure = static_voltage * 5500;
  
  //calculated depth assuming 2988.98Pa/ft
  double depth = static_pressure / 2988.98;

  return depth;

}
