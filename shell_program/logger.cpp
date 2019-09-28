#include "logger.h"
#include<string>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include<fstream>

using namespace std;

//This is the implementation for the logger object for storing the history items for the 
//jhsh shell.

//Constructor. Takes in filename as parameter to allow flexible history file usage
logger::logger(string history_filename) {
	//Declare necessary variables
	string current_line;
	int oldFD = dup(STDIN_FILENO);
	this->history_filename = history_filename;

	//Attempt to open the history file with a file descriptor, message and exit upon failure
	int file_descriptor = open((char*)history_filename.c_str(), O_RDONLY);
	if (file_descriptor < 0) {
		cout << "ERROR: Cannot open history file. Does file exist?\n" << endl;
		exit(1);
	}

	//Switch STDIN to file_descriptor to utilize getline
	dup2(file_descriptor, 0);

	//Loop through file and get all the commands
	while (getline(cin, current_line)) {
		live_history.push_back(current_line);	
	}

	//Switch STDIN back to the console, close file descriptor
	dup2(oldFD, 0);
	close(file_descriptor);
}

//Returns the last history item as a string
string logger::get_last_item() {
	return live_history.back();
}

//Adds a history item to both the live vector and the history file
void logger::add_history_item(string history_item) {
	//Add string to vector
	live_history.push_back(history_item);

	//Declare necessary variables
	int oldFD = dup(STDOUT_FILENO);
	
	//Attempt to opent the history file with a file descriptor, message and exit upon failure
	int file_descriptor = open((char*)history_filename.c_str(), O_WRONLY | O_APPEND);
	if (file_descriptor < 0) {
		cout << "ERROR: Cannot open history file. Does file exist?\n" << endl;
		exit(1);
	}

	//Swtich STDOUT to file_descriptor to utilize cout
	dup2(file_descriptor, 1);

	//Write the string to the file
	cout << history_item << endl;

	//Switch STDOUT back to the console, close the file descriptor
	dup2(oldFD, 1);
	close(file_descriptor);
}

//Print out the entire history from the live vector
void logger::print_history() {
	//iterate through the vector and print the items
	for (unsigned i = 0; i < live_history.size(); i++) {
		cout << live_history[i] << endl;
	}
}
