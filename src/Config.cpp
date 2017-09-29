#include "Config.h"
#include "File.h"

Config Config::_config;

Config::Config()
{
    _this = &_config;
    //init();
}


Config::~Config()
{
    //delete doc;
    //write();
}

void Config::init(const std::string& filepath)
{
    _filename = filepath + "/config.xml";
    printf("try find config file: %s\n", _filename.c_str());
    _doc.LoadFile(_filename.c_str());
#ifdef _DEBUG
    //_doc.Print();
#endif
    //��ʼ���ṹ
    if (_doc.Error())
    {
        //_doc.DeleteChildren();
        _doc.LinkEndChild(_doc.NewDeclaration());
        _root = _doc.NewElement("root");
    }
    else
    {
        _root = _doc.FirstChildElement("root");
    }

    _record = _root->FirstChildElement("record");
    if (!_record)
    {
        _record = _root->InsertFirstChild(_doc.NewElement("record"))->ToElement();
    }
}

void Config::write()
{
    //_doc.LinkEndChild(_doc.NewDeclaration());
    _doc.LinkEndChild(_root);
    _doc.SaveFile(_filename.c_str());
}

tinyxml2::XMLElement* Config::getElement(tinyxml2::XMLElement* parent, const char* name)
{
    auto p = parent->FirstChildElement(name);
    if (p)
    {
        return p;
    }
    else
    {
        p = parent->InsertFirstChild(_doc.NewElement(name))->ToElement();
        p->SetText("");
        return p;
    }
}

int Config::getRecord(const char* name)
{
    if (strlen(name) == 0) { return 0; }
    auto mainname = File::getFileMainname(File::getFilenameWithoutPath(name));
    dealFilename(mainname);
    const char* str = getElement(_record, mainname.c_str())->GetText();
    if (!str)
    {
        return 0;
    }
    return atoi(str);
}

void Config::removeRecord(const char* name)
{
    if (strlen(name) == 0) { return; }
    auto mainname = File::getFileMainname(File::getFilenameWithoutPath(name));
    dealFilename(mainname);
    _record->DeleteChild(getElement(_record, mainname.c_str()));
}

void Config::setRecord(int v, const char* name)
{
    if (strlen(name) == 0) { return; }
    auto mainname = File::getFileMainname(File::getFilenameWithoutPath(name));
    dealFilename(mainname);
    getElement(_record, mainname.c_str())->SetText(File::formatString("%d", v).c_str());
}

void Config::clearRecord()
{
    if (_record)
    {
        _record->DeleteChildren();
    }
}

std::string Config::getString(const char* name, std::string def /*= ""*/)
{
    auto p = _root->FirstChildElement(name);
    if (p && p->FirstChild())
    {
        return p->GetText();
    }
    else
    {
        return def;
    }
}

int Config::getInteger(const char* name, int def /*= 0*/)
{
    return atoi(getString(name, File::formatString("%d", def)).c_str());
}

double Config::getDouble(const char* name, double def /*= 0.0*/)
{
    return atof(getString(name, File::formatString("%f", def)).c_str());
}

bool Config::getBool(bool& v, const char* name)
{
    return atoi(getString(name, "0").c_str()) != 0;
}

void Config::setString(const std::string v, const char* name)
{
    getElement(_root, name)->SetText(v.c_str());
}

void Config::setInteger(int v, const char* name)
{
    setString(File::formatString("%d", v), name);
}

void Config::setDouble(double v, const char* name)
{
    setString(File::formatString("%f", v), name);
}

void Config::setBool(bool v, const char* name)
{
    setString(File::formatString("%d", v != 0), name);
}

int Config::replaceAllString(std::string& s, const std::string& oldstring, const std::string& newstring)
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

int Config::dealFilename(std::string& s)
{
    //replaceAllString(s, " ", "_");
    s = _sha3(s);
    s = "_" + s;
}

