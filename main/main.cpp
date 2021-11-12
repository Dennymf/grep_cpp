#include <iostream>
#include "grep.h"

char check_argument(char* arg)
{
    if (arg[0] != '-')
    {
        std::cerr << "Expectded -<argument>\n";
    }
    if (arg[1] == 'c' || arg[1] == 'h' || arg[1] == 'i' || arg[1] == 'l'
        || arg[1] == 'n' || arg[1] == 'v' || arg[1] == 'w')
    {
        return arg[1];
    }
    else
    {
        std::cerr << "Expected only argument: c, h, i, l, n, v, w\n";
    }
}

std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

int main(int argc, char* argv[])
{
    if (argc >= 3 && argc <= 4)
    {
        std::wstring str = s2ws(argv[1]);
        std::wstring path = s2ws(argv[2]);

        if (argc == 3)
        {
            grep a(str, path);
            a.grep_find();
            a.print();
        }
        else if (argc == 4)
        {
            char arg = check_argument(argv[3]);
            grep a(path, path, arg);
            a.grep_find();
            a.print();
        }
    }
    else
    {
        std::cerr << "Expected two arguments: grepcpp <string> <path> or Expected three arguments: grepcpp <string> <path> -<argument>\n";
    }
}