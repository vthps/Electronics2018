#include <stdint.h>

//Structs
typedef struct {
    int8_t left;  //Control surface position in degrees. 
    int8_t right; //0 degrees is straight 
    int8_t top;
    int8_t bottom;
} actuatorData;

typedef struct {
    int16_t roll; //Accelerometer position in degrees
    int16_t pitch;
    int16_t yaw;
} accelData;

typedef struct {
    uint16_t rpm;
    double velocity; 
    double depth;
    accelData accelPos;
//    actuatorData actuatorPos;
} dataStruct;


//Function prototypes

void rpm_count_increment(void); 
uint16_t rpm_calculate(void);
void rpm_sensor_setup(void);

bool logData(dataStruct *dataToLog);
void datalogger_init(void);

double get_velocity(void);
double depth_function(void);

void getGyroPos(accelData *accelStruct);

