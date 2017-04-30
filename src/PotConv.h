#pragma once

extern "C"
{
#ifdef _MSC_VER
#include "libiconv/iconv.h"
#else
#include <iconv.h>
#endif
}

#include <cstring>
#include <string>
#include <algorithm>

#define CONV_BUFFER_SIZE 2048

class PotConv
{
public:
    PotConv();
    virtual ~PotConv();

    static std::string conv(const std::string& src, const char* from, const char* to);
    static std::string conv(const std::string& src, const std::string& from, const std::string& to);
    static std::string cp936toutf8(const std::string& src) { return conv(src, "cp936", "utf-8"); }
    static std::string cp950toutf8(const std::string& src) { return conv(src, "cp950", "utf-8"); }
};

