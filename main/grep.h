#ifndef GREP_H
#define GREP_H


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

class grep
{
private:
	std::wstring str_;
	std::wstring path_;
	std::vector<std::wstring> all_paths;
	std::vector<std::wstring> out_;
	char arg_ = '0';
	size_t cnt = 0;

	void initFiles(std::wstring path);
	bool check(std::wstring str);

public:
	grep() = delete;
	grep(std::wstring str, std::wstring path);
	grep(std::wstring str, std::wstring path, char arg);
	~grep();

	void print_all_paths();
	void grep_find();
	void print();
};

#endif