/*
	Openlog.h - Library the contains functions that Openlog to the openlog
*/
#ifndef Openlog_h
#define Openlog_h

#include "Arduino.h"
class Openlog
{
	public:
		Openlog(int reset_pin, int status_led);
		void data_logger_reset();
		void enter_command_mode();
		void make_file(String file_name);
		void remove_file(String file_name);
		void open_file(String file_name);
		void write_file(String file_name, String message);
		void Openlog_file(String file_name, String message);
		void make_directory(String folder_name);
		void change_directory(String folder_name);
		void change_to_parent_directory();
		void update_file_array();
		void update_dir_array();
		void read_file(String file_name);
		void read_all_files();
		void clean_card();
		void print_file_names();
		void print_dir_names();
		bool in_command_mode();
		bool in_log_mode();
		int get_reset_pin();
	private:
		//pins
		int reset_pin; //A10
		int status_led; //13
		
		//variables
		char buff[50]; //buffer that sprintf will use to print commands
		String file_name;
		String filenames[100];
		String dirnames[100];
		int num_files = 0;
		int num_dirs = 0;
		int file_limit = 100;
		int delay_time = 100;
		int mode = 1; //int to keep track of what mode the openlog is currently in
		//0 indicates command mode, 1 indicates log mode, there are several other modes that could be used in the future
};
#endif
