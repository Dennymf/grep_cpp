#include "grep.h"

void grep::initFiles(std::wstring path)
{
	WIN32_FIND_DATAW wfd;
	FindFirstFileW(path.c_str(), &wfd);

	if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		all_paths.push_back(path);
	}
	std::wstring path1 = path + L"\\*.*";
	HANDLE const hFind = FindFirstFileW(path1.c_str(), &wfd);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (wfd.cFileName[0] != L'.' && wfd.dwFileAttributes != FILE_ATTRIBUTE_REPARSE_POINT) {
				if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					all_paths.push_back(path + L"\\" + &wfd.cFileName[0]);
				}
			}
		} while (NULL != FindNextFileW(hFind, &wfd));

		FindClose(hFind);
	}

	path1 = path + L"\\*";
	HANDLE const hFind1 = FindFirstFileW(path1.c_str(), &wfd);

	if (INVALID_HANDLE_VALUE != hFind1)
	{
		do
		{
			if (wfd.cFileName[0] != L'.' && wfd.dwFileAttributes != FILE_ATTRIBUTE_REPARSE_POINT) {
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					initFiles(path + L"\\" + &wfd.cFileName[0]);
				}
			}
		} while (NULL != FindNextFileW(hFind1, &wfd));

		FindClose(hFind1);
	}
}

bool grep::check(std::wstring str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (tolower(str[i]) != tolower(str_[i]))
		{
			return false;
		}
	}

	return true;
}

grep::grep(std::wstring str, std::wstring path)
{
	str_ = str;
	path_ = path;
	initFiles(path_);
}

grep::grep(std::wstring str, std::wstring path, char arg)
{
	str_ = str;
	path_ = path;
	arg_ = arg;
	initFiles(path_);
}

grep::~grep()
{
}

void grep::print_all_paths()
{
	for (const auto& it : all_paths)
	{
		std::wcout << it << '\n';
	}
}

void grep::grep_find()
{
	size_t i = 0;
	for (const auto& path : all_paths)
	{
		std::wifstream file(path, std::ios::in);
		std::vector<std::wstring> tmp;
		std::wstring line;
		while (getline(file, line))
		{
			tmp.push_back(line);
		}
		file.close();

		for (const auto& cur_line : tmp)
		{
			if (arg_ != 'i' && arg_ != 'w' && arg_ != 'v')
			{
				if (cur_line.find(str_) != std::wstring::npos)
				{
					std::wstring out;
					if (arg_ == 'c')
					{
						cnt++;
						out = path + L": " + cur_line;
					}
					else if (arg_ == 'h')
					{
						out =  cur_line;
					}
					else if (arg_ == 'l')
					{
						out = path;
					}
					else if (arg_ == 'n')
					{
						out = std::to_wstring(i) + L" " + cur_line;
					}
					else
					{
						out = cur_line;
					}
					out_.push_back(out);
				}
			}
			else if (arg_ == 'v')
			{
				if (cur_line.find(str_) == std::wstring::npos)
				{
					out_.push_back(cur_line);
				}
			}
			else if (arg_ == 'w')
			{
				std::wstring tmp = L" " + str_ + L" ";
				if (cur_line.find(tmp) != std::wstring::npos)
				{
					out_.push_back(cur_line);
				}
			}
			else if (arg_ == 'i')
			{
				for (size_t j = 0; j < cur_line.size() - str_.size(); ++j)
				{
					if (check(cur_line.substr(j, str_.size())))
					{
						out_.push_back(cur_line);
					}
				}
			}
		}
		++i;
	}
}

void grep::print()
{
	if (arg_ == 'c')
	{
		std::cout << cnt << '\n';
		return;
	}
	if (out_.size() == 0)
	{
		std::cout << "Not found\n";
		return;
	}
	for (const auto& it : out_)
	{
		std::wcout << it << '\n';
	}
}
