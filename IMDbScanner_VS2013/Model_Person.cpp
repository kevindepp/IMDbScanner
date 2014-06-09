#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "Model_Person.h"
#include "API_CPP.h"
#include "Private.h"

Model_Person::Model_Person( ) {

	num_name_part_eng_max = 10;
	num_name_part_chs_max = 10;
	name_eng_partial = new string [num_name_part_eng_max];
	name_chs_partial = new string [num_name_part_chs_max];

	num_name_part_eng = 1;
	num_name_part_chs = 1;
	num_translation_chs = 1;

	list_name_eng_discard_file_path = "List_Name_Eng_Discard.txt";
	list_name_trans_eng_chs_file_path = "List_Name_Trans_Eng_Chs.csv";
	
}

int Model_Person::Initialize(int id_in) {

	ExtractID(id_in);
	GetIMDbLink();
	DefineWebContentFilePath();
	FetchWebContent();
	ReadFiletoBuffer();
	ExtractName();
	ReformatEngName();
	CountNumTransChs();
	CountNamePart();
	SplitNameEng();
	ReadChsTrans();
	ChooseTransChs();
	SplitNameChs();
//	WriteChsTrans();
	

	return 0;
}

int Model_Person::ExtractID(int id_in) {

	int i;

	id = IntToString(id_in);

	size_t id_size = id.size();

	if (id_size < 7) {
		for (i = 0; i < (7 - id_size); i++) {
			id.insert(i, "0");
		}
	}

	return 0;
}

int Model_Person::GetIMDbLink() {

	link_imdb_com = "http://www.imdb.com/name/nm" + id + '/';

//	link_imdb_cn = "http://www.imdb.cn/name/nm" + id;
	link_imdb_cn = "http://www.7176.com/name/nm" + id;

	return 0;
}

int Model_Person::DefineWebContentFilePath() {

	web_content_file_imdb_com = "nm" + id + "_imdb_com.txt";

	web_content_file_imdb_cn = "nm" + id + "_imdb_cn.txt";

	return 0;
}

int Model_Person::FetchWebContent() {

	if (AccessFile(web_content_file_imdb_com) != 0) {

		cout << "nm" << id << "\tFetching from IMDb.com...";

		FetchWebContentCurlBinWithCookie(link_imdb_com, "cookies.txt", web_content_file_imdb_com);

		cout << "Done!" << endl;
	}

	if (AccessFile(web_content_file_imdb_cn) != 0) {

		cout << "nm" << id << "\tFetching from IMDb.cn...";

		FetchWebContentCurlBin(link_imdb_cn, web_content_file_imdb_cn);

		cout << "Done!" << endl;
	}

	return 0;
}

int Model_Person::ReadFiletoBuffer() {

	char ch;
	ifstream fin;

	fin.open(web_content_file_imdb_com.c_str());
	if (fin.is_open() == 0) {
		cout << "Cannot Open Web Content File:" << endl;
		cout << web_content_file_imdb_com << endl;
		return 1;
	}

	while(fin.get(ch))
		web_content_imdb_com += ch;

	fin.close();

	fin.clear();
	fin.open(web_content_file_imdb_cn.c_str());
	if (fin.is_open() == 0) {
		cout << "Cannot Open Web Content File:" << endl;
		cout << web_content_file_imdb_cn << endl;
		return 1;
	}

	while(fin.get(ch))
		web_content_imdb_cn += ch;

	fin.close();

	return 0;
}

int Model_Person::ExtractName() {

	string keyword_name_imdb_com = "og:title";

	name_eng_full = ExtractWebContent(web_content_imdb_com, keyword_name_imdb_com, 0, '\"', '\"');

	string keyword_error_imdb_cn_1 = "数据库发生错误";

	if (web_content_imdb_cn.find(keyword_error_imdb_cn_1) != -1)
		return 1;

	string keyword_name_imdb_cn = "<TITLE>";

	name_chs_full = ExtractWebContent(web_content_imdb_cn, keyword_name_imdb_cn, 0, NULL, ' ');

	return 0;
}

int Model_Person::ReformatEngName () {

	int i;

	size_t pos;

	num_name_eng_discard = CountFileLine(list_name_eng_discard_file_path);

	name_eng_discard = new string [num_name_eng_discard];

	ifstream fin;
	fin.open(list_name_eng_discard_file_path.c_str());
	for (i = 0; i < num_name_eng_discard; i++)
		fin >> name_eng_discard[i];
	fin.close();

	for (i = 0; i < num_name_eng_discard; i++) {
		name_eng_discard[i].insert(0, " ");
		name_eng_discard[i] += " ";
	}

	for (i = 0; i < num_name_eng_discard; i++) {
		pos = 0;
		while (pos != -1) {
			pos = name_eng_full.find(name_eng_discard[i], pos);
			if (pos != -1 && pos << name_eng_full.size()) {
				name_eng_full.replace(pos, name_eng_discard[i].size(), " ");
			}
		}
	}

	return 0;
}

int Model_Person::CountNumTransChs() {

	int i;

	for (i = 0; i < name_chs_full.size(); i++) {
		if (name_chs_full[i] == '/')
			num_translation_chs++;
	}

	return 0;
}

int Model_Person::CountNamePart() {

	int i;

	for (i = 0; i < name_eng_full.size(); i++) {
		if (name_eng_full[i] == ' ')
			num_name_part_eng++;
	}

	if (!name_chs_full.empty()) {

		string keyword_chs_name_divider = "·";

		size_t pos = 0;

		while (pos != -1) {
			pos = name_chs_full.find(keyword_chs_name_divider, pos + keyword_chs_name_divider.size());
//			cout << "pos = " << pos << endl;
//			cout << "Next Word: " << name_chs_full[pos + 2] << name_chs_full[pos + 3] << endl;
			if (pos <= name_chs_full.size())
				num_name_part_chs++;
//			cout << "num_name_part_chs = " << num_name_part_chs << endl;
		}
	}

	num_name_part_chs -= (num_translation_chs - 1);

	return 0;
}

int Model_Person::SplitNameEng() {

	int i;

	size_t pos_start = 0, pos_stop = 0;

	if (num_name_part_eng == 1)
		name_eng_partial[0] = name_eng_full;
	else {
		for (i = 0; i < num_name_part_eng; i++) {
			pos_stop = name_eng_full.find(' ', pos_start);
			if (pos_stop != -1)
				name_eng_partial[i] = name_eng_full.substr(pos_start , pos_stop - pos_start);	//The Words between two Spaces
			else
				name_eng_partial[i] = name_eng_full.substr(pos_start);

			//Set the Starting Point for Next Search
			pos_start = pos_stop + 1;
		}
	}

	return 0;
}

int Model_Person::ReadChsTrans() {

	int i;
	ifstream fin;

	num_chs_trans_record = CountFileLine(list_name_trans_eng_chs_file_path);

	cout << "# of Name Translation in Record: " << num_chs_trans_record << endl;

	name_eng_record = new string[num_chs_trans_record];
	name_chs_record = new string[num_chs_trans_record];

	fin.open(list_name_trans_eng_chs_file_path);
	for (i = 0; i < num_chs_trans_record; i++) {
		getline(fin, name_eng_record[i], ',');
		getline(fin, name_chs_record[i], '\n');
		cout << "[" << i << "]\t" << name_eng_record[i] << " | " << name_chs_record[i] << endl;
	}
	fin.close();

	return 0;
}

int Model_Person::ChooseTransChs() {

	int i, j;
	size_t pos_start = 0, pos_stop = 0;

	if (num_translation_chs == 1)
		return 0;

	name_chs_multi_full = new string [num_translation_chs];

	for (i = 0; i < num_translation_chs; i++) {
		pos_stop = name_chs_full.find("/", pos_start);
		if (pos_stop != -1)
			name_chs_multi_full[i] = name_chs_full.substr(pos_start , pos_stop - pos_start);
		else
			name_chs_multi_full[i] = name_chs_full.substr(pos_start);
		//Set the Starting Point for Next Search
		pos_start = pos_stop + 1;
	}

	//Reset
	pos_start = pos_stop = 0;

	string keyword_chs_name_divider = "·";

	name_chs_multi_part = new string *[num_translation_chs];
	for (i = 0; i < num_translation_chs; i++) {
		name_chs_multi_part[i] = new string [num_name_part_chs];
		for (j = 0; j < num_name_part_chs; j++) {
			pos_stop = name_chs_multi_full[i].find(keyword_chs_name_divider, pos_start);
			if (pos_stop != -1)
				name_chs_multi_part[i][j] = name_chs_multi_full[i].substr(pos_start, pos_stop - pos_start);
			else
				name_chs_multi_part[i][j] = name_chs_multi_full[i].substr(pos_start);

			//Set the Starting Point for Next Search
			pos_start = pos_stop + 2;
			cout << '[' << i << "][" << j << "]\t" << name_chs_multi_part[i][j] << endl;
		}
		//Reset
		pos_start = pos_stop = 0;
	}

	//Erase the Original String that Contains Multiple Chinese Translation
	name_chs_full.clear();

	int choose = 0;
	cout << "Multiple Chinese Translations Found for \"" << name_eng_full << "\"" << endl;
	cout << "Please Choose the Preferred Translation." << endl;
	for (j = 0; j < num_name_part_chs; j++) {
		choose = 0;
		while (choose < 1 || choose > num_name_part_chs) {
			for (i = 0; i < num_translation_chs; i++) {
				cout << '[' << i + 1 << "]\t" << name_chs_multi_part[i][j] << endl;
			}
			cout << "Choose Translation for:\t" << name_eng_partial[j] << endl;
			cin >> choose;
		}
		name_chs_full += name_chs_multi_part[choose - 1][j];
		if (j != (num_name_part_chs - 1))
			name_chs_full += "·";
	}

	return 0;
}

int Model_Person::SplitNameChs() {

	int i;

	size_t pos_start = 0, pos_stop = 0;

	string keyword_chs_name_divider = "·";

	if (name_chs_full.empty())
		return 1;

	if (num_name_part_chs == 1)
		name_chs_partial[0] = name_chs_full;
	else {
		for (i = 0; i < num_name_part_chs; i++) {
			pos_stop = name_chs_full.find(keyword_chs_name_divider, pos_start);
			if (pos_stop != -1)
				name_chs_partial[i] = name_chs_full.substr(pos_start , pos_stop - pos_start);	//The Words between two Keywords
			else
				name_chs_partial[i] = name_chs_full.substr(pos_start);

			//Set the Starting Point for Next Search
			pos_start = pos_stop + 2;		//The Divider Keyword "·" Takes 2 Chars
		}
	}

	return 0;
}

int Model_Person::WriteChsTrans(){

	int i;
	ofstream fout;

	if (name_chs_full.empty())
		return 1;

	fout.open(list_name_trans_eng_chs_file_path, ios::app);
	for (i = 0; i < num_name_part_eng; i++)
		fout << name_eng_partial[i] << ',' << name_chs_partial[i] << endl;
	fout.close();

	return 0;
}



int Model_Person::Display() {

	int i;

	cout << "ID: " << id << endl;

	cout << "Name: " << name_eng_full << " | " << name_chs_full << endl;

	cout << "# of Trans CHS: " << num_translation_chs << endl;

	cout << "Name Part: " << num_name_part_eng << " | " << num_name_part_chs<< endl;

	if (num_name_part_eng != num_name_part_chs) {
		cout << "# of Name Part Mismatch!";
		cin.get();
	}

	cout << "Name Match: " << endl;
	for (i = 0; i < num_name_part_eng; i++)
		cout << "[" << i + 1 << "]\t" << name_eng_partial[i] << '\t' << name_chs_partial[i] << endl;


	return 0;
}