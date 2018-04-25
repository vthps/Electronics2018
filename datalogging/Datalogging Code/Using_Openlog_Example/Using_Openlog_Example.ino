#include <Openlog.h>


/*
 * Pin connections:
 * Openlog  |  Arduino
 *   GRN    |    A10
 *   RXI    |    TX3
 *   TX0    |    RX3
 *   VCC    |    5V
 *   GDN    |    not connected
 *   BLK    |    GND
 *   
 *   
 *   Things to keep in mind:
 *   Max size files names are 8.3, name.ext
 *   File names are alpha numeric, probably easiest to avoid spaces in file names
 *   spaces would have to be escaped when reading.
 *   Max 2 subdirectories without modifying the firmware (might be worth looking into)
 *   Honestly I should switch all instances of strings to char * [], but from what I have read
 *   arduino reimplemented String and its better than it was before
 */

void setup() {
    Serial.begin(9600); //debugging information will be sent to serial monitor
    Serial3.begin(9600); //data will be written through 3rd set of ports at BR 9600 
    Openlog logger(A10, 13);
    logger.data_logger_reset();
    logger.enter_command_mode();
    logger.read_all_files();
    logger.clean_card();
}

void loop() { 
    //only the light should blink after this indicating completion.
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
} 






