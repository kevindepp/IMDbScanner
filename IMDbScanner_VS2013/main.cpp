#include <iostream>
using namespace std;

#include "API_CPP.h"
#include "Model_Movie.h"
#include "Model_Person.h"

int main(int argc, char **argv) {

	string link_list_file_path;
	int num_movie;

	int i;

	if (argc == 1) {
		cout << "Using Default Link List File: \"Link_List.txt\"" << endl;
		link_list_file_path = "Link_List.txt";
	}
	else
		link_list_file_path = argv[1];

	if ( AccessFile( link_list_file_path ) != 0 ) {
		cout << "Cannot Locate Link List File:" << endl;
		cout << link_list_file_path.c_str() << endl;
		return 1;
	}

	num_movie = CountFileLine( link_list_file_path );
	if (num_movie == 0) {
		return 1;
	}
	cout << "num_movie = " << num_movie << endl;

	Model_Movie *movie = new Model_Movie [num_movie];
	for (i = 0; i < num_movie; i++) {
		movie[i].Initialize( link_list_file_path, i );
		movie[i].Display();
	}

	int num_person = 20;
	Model_Person *person = new Model_Person [num_person];
	for (i = 0; i < num_person; i++) {
		person[i].Initialize(i + 1);
		person[i].Display();

	}



	return 0;
}