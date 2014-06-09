#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <string>
#include <io.h>
//#include <Windows.h>

#include "API_CPP.h"

int AccessFile(string file_path) {

	/***********************
	** Check File Existence
	** 0: File Exists
	** 1: File Doesn't Exist
	***********************/

	if (_access(file_path.c_str(), 00) == -1) {
		return 1;
	}

	return 0;
}

int AccessDirectory(string directory_path) {

	/****************************
	** Check Directory Existence
	** 0: Directory Exists
	** 1: Directory Doesn't Exist
	****************************/

	if (_access(directory_path.c_str(), 00) == -1) {
		return 1;
	}
	
	return 0;
}

int CopyFile(string source_file_path, string destination_file_path) {

	if (AccessFile(source_file_path) != 0) {
		cout << "Cannot Locate Source File:" << endl;
		return 1;
	}

	string command = "copy \"" + source_file_path + "\" \"" + destination_file_path + "\" > nul";

	cout << command << endl;

	system(command.c_str());

	cout << "File Copied!" << endl;

	if (AccessFile(destination_file_path) != 0) {
		cout << "Cannot Locate Destination File:" << endl;
		return 1;
	}

	return 0;
}

int AppendFile(string source_file_path_1, string source_file_path_2) {

	/***************************************
	** Append File
	** Append Source File 2 to Source File 1
	***************************************/

	if (AccessFile(source_file_path_1) != 0) {
		cout << endl << "Cannot Locate Source File 1:" << endl;
		cout << source_file_path_1 << endl;
		return 1;
	}

	if (AccessFile(source_file_path_2) != 0) {
		cout << endl << "Cannot Locate Source File 2:" << endl;
		cout << source_file_path_2 << endl;
		return 1;
	}

	string command = "copy /b \"" + source_file_path_1 + "\" + \"" + source_file_path_2 + "\" \"" + source_file_path_1 + "\" > nul";

	system(command.c_str());

	return 0;

}

int AppendFile(string source_file_path_1, string source_file_path_2, string destination_file_path) {

	if (AccessFile(source_file_path_1) != 0) {
		cout << endl << "Cannot Locate Source File 1:" << endl;
		cout << source_file_path_1 << endl;
		return 1;
	}

	if (AccessFile(source_file_path_2) != 0) {
		cout << endl << "Cannot Locate Source File 2:" << endl;
		cout << source_file_path_2 << endl;
		return 1;
	}

	string command = "copy /b \"" + source_file_path_1 + "\" + \"" + source_file_path_2 + "\" \"" + destination_file_path + "\" > nul";

	system(command.c_str());

	return 0;
}

int DeleteFile(string file_path) {

	if (AccessFile(file_path) != 0) {
		cout << endl << "Cannot Locate File:" << endl;
		cout << file_path << endl;
		return 1;
	}

	string command = "del \"" + file_path + "\"";

	system(command.c_str());

	if (AccessFile(file_path) == 0) {
		cout << endl << "Failed to Delete File:" << endl;
		cout << file_path << endl;
		return 1;
	}

	return 0;
}

int MoveFile(string source_file_path, string destination_file_path) {

	if (AccessFile(source_file_path) != 0) {
		cout << endl << "Cannot Locate Source File:" << endl;
		cout << source_file_path << endl;
		return 1;
	}

	string command = "move \"" + source_file_path + "\" \"" + destination_file_path + "\" > nul";

	system(command.c_str());

	if (AccessFile(destination_file_path) != 0) {
		cout << endl << "Fail to Move File to Destination Path:" << endl;
		cout << destination_file_path << endl;
		return 1;
	}

	if (AccessFile(source_file_path) == 0) {
		cout << endl << "Fail to Delete Source File:" << endl;
		cout << source_file_path << endl;
		return 1;
	}

	return 0;

}

int BackUpFile(string file_path) {

	if (AccessFile(file_path) != 0) {
		return 1;
	}

	string backup_file_path = file_path + ".bak";

	CopyFile(file_path, backup_file_path);

	if (AccessFile(backup_file_path) != 0) {
		cout << endl << "Fail to Back Up File to Destination Path:" << endl;
		cout << backup_file_path << endl;
		return 1;
	}

	return 0;

}

int DeleteBackUpFile(string file_path) {

	string backup_file_path = file_path + ".bak";

	if (AccessFile(backup_file_path) != 0) {
		return 1;
	}

	DeleteFile(backup_file_path);

	if (AccessFile(backup_file_path) == 0) {
		cout << endl << "Fail to Delete Back Up File:" << endl;
		cout << backup_file_path << endl;
		return 1;
	}

	return 0;

}

int CopyDirectory(string source_directory_path, string destination_directory_path) {

	if (AccessDirectory(source_directory_path) != 0) {
		cout << endl << "Cannot Locate Source Directory:" << endl;
		cout << source_directory_path << endl;
		return 1;
	}

	string command = "xcopy \"" + source_directory_path + "\" \"" + destination_directory_path + "\" /i /q /y /s /e > nul";

	system(command.c_str());

	if (AccessDirectory(destination_directory_path) != 0) {
		cout << endl << "Fail to Copy Directory to Destination Path:" << endl;
		cout << destination_directory_path << endl;
		return 1;
	}

	return 0;

}

int DeleteDirectory(string directory_path) {

	if (AccessDirectory(directory_path) != 0) {
		return 1;
	}

	string command = "rd /S /Q \"" + directory_path + "\"";

	system(command.c_str());

	if (AccessDirectory(directory_path) == 0) {
		cout << endl << "Fail to Delete Directory:" << endl;
		cout << directory_path << endl;
		return 1;
	}

	return 0;
}

int CountFileLine(string file_path, const int start_line_num) {

	if (AccessFile(file_path) != 0) {
		cout << endl << "Fail to Locate File:" << endl;
		cout << file_path.c_str() << endl;
		return 0;
	}

	int num_line = 0;

	ifstream fin;
	char ch;

	fin.open(file_path.c_str());

	if (fin.is_open() != true) {
		cout << "Error in Opening File:"<< endl;
		cout << file_path.c_str() << endl;
		fin.close();
		return 0;
	}

	//Test if File is Empty
	fin.seekg(0, fin.beg);
	if (fin.peek() == EOF) {
		cout << "Empty File Provided:" << endl;
		cout << file_path.c_str() << endl;
		fin.close();
		return 0;
	}

	//Test if First Line Contains Return Only
	fin.seekg(0, fin.beg);
	fin.get(ch);
	if (ch == '\n') {
		cout << "Wrong File Format:" << endl;
		cout << file_path.c_str() << endl;
		fin.close();
		return 0;
	}

	//Calculate Number of Newline Characters (\n)
	fin.seekg(0, fin.beg);
	while (!fin.eof()) {
		fin.get(ch);
		if (ch == '\n')

			num_line++;
	}
	fin.close();

	//Eliminate Extra Newline Characters(\n) at the end of File
	fin.clear();
	fin.open(file_path.c_str());
	fin.seekg(-2, fin.end);
	fin.get(ch);
	while(ch == '\n') {
		num_line--;
		fin.seekg(-3, ios::cur);
		fin.get(ch);
	}
	fin.close();

	//Eliminate Lines before Starting Line Number
	num_line -= (start_line_num - 1);

	return num_line;
}

int CountFileLine(string file_path) {

	return CountFileLine(file_path, 1);

}

string SplitFilePath(string input_file_path, int work_mode) {

	/***************************************************
	** Split File Path
	** e.g. Given Path							C:\A.txt
	** 1: File Path Only						C:\
	** 2: File Path and Name without Suffix		C:\A
	** 3: File Name with Suffix					A.txt
	** 4: File Name without Suffix				A
	** 5: File Suffix Only						txt
	** Else: Input File Path					C:\A.txt
	***************************************************/

	string output;

	if (work_mode == 1)
		output = input_file_path.substr(0, input_file_path.rfind('\\') + 1);
	else if (work_mode == 2)
		output = input_file_path.substr(0, input_file_path.rfind('.'));
	else if (work_mode == 3)
		output = input_file_path.substr(input_file_path.rfind('\\') + 1, input_file_path.size() - (input_file_path.rfind('\\') + 1));
	else if (work_mode == 4)
		output = input_file_path.substr(input_file_path.rfind('\\') + 1, input_file_path.rfind('.') - (input_file_path.rfind('\\') + 1));
	else if (work_mode == 5)
		output = input_file_path.substr(input_file_path.rfind('.') + 1, input_file_path.size() - (input_file_path.rfind('.') + 1));
	else
		output = input_file_path;

	return output;
}

string ParallelFilePath(string input_file_path, int ref_num, int work_mode) {

	/*****************************************************
	** File Path Parallelization
	** e.g. Given Path and Reference Number: 1	C:\A.txt
	** 1: Parallel File Path with Suffix		C:\A_1.txt
	** 2: Parallel File Path without Suffix		C:\A_1
	** 3: Parallel File Name with Suffix		A_1.txt
	** 4: Parallel File Name without Suffix		A_1
	** Else: Input File Path					C:\A.txt
	*****************************************************/

	string output, ref_num_string, file_path, file_name, file_suffix;

	//Convert int to string
	stringstream ss;
	ss << ref_num;
	ref_num_string = ss.str();

	file_path = SplitFilePath(input_file_path, 1);
	file_name = SplitFilePath(input_file_path, 4);
	file_suffix = SplitFilePath(input_file_path, 5);

	if (work_mode == 1)
		output = file_path + file_name + '_' + ref_num_string + '.' + file_suffix;
	else if (work_mode == 2)
		output = file_path + file_name + '_' + ref_num_string;
	else if (work_mode == 3)
		output = file_name + '_' + ref_num_string + '.' + file_suffix;
	else if (work_mode == 4)
		output = file_name + '_' + ref_num_string;
	else
		output = input_file_path;
	return output;

}

string ModifyFileSuffix(string input_file_path, string suffix_new) {
	
	string output, file_path_no_suffix;

	file_path_no_suffix = SplitFilePath(input_file_path, 2);

	output = file_path_no_suffix + '.' + suffix_new;

	return output;

}

char FindUnitAfterNumber(string file_path, string keyword) {

	ifstream fin;
	string file_content;
	size_t loc;
	char ch, output;
	int i = 0;

	if (AccessFile(file_path) != 0) {
		cout << endl << "Fail to Locate File:" << endl;
		cout << file_path << endl;
		return NULL;
	}

	fin.open(file_path.c_str());
	if (fin.is_open() == false) {
		cout << "Can't Open Input File!" << endl;
		return 1;
	}
	while (fin.get(ch))
		file_content += ch;
	fin.close();

	loc = file_content.find(keyword) + keyword.size();

	while (file_content[loc + i] < 65)
		i++;

	output = file_content[loc + i];
	
	return output;

}

int ModifyFileContent(string file_path, string keyword, int num_char_jump, char keyword_start, char keyword_stop, string string_new, double number_new, int mode) {

	////////////////////////
	//Modify File Content
	//1: Replace with String
	//2: Replace with Number
	////////////////////////

	ifstream fin;
	ofstream fout;
	string file_content;
	char ch;
	int length_original_string = 0;
	size_t loc;

	fin.open(file_path.c_str());
	if (fin.is_open() == false) {
		cout << "Can't Open Input File!" << endl;
		return 1;
	}
	while (fin.get(ch))
		file_content += ch;
	fin.close();

	loc = file_content.find(keyword) + keyword.size();

	if (num_char_jump != 0)
		loc += num_char_jump;
	else
		loc = file_content.find(keyword_start, loc) + 1;

	while (file_content[loc + length_original_string] != keyword_stop) {
		length_original_string++;
	}

	if (mode == 1)
		file_content.replace(loc, length_original_string, string_new);
	else if (mode = 2) {

		//Convert double to string
		string number_new_string;
		stringstream ss;
		ss << number_new;
		number_new_string = ss.str();

		file_content.replace(loc, length_original_string, number_new_string);
	}

	fout.open(file_path.c_str());
	fout << file_content;
	fout.close();

	return 0;
}

int Hold(int msecs) {

//	Sleep(msecs);

	return msecs;
}