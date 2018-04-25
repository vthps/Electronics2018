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
 */
char newChar;
String command;
int newCommand;
byte input;
String echo;
void setup() {
    Serial.begin(9600); //debugging information will be sent to serial monitor
    Serial3.begin(9600); //data will be written through 3rd set of ports at BR 9600
    Openlog logger(A10, 13);
    logger.data_logger_reset();
    logger.enter_command_mode();
    command = "";
    echo = "";
    newCommand = 0;
}

void loop() { 
   if(Serial.available() > 0){
      newChar = Serial.read();
      command = command + String(newChar);
      if(newChar == '\n'){ // we are done taking in the command, issue command and echo output
         Serial3.print(command);
         Serial.println(command);
         Serial.println("-------------------\n");
         command = "";
         while(1){
            if(Serial3.available()){
              input = Serial3.peek();
               if(input == '>' || input == '!'){ //we are back in command mode
                   input = Serial3.read();
                   break;
               }
               else{ //we are still taking in important info
                   echo = Serial3.readStringUntil('\n');
                   Serial.println(echo);
               }
            }
         }
      }    
   }
}






