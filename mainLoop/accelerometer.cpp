#include "src/I2Cdev.h"
#include "src/MPU6050.h"
#include "Wire.h"
#include "common.h"

#define I2CDEV_IMPLEMENTATION I2CDEV_ARDUINO_WIRE

static MPU6050 accelgyro;

static int16_t gx, gy, gz;

void accel_init(void) {
    Wire.begin();

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void getGyroPos(accelData *accelStruct) {
    accelgyro.getRotation( &(accelStruct->roll), &(accelStruct->pitch), &(accelStruct->yaw) );
}

