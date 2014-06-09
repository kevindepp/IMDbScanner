#ifndef MODEL_MOVIE_H_
#define MODEL_MOVIE_H_

class Model_Movie
{
private:

	int num_genre_max, num_country_max, num_language_max, num_runtime_max, num_cast_max;

	string id;
	string imdb_com_link, imdb_cn_link;
	string web_content_file_imdb_com, web_content_file_imdb_cn;
	string web_content_imdb_com, web_content_imdb_cn;
	string title_imdb_com, year, rating_imdb, rating_my, rating_count_imdb;
	string title_imdb_cn;
	string *country;
	string *language;
	string *runtime_length, *runtime_version;
	string *genres;
	string release_date, release_day, release_month, release_year;
	string release_region;
	string *cast_id;

public:

	Model_Movie::Model_Movie( );

	int Model_Movie::Initialize( string link_list_file_path, int movie_num );
	int Model_Movie::GetIMDbLink( string link_list_file_path, int movie_num );
	int Model_Movie::ExtractID();
	int Model_Movie::DefineWebContentFilePath();
	int Model_Movie::FetchWebContent();
	int Model_Movie::ReadFiletoBuffer();
	int Model_Movie::ExtractTitle();
	int Model_Movie::ExtractYear();
	int Model_Movie::ExtractIMDbRating();
	int Model_Movie::ExtractIMDbRatingCount();
	int Model_Movie::ExtractCountry();
	int Model_Movie::ExtractLanguage();
	int Model_Movie::ExtractGenres();
	int Model_Movie::ExtractReleaseDate();
	int Model_Movie::ExtractRuntime();
	int Model_Movie::ExtractCast();
	int Model_Movie::Statistics();
	int Model_Movie::Display();

};

#endif