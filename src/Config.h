#pragma once

#include "tinyxml2/tinyxml2.h"
#include <stdint.h>
#include "hash/sha3.h"

class Config
{
private:
    static Config _config;
    Config* _this;
    std::string _content;
    std::string _filename;
    SHA3 _sha3;
    Config();
    virtual ~Config();

    tinyxml2::XMLDocument _doc;
    tinyxml2::XMLElement* _root, *_record;

    tinyxml2::XMLElement* getElement(tinyxml2::XMLElement* parent, const char* name);

public:
    void init(const std::string& filepath);
    void write();
    static Config* getInstance() { return &_config; };

    //xmlֻ���ִ�������������ִ�����
    std::string getString(const char* name, std::string def = "");
    int getInteger(const char* name, int def = 0);
    double getDouble(const char* name, double def = 0.0);
    bool getBool(bool& v, const char* name);

    void setString(const std::string v, const char* name);
    void setInteger(int v, const char* name);
    void setDouble(double v, const char* name);
    void setBool(bool v, const char* name);

    //��¼
    int getRecord(const char* name);
    void removeRecord(const char* name);
    void setRecord(int v, const char* name);
    void clearRecord(); //string replace(string str, const char *string_to_replace, const char *new_string);

    int replaceAllString(std::string& s, const std::string& oldstring, const std::string& newstring);

    int dealFilename(std::string& s);

};

