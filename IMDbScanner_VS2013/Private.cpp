#include <iostream>
using namespace std;
#include <string>
#include <sstream>

#include "API_CPP.h"
#include "Private.h"

int FetchWebContentCurlBin( string web_link, string file_path) {

	if (AccessFile("curl.exe") != 0) {
		cout << "Unable to Locate 'curl.exe'" << endl;
		return 1;
	}

	//curl.exe
	//-o file_path	--> Output to File
	//-s			--> Silent Mode

	string command = "curl.exe \"" + web_link + "\" -s -o " + file_path;

	system(command.c_str());

	return 0;
}

int FetchWebContentCurlBinWithCookie( string web_link, string cookie_file_path, string output_file_path ) {

	if (AccessFile("curl.exe") != 0) {
		cout << "Unable to Locate 'curl.exe'" << endl;
		return 1;
	}

	if (AccessFile(cookie_file_path) != 0) {
		cout << "Unable to Locate Cookie File" << endl;
		return 1;
	}

	//curl.exe
	//-b cookie_file_path
	//-o output_file_path
	//-s ->Silent Mode

//	string command = "curl.exe \"" + web_link + "\" -b " + cookie_file_path + " -o " + output_file_path + " -s";
	string command = "curl.exe \"" + web_link + "\" -b \"" + cookie_file_path + "\" -s -o " + output_file_path;

//	cout << command << endl;

	system(command.c_str());

	return 0;
}

string ExtractWebContent(string web_content, string keyword, int num_char_jump, char keyword_start, char keyword_stop) {

	size_t loc;
	string string_find;
	int length_original_string = 0;

	loc = web_content.find(keyword);

	if (loc != -1)
		loc += keyword.size();
	else
		return "";

	if (num_char_jump != 0) {
		loc += num_char_jump;
	}

	if (keyword_start != NULL) {
		loc = web_content.find(keyword_start, loc) + 1;
	}

	while (web_content[loc + length_original_string] != keyword_stop) {
		length_original_string++;
	}

	//Eliminate Last Space 
	if (web_content[loc + length_original_string - 1] == ' ')
		length_original_string--;

	string_find = web_content.substr(loc, length_original_string);

	return string_find;
}

string ConvertMonth(string month_phrase_full) {

	string month_number;

	if (month_phrase_full == "January")
		month_number = "01";
	else if (month_phrase_full == "February")
		month_number = "02";
	else if (month_phrase_full == "March")
		month_number = "03";
	else if (month_phrase_full == "April")
		month_number = "04";
	else if (month_phrase_full == "May")
		month_number = "05";
	else if (month_phrase_full == "June")
		month_number = "06";
	else if (month_phrase_full == "July")
		month_number = "07";
	else if (month_phrase_full == "August")
		month_number = "08";
	else if (month_phrase_full == "September")
		month_number = "09";
	else if (month_phrase_full == "October")
		month_number = "10";
	else if (month_phrase_full == "November")
		month_number = "11";
	else if (month_phrase_full == "December")
		month_number = "12";

	return month_number;
}

string IntToString(int input) {

	//Convert int to string
	stringstream ss;
	ss << input;
	string output = ss.str();

	return output;

}