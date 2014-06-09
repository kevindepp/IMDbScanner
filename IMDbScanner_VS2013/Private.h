#ifndef PRIVATE_H_
#define PRIVATE_H_

int FetchWebContentCurlBin( string web_link, string file_path);
int FetchWebContentCurlBinWithCookie( string web_link, string cookie_file_path, string output_file_path );
string ExtractWebContent(string web_content, string keyword, int num_char_jump, char keyword_start, char keyword_stop);
string ConvertMonth(string month_phrase_full);
string IntToString(int input);

#endif