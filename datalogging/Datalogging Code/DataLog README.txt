The Openlog.cpp and Openlog.h files make up the Openlog library which contains functionality for reading and writing to the Openlog through the arduino.


INTRODUCTION:

Every sketch you write involving the openlogger should start with something like this
--------------------------------------------------------------------------------------
Serial.begin(9600); //debugging information will be sent to serial monitor
Serial3.begin(9600); //data will be written through 3rd set of ports at BR 9600 
Openlog logger(A10, 13);
logger.data_logger_reset();
logger.enter_command_mode();
--------------------------------------------------------------------------------------
Set up the serial ports and create a new openlogger with reset pin A10 and and status led 13
reset it and enter command mode so that it is ready for whatever you want to throw at it
 
 
write_file(file_name, message to write) is your go to command for writing
read_file(file_name) will read out the contents of a single file to the serial monitor provided you know the absolute filepath
read_all_files() will read out the contents of all the files found on the sd card
clean_card() removes the auto generated .TXT files
there are other utility functions like remove_file(file name) that can be found if you look at the .cpp or .h

General Notes on Files Management:
Max size file names are 8.3, name.ext , I think max size dir names are just 8
use the .LOG ext to differentiate your files from the .TXT ones that are automatically created
File names are alpha numeric, probably easiest to avoid spaces in file names, spaces would have to be escaped when reading
Only 2 layers of subdirectores are permitted by the firm wire e.g. root/dirA/dib/file is as deep as you can go


NOTES ON READING PROCESS (instead of just looking at serial monitor):

The uploaded Using_Openlog_Example is how I was performing this method during testing.  

1. Read out all the files to the serial monitor using logger.read_all_files().
2. capture this read out using putty and export it to a log file.
3. navigate to the Dump_Files_Executable in the command line and call it as ./Dump_Files.exe path_to_log path_to_dump_dest

I use putty to capture this read out.  Download the appropriate putty version from here: https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html
(I just used all of the defaults on install)

When you go ahead and open putty, a configuration window will pop up.  Go ahead and select Connection type: Serial, leave baudrate at 9600 and change
Serial Line to where your Arduino is connect (mine is on COM6, on my windows 7 machine i go to start menu>right click computer>properties>device manager>ports
to find this).

Back in putty, navigate to the Session:Logging tab under Category in the top left. Change Session logging to All session output and browse your file system for 
the desired destination of the log file.  You can go ahead and click open in the bottom of the config window and you should see information stream into the monitor.
When it is down you can close the putty monitor window and your log file should have everything in it.

Procede to step 3 and you should be done.

GENERAL NOTES:
You will also find a sketch called debugging in the uploaded folder.  This is a small sketch I wrote that allows you to interact with the openlogger
like a traditional terminal.  Just pop open the serial monitor and you can type commands to it.  The openlogger's response will be output to the serial monitor.
Feel free to use it if things go south, or you just want to poke around with whats on the SD card