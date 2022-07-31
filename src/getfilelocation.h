#pragma once
#ifdef _WIN32
#include <Windows.h>
#include<iostream>
#include<string.h>
using namespace std;
extern "C" const IMAGE_DOS_HEADER __ImageBase;
#endif // defined(_WIN32)

#define PATH_MAX 1024

using namespace std;

string exePath(bool isExe)
{
    char buffer[PATH_MAX * 2 + 1] = { 0 };

#ifdef _WIN32
    int n = GetModuleFileNameA(isExe ? nullptr : (HINSTANCE)&__ImageBase, buffer, sizeof(buffer));
#else
    int n = readlink("/proc/self/exe", buffer, sizeof(buffer));
#endif

    string filePath;
    if (n <= 0)
    {
        filePath = "./";
    }
    else
    {
        filePath = buffer;
    }

#ifdef _WIN32
    for (auto& ch : filePath)
    {
        if (ch == '\\')
        {
            ch = '/';
        }
    }
    cout << filePath << endl;
    string::size_type pos = 0;
    string exe = "out/build/x64-debug/comp371_final.exe";
    pos = filePath.find(exe);
    if (pos == string::npos)
    {

        cout << "Can not find this string !" << endl;
    }
    else {
        filePath.replace(pos, 37, "");
        cout << filePath << endl;
    }
#endif

    return filePath;
}