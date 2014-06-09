#ifndef API_CPP_H_
#define API_CPP_H_

int AccessFile(string file_path);
int AccessDirectory(string directory_path);
int CopyFile(string source_file_path, string destination_file_path);
int AppendFile(string source_file_path_1, string source_file_path_2);
int AppendFile(string source_file_path_1, string source_file_path_2, string destination_file_path);
int DeleteFile(string file_path);
int MoveFile(string source_file_path, string destination_file_path);
int BackUpFile(string file_path);
int DeleteBackUpFile(string file_path);
int CopyDirectory(string source_directory_path, string destination_directory_path);
int DeleteDirectory(string directory_path);
int CountFileLine(string file_path, const int start_line_num);
int CountFileLine(string file_path);
string SplitFilePath(string input_file_path, int work_mode);
string ParallelFilePath(string input_file_path, int ref_num, int work_mode);
string ModifyFileSuffix(string input_file_path, string suffix_new);
char FindUnitAfterNumber(string file_path, string keyword);
int ModifyFileContent(string file_path, string keyword, int num_char_jump, char keyword_start, char keyword_stop, string string_new, double number_new, int mode);
int Hold(int msecs);

#endif