#ifndef MODEL_PERSON_H_
#define MODEL_PERSON_H_

class Model_Person
{

private:

	int num_name_part_eng, num_name_part_chs;
	int num_name_part_eng_max, num_name_part_chs_max;
	int num_translation_chs;

	string list_name_eng_discard_file_path;
	int num_name_eng_discard;
	string *name_eng_discard;
	string *name_chs_multi_full;
	string **name_chs_multi_part;

	string id;
	string link_imdb_com, link_imdb_cn;
	string web_content_file_imdb_com, web_content_file_imdb_cn;
	string web_content_imdb_com, web_content_imdb_cn;

	string name_eng_full, name_chs_full;
	string *name_eng_partial, *name_chs_partial;

public:

	Model_Person::Model_Person();
	int Model_Person::Initialize(int id_in);
	int Model_Person::ExtractID(int id_in);
	int Model_Person::GetIMDbLink();
	int Model_Person::DefineWebContentFilePath();
	int Model_Person::FetchWebContent();
	int Model_Person::ReadFiletoBuffer();
	int Model_Person::ExtractName();
	int Model_Person::ReformatEngName();
	int Model_Person::CountNumTransChs();
	int Model_Person::CountNamePart();
	int Model_Person::SplitNameEng();
	int Model_Person::ChooseTransChs();
	int Model_Person::SplitNameChs();


	int Model_Person::Display();


};

#endif