#include "Arduino.h"
#include "common.h"


bool logData(dataStruct *dataToLog) {

    Serial.println(millis(), DEC);
    Serial.print("RPM = ");
    Serial.println(dataToLog->rpm, 4);

    Serial.print("Velocity = ");
    Serial.println(dataToLog->velocity, 4);

    Serial.print("Depth = ");
    Serial.println(dataToLog->depth, 4);

    Serial.print("Roll = ");
    Serial.print(dataToLog->accelPos.roll, DEC);
    Serial.print(", Pitch = ");
    Serial.print(dataToLog->accelPos.pitch, DEC);
    Serial.print(", Yaw = ");
    Serial.println(dataToLog->accelPos.yaw, DEC);

    Serial.println(" ");
    Serial.println(" ");
   

    return 0;
}

void datalogger_init(void){
    
}

