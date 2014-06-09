#include <iostream>
using namespace std;
#include <fstream>
#include <string>

#include "Model_Movie.h"
#include "API_CPP.h"
#include "Private.h"

Model_Movie::Model_Movie( ) {

	num_genre_max = 30;
	num_country_max = 30;
	num_language_max = 30;
	num_runtime_max = 30;
	num_cast_max = 30;

}

int Model_Movie::Initialize( string link_list_file_path, int movie_num ) {

	GetIMDbLink(link_list_file_path, movie_num);
	ExtractID();
	DefineWebContentFilePath();
	FetchWebContent();
	ReadFiletoBuffer();
	ExtractTitle();
	ExtractYear();
	ExtractIMDbRating();
	ExtractIMDbRatingCount();
	ExtractCountry();
	ExtractLanguage();
	ExtractGenres();
	ExtractReleaseDate();
	ExtractRuntime();
	ExtractCast();

	return 0;
}

int Model_Movie::GetIMDbLink( string link_list_file_path, int movie_num ) {

	int i;
	char buffer[256];
	ifstream fin;

	fin.open(link_list_file_path.c_str());
	if (fin.is_open() == 0) {
		cout << "Cannot Open Link List File!" << endl;
		return 1;
	}

	for ( i = 0; i < movie_num; i++ )
		fin.getline(buffer, 256, '\n');
	fin.getline(buffer, 256, '\n');

	imdb_com_link = buffer;

	fin.close();
	
	imdb_cn_link = imdb_com_link;

//	imdb_cn_link = imdb_cn_link.replace(imdb_cn_link.find("com"), 3, "cn");
	imdb_cn_link = imdb_cn_link.replace(imdb_cn_link.find("imdb"), 4, "7176");

	imdb_cn_link.erase(imdb_cn_link.end() - 1, imdb_cn_link.end());

	return 0;
}

int Model_Movie::ExtractID() {

	size_t pos = imdb_com_link.find("/tt");

	if (pos == -1) {
		cout << "Wrong IMDb Link Provided:" << endl << imdb_com_link << endl;
		return 1;
	}

	id = imdb_com_link.substr(pos + 3, 7);

	return 0;
}

int Model_Movie::DefineWebContentFilePath() {

	web_content_file_imdb_com = "tt" + id + "_imdb_com.txt";

	web_content_file_imdb_cn = "tt" + id + "_imdb_cn.txt";

	return 0;
}

int Model_Movie::FetchWebContent() {

	if (AccessFile(web_content_file_imdb_com) != 0) {

		cout << "tt" << id << "\tFetching from IMDb.com...";

		FetchWebContentCurlBinWithCookie(imdb_com_link, "cookies.txt", web_content_file_imdb_com);

		cout << "Done!" << endl;
	}

	if (AccessFile(web_content_file_imdb_cn) != 0) {

		cout << "tt" << id << "\tFetching from IMDb.cn...";

		FetchWebContentCurlBin(imdb_cn_link, web_content_file_imdb_cn);

		cout << "Done!" << endl;
	}

	return 0;
}

int Model_Movie::ReadFiletoBuffer() {

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

int Model_Movie::ExtractTitle() {

	string keyword_title_imdb_com = "<title>";

	title_imdb_com = ExtractWebContent(web_content_imdb_com, keyword_title_imdb_com, 0, NULL, '(');

	string keyword_title_imdb_cn = "<h1>";

	title_imdb_cn = ExtractWebContent(web_content_imdb_cn, keyword_title_imdb_cn, 0, NULL, '<');

	return 0;
}

int Model_Movie::ExtractYear() {

	string keyword_year = "<title>";

	year = ExtractWebContent(web_content_imdb_com, keyword_year, 0, '(', ')');

	return 0;
}

int Model_Movie::ExtractIMDbRating() {

	string keyword_rating_imdb = "\"ratingValue\"";

	rating_imdb = ExtractWebContent(web_content_imdb_com, keyword_rating_imdb, 0, '>', '<');

	return 0;
}

int Model_Movie::ExtractIMDbRatingCount() {

	string keyword_rating_count_imdb = "ratingCount";

	rating_count_imdb = ExtractWebContent(web_content_imdb_com, keyword_rating_count_imdb, 0, '>', '<');

	//Eliminate Comma
	string::iterator it;
	for (it = rating_count_imdb.begin(); it != rating_count_imdb.end(); ++it) {
		if ( *it == ',')
			rating_count_imdb.erase(it);
	}

	return 0;
}

int Model_Movie::ExtractCountry() {

	country = new string [num_country_max];

	int i;

	string keyword_country = "Country:";

	country[0] = ExtractWebContent(web_content_imdb_com, keyword_country, (int)strlen("</h4>"), '>', '<');

	for (i = 1; i < num_country_max; i++) {
		if (!country[i - 1].empty()) {
			keyword_country = country[i - 1] + "</a>\n              <span class=\"ghost\">|</span>";
			country[i] = ExtractWebContent(web_content_imdb_com, keyword_country, 0, '>', '<');
		}
		else
			break;
	}

	return 0;
}

int Model_Movie::ExtractLanguage() {

	int i;

	language = new string [num_language_max];

	string keyword_language = "Language:";

	language[0] = ExtractWebContent(web_content_imdb_com, keyword_language, (int)strlen("</h4>"), '>', '<');

	for (i = 1; i < num_language_max; i++) {
		if (!language[i - 1].empty()) {
			keyword_language = language[i - 1] + "</a>\n            <span class=\"ghost\">|</span>";
			language[i] = ExtractWebContent(web_content_imdb_com, keyword_language, 0, '>', '<');
		}
		else
			break;
	}

	return 0;
}

int Model_Movie::ExtractGenres() {

	genres = new string [num_genre_max];

	int i;

	string keyword_genres = "Genres:";

	genres[0] = ExtractWebContent(web_content_imdb_com, keyword_genres, (int)strlen("</h4>"), '>', '<');

	for (i = 1; i < num_genre_max; i++) {
		if (genres[i - 1][0] == ' ')
			genres[i - 1].erase(0, 1);
		if (!genres[i - 1].empty()) {
			keyword_genres = genres[i - 1] + "</a>&nbsp;<span>|</span>";
			genres[i] = ExtractWebContent(web_content_imdb_com, keyword_genres, 0, '>', '<');
		}
		else
			break;
	}

	return 0;
}

int Model_Movie::ExtractReleaseDate() {

	string keyword_release_date = "Release Date:";

	release_date = ExtractWebContent(web_content_imdb_com, keyword_release_date, 0, '>', '(');

	if (release_date[0] == ' ')
		release_date.erase(0, 1);

	if (release_date[release_date.size() - 1] == ' ')
		release_date.erase(release_date.size() - 1, release_date.size());

	//Transfer to Day, Month, and Year

	release_day = release_date.substr(0, release_date.find_first_of(' '));

	if (release_day.size() == 1) {
		release_day.insert(0, "0");
	}

	release_month = release_date.substr(release_date.find_first_of(' ') + 1, release_date.find_last_of(' ') - release_date.find_first_of(' ') - 1);

	release_month = ConvertMonth(release_month);

	release_year = release_date.substr(release_date.find_last_of(' ') + 1, release_date.size() - release_date.find_last_of(' '));

	release_region = ExtractWebContent(web_content_imdb_com, keyword_release_date, 0, '(', ')');

	return 0;
}

int Model_Movie::ExtractRuntime() {

	int i;

	runtime_length = new string [num_runtime_max];
	runtime_version = new string [num_runtime_max];

	string keyword_runtime = "Runtime:";

	runtime_length[0] = ExtractWebContent(web_content_imdb_com, keyword_runtime, (int)strlen("</h4>"), '>', ' ');
	runtime_version[0] = "Theatrical";

	for (i = 1; i < num_genre_max; i++) {
		if (!runtime_length[i - 1].empty()) {
			keyword_runtime = runtime_length[i - 1] + " min</time>\n          <span class=\"ghost\">|</span>";
			runtime_length[i] = ExtractWebContent(web_content_imdb_com, keyword_runtime, 0, '>', ' ');
			runtime_version[i] = ExtractWebContent(web_content_imdb_com, keyword_runtime, 0, '(', ')');
		}
		else
			break;
	}

	return 0;
}

int Model_Movie::ExtractCast() {

	cast_id = new string [num_cast_max];

	string keyword_cast_id = "primary_photo\">";

	cast_id[0] = ExtractWebContent(web_content_imdb_com, keyword_cast_id, (int)strlen("\n<a href=\"/name"), 'm', '/');

	return 0;
}

int Model_Movie::Statistics() {

	return 0;
}

int Model_Movie::Display() {

	int i;
	
	cout << endl;
	cout << title_imdb_com << " | " << title_imdb_cn << endl;
	cout << "ID: " << id << endl;
	cout << "Year: " << year << endl;
	cout << "Rating: " << rating_imdb << " @ " << rating_count_imdb << endl;

	cout << "Country: " << country[0];
	for (i = 1; i < num_country_max; i++) {
		if (!country[i].empty())
		cout << " | " << country[i];
	else
		break;
	}
	cout << endl;

	cout << "Language: " << language[0];
	for (i = 1; i < num_language_max; i++) {
		if (!language[i].empty())
			cout << " | " << language[i];
		else
			break;
	}
	cout << endl;

	cout << "Genres: " << genres[0];
	for (i = 1; i < num_genre_max; i++) {
		if (!genres[i].empty())
			cout << " | " << genres[i];
		else
			break;
	}
	cout << endl;

	cout << "Release Date: " << release_year << '-' << release_month << '-' << release_day << " @ " << release_region << endl;

	cout << "Runtime: " << runtime_length[0] << " mins";
	for (i = 1; i < num_runtime_max; i++) {
		if (!runtime_length[i].empty())
			cout << " | " << runtime_length[i] << " mins (" << runtime_version[i] << ')';
		else
			break;
	}
	cout << endl;

	cout << "Cast: " << cast_id[0] << endl;

	cout << endl;

	return 0;
}