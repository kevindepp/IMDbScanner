#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "Model_Person.h"
#include "API_CPP.h"
#include "Private.h"

Model_Person::Model_Person( ) {

	num_name_part_eng = 1;
	num_name_part_chs = 1;
	num_translation_chs = 1;

	list_name_eng_discard_file_path = "List_Name_Eng_Discard.txt";
	
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

	num_name_eng_discard = CountFileLine(list_name_eng_discard_file_path);

	cout << num_name_eng_discard << endl;

	name_eng_discard = new string [num_name_eng_discard];

	ifstream fin;
	fin.open(list_name_eng_discard_file_path.c_str());
	for (i = 0; i < num_name_eng_discard; i++)
		fin >> name_eng_discard[i];
	fin.close();

	for (i = 0; i < num_name_eng_discard; i++)
		cout << name_eng_discard[i] << endl;

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

int Model_Person::Display() {

	cout << "ID: " << id << endl;

	cout << "Name: " << name_eng_full << " | " << name_chs_full << endl;

	cout << "# of Trans CHS: " << num_translation_chs << endl;

	cout << "Name Part: " << num_name_part_eng << " | " << num_name_part_chs<< endl;

	return 0;
}