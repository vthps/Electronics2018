/*
	Openlog.cpp - Library for writing to openlog. Contains functionality for reading and writing files.
	Ideally all of the commands will terminate in command mode.  This makes actions self-contained and will
	prevent one command from interfering with others.
*/
#include "Openlog.h"
#include "Arduino.h"

/*
	Constructor for the Openlog class
*/
Openlog::Openlog(int reset_Pin, int status_Led){
	reset_pin = reset_Pin;
	status_led = status_Led;
	
	pinMode(reset_pin, OUTPUT);
    pinMode(status_led, OUTPUT);
	
	for(int i = 0; i < 50; i++){
		buff[i] = "";
	}
}

/*
	Reset the openlog by toggling the reset pin
*/
void Openlog::data_logger_reset(){
	  
    Serial.println("Resetting datalogger...");
	
    digitalWrite(reset_pin, LOW);
    delay(100);
    digitalWrite(reset_pin, HIGH);
	
	if (in_log_mode()){
		Serial.println("datalogger is now reset\n");
	}
    delay(delay_time);
}

/*
	Enter command mode by sending three escape characters to the openlog.  
*/
void Openlog::enter_command_mode(){
	
	Serial.println("Attempting to enter command mode...");
    
    Serial3.write(26);
    Serial3.write(26);
    Serial3.write(26);
	
	if(in_command_mode()){
		Serial.println("datalogger is in command mode and waiting for commands...\n");
	}
    delay(delay_time);
}

/*
	Creates a file with a given name, make sure to include the .log extension
	Precondition: a file named file_name does not exist prior to the execution of this function
*/
void Openlog::make_file(String file_name){
	if(in_command_mode()){
		sprintf(buff, "Attempting to create file: %s", file_name.c_str());
		Serial.println(buff);
		Serial.println("");
    }
    
    sprintf(buff, "new %s", file_name.c_str());
    Serial3.print(buff);
    Serial3.write(13); //makes new line
    
    Serial.println("File Created\n");
    delay(delay_time);
}

/*
	Removes a specified file
*/
void Openlog::remove_file(String file_name){
	if(in_command_mode()){
		sprintf(buff, "Attempting to remove file: %s", file_name.c_str());
		Serial.println(buff);
		Serial.println("");
	}
	
    sprintf(buff, "rm %s", file_name.c_str());
    Serial3.print(buff);
    Serial3.write(13); //makes new line

    Serial.println("File Removed\n");
    delay(delay_time);
}

/*
	Opens a file to write to.  Mostly used as a helper to the write_file function.
*/
void Openlog::open_file(String file_name){
	if(in_command_mode()){
		sprintf(buff, "Opening %s for writing...", file_name.c_str());
		Serial.println(buff);
		Serial.println("");
    }
    
    sprintf(buff, "append %s", file_name.c_str());
    Serial3.print(buff);
    Serial3.write(13); //make new line
	
	if(in_log_mode()){
		Serial.println("File is ready to write to\n");
		delay(delay_time);
	}
}

/*
	Writes a given message < 50 char to a specified file.  Upon completion, returns to command mode.
*/
void Openlog::write_file(String file_name, String message){
	open_file(file_name);

	Serial.println("Attempting to write message to file...");

    sprintf(buff, "%s", message.c_str());
    Serial3.print(buff);
    Serial3.write(13);
	
	Serial.println("Message written to file\n");
	enter_command_mode();
	delay(delay_time);
}

/*
	Creates a new directory named folder_name
*/
void Openlog::make_directory(String folder_name){
	if(in_command_mode()){
       sprintf(buff, "Attempting to create directory: %s", folder_name.c_str());
		Serial.println(buff);
		Serial.println("");
    }

    sprintf(buff, "md %s", folder_name.c_str());
    Serial3.print(buff);
    Serial3.write(13); //makes new line

    Serial.println("Directory Created\n");
    delay(delay_time);
}

/*
	Used to change to a directory specified by that directory's name
	Precondition: there exists a directory with folder_name
*/
void Openlog::change_directory(String folder_name){
	
	if(in_command_mode()){
      sprintf(buff, "Attempting to change to directory: %s", folder_name.c_str());
		Serial.println(buff);
		Serial.println("");
    }

    sprintf(buff, "cd %s", folder_name.c_str());
    Serial3.print(buff);
    Serial3.write(13); //makes new line

    sprintf(buff, "Successfully changed to directory: %s\n", folder_name.c_str());
    Serial.println(buff);
    Serial.write(13); //makes new line
    delay(delay_time);
}

/*
	Used to change to the parent directory
	Precondition:  There exists a parent directory
*/
void Openlog::change_to_parent_directory(){
	
	if(in_command_mode()){
        Serial.println("Attempting to change to parent directory");
		Serial.println("");
    }
    
    Serial3.print("cd ..");
    Serial3.write(13); //makes new line

    Serial.println("Successfully changed to parent directory\n");
     delay(delay_time);
	
}

/*
	Update the array of file names.
*/
void Openlog::update_file_array(){
	update_dir_array();
	data_logger_reset();
	enter_command_mode();//clean out messages so we just get the ls . log output
	
    Serial.println("Attempting to update file array\n");
	Serial.println("");

    num_files = 0;

	byte input;
    String input_file_name;
	String cur_dir= "";
	
	//ROOT DIRECTORY
    Serial3.println("ls *.LOG");
	
	while(1){//ditch characters until we start echoing files
		if(Serial3.available()){
			if(Serial3.read() == '/'){
				break;
			}
			
		}
	}
    
    while(1){
        if(Serial3.available()){
			input = Serial3.peek();
            if(input == '>' || input == '!'){ //we are back in command mode
				input = Serial3.read();
                break;
            }
            else{ //we are still taking in important info
                input_file_name = Serial3.readStringUntil('\n');
				if (input_file_name.indexOf(" ") != 0 && input_file_name.indexOf("LOG") > 0){ //we are in the current directory of a file
					Serial.println("Found File " + input_file_name);
					filenames[num_files] = cur_dir + input_file_name.substring(0, input_file_name.indexOf('.')) + ".LOG";
					num_files ++;
				
				}
            }
        }
    }
	//END ROOT DIRECTORY
	
	//START SUB DIRECTORIES
	for(int i = 0; i < num_dirs; i++){
		Serial3.print("cd ");
		Serial3.println(dirnames[i]);
		Serial3.println("ls *.LOG");
		cur_dir= dirnames[i];
		while(1){//ditch characters until we start echoing files
			if(Serial3.available()){
				if(Serial3.read() == '2'){ //we see FAT32 has been echoed and we are ready to start reading 
					break;
				}
				
			}
		}
		
		while(1){
			if(Serial3.available()){
				input = Serial3.peek();
				if(input == '>' || input == '!'){ //we are back in command mode
					input = Serial3.read();
					break;
				}
				else{ //we are still taking in important info
					input_file_name = Serial3.readStringUntil('\n');
					Serial.println("Output:\t" + input_file_name);
					if (input_file_name.indexOf(" ") != 0 && input_file_name.indexOf("*") == -1 && input_file_name.indexOf("LOG") > 0){ //we are in the current directory of a file
						Serial.println("Found File " + input_file_name);
						filenames[num_files] = cur_dir + input_file_name.substring(0, input_file_name.indexOf('.')) + ".LOG";
						num_files ++;
					
					}
				}
			}
		}
		
		Serial3.println("cd ..");
		Serial3.println("cd ..");//at max 2 sub directories, ensures we get to the root
	}
	//END SUB DIRECTORIES
    Serial.print("Finished Updating Array of File Names, there are now ");
    Serial.print(num_files);
    Serial.println(" files\n");
	delay(delay_time);
}

/*
	Update the array of file names.
	IN PROGRESS
*/
void Openlog::update_dir_array(){
	data_logger_reset();
	enter_command_mode();//clean out messages so we just get the ls . log output
	
    Serial.println("Attempting to update dir array\n");
	Serial.println("");

    num_dirs = 0;

	byte input;
    String input_dir_name;
	String orig;
	String cur_parent = "";
    Serial3.println("ls */"); //searching for subdirectories
	
	while(1){//ditch characters until we start echoing dirs
		if(Serial3.available()){
			if(Serial3.read() == '/'){
				break;
			}
			
		}
	}
    
    while(1){
        if(Serial3.available()){
			input = Serial3.peek();
            if(input == '>' || input == '!'){ //we are back in command mode
				input = Serial3.read();
                break;
            }
            else{ //we are still taking in important info
                input_dir_name = Serial3.readStringUntil('\n');
				orig = input_dir_name;
				input_dir_name.trim();
				Serial.println("Serial3 output " + input_dir_name);
				if(input_dir_name.indexOf("/") > 0 && input_dir_name.indexOf("SYSTEM") != 0){ //valid directory
					if(orig.indexOf(" ") == 0){ //there is a parent
						dirnames[num_dirs] = cur_parent + input_dir_name;
					}
					else{ //has no parent
						dirnames[num_dirs] = input_dir_name;
						cur_parent = input_dir_name;
					}
					num_dirs++;
				}
            }
        }
    }
	
    Serial.print("Finished Updating Array of Directory Names, there are now ");
    Serial.print(num_dirs);
    Serial.println(" directories\n");
	delay(delay_time);
}

/*
	Read a file of the specified name to the serial monitor.  Can be used on all the file names in the file name array to print everything
*/
void Openlog::read_file(String file_name){
	Serial.println(file_name);
	Serial.print("---------------");
	
	if(in_command_mode()){	
		sprintf(buff, "read %s", file_name.c_str());
		Serial3.print(buff);
		Serial3.write(13); //makes new line
		
		while(1){
			if(Serial3.available()){
				if(Serial3.read() == 'G'){ //we see a .LOG, specifically a G so we are done echoing and can start reading
					break;
				}
			}
		}
		

		for(int timeOut = 0; timeOut < 1000; timeOut++){ //times out after about a second
			
			while(Serial3.available()){ //this only checks 1 char is ready may cause problems if not all characters are ready
			
				char input[1000];
				int index = 0;
				
				while(Serial3.available()){
					if(Serial3.peek() == '>' || Serial3.peek() == '!'){
						Serial3.read();
						break;
					}
					input[index] = Serial3.read();
					index++;
					if(index == 999){
						break;
					}
				}
				
				input[index] = '\0'; //terminator character
				Serial.write(input);
			}
			  delay(1);
		}

		Serial.println("---------------\n");
	}
	
	delay(delay_time);
}

/*
	reads through the file name array and prints the contents of each file to the serial monitor
*/
void Openlog::read_all_files(){
	update_file_array();
	print_dir_names();
	print_file_names();
	Serial.println("Attempting to read out the contents of all files...\n");
	
	for(int i = 0; i < num_files; i++){
		read_file(filenames[i]);
	}
	
	Serial.println("Successfully read out all files\n");
	delay(delay_time);
}

/*
Prints out the name of all files on the sd card
*/
void Openlog::print_file_names(){
	Serial.println("Files in file array");
	Serial.println("-------------------");
	
	
	for(int i = 0; i < num_files; i++){ 
		Serial.println(filenames[i]);
	}
	
	Serial.println("-------------------\n");
	delay(delay_time);
}

/*
Prints out the name of all dirs on the sd card
*/
void Openlog::print_dir_names(){
	Serial.println("Dirs in file array");
	Serial.println("-------------------");
	
	
	for(int i = 0; i < num_dirs; i++){ 
		Serial.println(dirnames[i]);
	}
	
	Serial.println("-------------------\n");
	delay(delay_time);
}
/*
	Called to clean up the .TXT files the openlogger automatically creates (we read and write to text files too, but use the .LOG extension to differentiate them)
*/
void Openlog::clean_card(){
	if(in_command_mode()){
		remove_file("*.TXT");
	}
}

/*
	This helper function is called prior to any action requiring the openlog to be in command mode(e.g. changing directories)
*/
bool Openlog::in_command_mode(){
	if (mode != 0){ //not currently in command mode so we must check
		while(1){
			if(Serial3.available()){
				if(Serial3.read() == '>'){
					mode = 0; //assign mode 0 to indicate we are now in command mode
					break;
				}
			}
		} //make sure the open logger is responsive
	}
	return true;
}

/*
	This helper function is called prior to any action requiring the openlog to be in log mode (e.g. writing to a file)
*/
bool Openlog::in_log_mode(){
	if (mode != 1){ //not currently in log mode so we must check
		while(1){
			if(Serial3.available()){
				if(Serial3.read() == '<'){
					mode = 1; //assign mode 1 to indicate we are now in 
					break;
				}
			}
		} //make sure the open logger is responsive
	}
	return true;
}
