#include "File.h"

#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/uio.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

#ifdef __clang__
//#define chdir _chdir
#endif

File::File()
{
}


File::~File()
{
}

bool File::fileExist(const std::string& filename)
{
    if (filename.length() <= 0)
    { return false; }

    std::fstream file;
    bool ret = false;
    file.open(filename.c_str(), std::ios::in);
    if (file)
    {
        ret = true;
        file.close();
    }
    return ret;
}

std::string File::readStringFromFile(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "rb");
    if (fp == nullptr)
    {
        printf("Can not open file %s\n", filename.c_str());
        return "";
    }
    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0, 0);
    char* s = new char[length + 1];
    for (int i = 0; i <= length; s[i++] = '\0');
    fread(s, length, 1, fp);
    std::string str(s);
    fclose(fp);
    delete[] s;
    return str;
}

std::string File::getFileExt(const std::string& filename)
{
    int pos_p = getLastPathPos(filename);
    int pos_d = filename.find_last_of('.');
    if (pos_p < pos_d)
    { return filename.substr(pos_d + 1); }
    return "";
}

//һ���������һ���㣬Ϊ1���ҵ�һ����
std::string File::getFileMainname(const std::string& filename, FindMode mode)
{
    int pos_p = getLastPathPos(filename);
    int pos_d = filename.find_last_of('.');
    if (mode == FINDFIRST)
    { pos_d = filename.find_first_of('.', pos_p + 1); }
    if (pos_p < pos_d)
    { return filename.substr(0, pos_d); }
    return filename;
}

std::string File::changeFileExt(const std::string& filename, const std::string& ext)
{
    auto e = ext;
    if (e != "" && e[0] != '.')
    { e = "." + e; }
    return getFileMainname(filename) + e;
}

std::string File::getFilePath(const std::string& filename)
{
    int pos_p = getLastPathPos(filename);
    if (pos_p != std::string::npos)
    { return filename.substr(0, pos_p); }
    return "";
}

//���������ļ������ļ���ֻ������
std::string File::fingFileWithMainName(const std::string& filename)
{
#ifndef _MSC_VER
    return "";
#else
    _finddata_t file;
    long fileHandle;
    std::string path = getFilePath(filename);
    std::string ext = getFileExt(filename);
    if (path != "") { path = path + '\\'; }
    std::string findname = getFileMainname(filename) + ".*";
    std::string ret = "";
    fileHandle = _findfirst(findname.c_str(), &file);
    ret = path + file.name;
    if (getFileExt(ret) == ext)
    {
        _findnext(fileHandle, &file);
        ret = path + file.name;
        if (getFileExt(ret) == ext)
        { ret = ""; }
    }
    _findclose(fileHandle);
    return ret;
#endif
}

std::string File::toLowerCase(const std::string& str)
{
    auto str1 = str;
    std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
    return str1;
}

std::string File::formatString(const char* format, ...)
{
    char s[4096];
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(s, sizeof(s), format, arg_ptr);
    va_end(arg_ptr);
    return s;
}

std::string File::getFilenameWithoutPath(const std::string& filename)
{
    std::string filename2 = filename;
    int pos_p = getLastPathPos(filename2);
    if (pos_p != std::string::npos)
    { return filename2.substr(pos_p + 1); }
    return filename2;
}

void File::changePath(const std::string& path)
{
    chdir(path.c_str());
}

int File::replaceAllString(std::string& s, const std::string& oldstring, const std::string& newstring)
{
    int pos = s.find(oldstring);
    while (pos >= 0)
    {
        s.erase(pos, oldstring.length());
        s.insert(pos, newstring);
        pos = s.find(oldstring, pos + newstring.length());
    }
    return pos + newstring.length();
}

int File::getLastPathPos(const std::string& filename)
{
    int pos_win = std::string::npos;
#ifdef _WIN32
    pos_win = filename.find_last_of('\\');
#endif // _WIN32
    int pos_other = filename.find_last_of('/');
    if (pos_win == std::string::npos)
    {
        return pos_other;
    }
    else
    {
        if (pos_other == std::string::npos)
        {
            return pos_win;
        }
        else
        {
            return pos_other > pos_win ? pos_other : pos_win;
        }
    }
}


