#pragma once

#include <string>
#include <vector>
#include <algorithm>

static std::string& rtrim(std::string& s, const char* whitespace = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(whitespace) + 1);
    return s;
}

static std::string& ltrim(std::string& s, const char* whitespace = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(whitespace));
    return s;
}

static std::string& trim(std::string& s, const char* whitespace = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, whitespace), whitespace);
}

static std::vector<std::string> split(std::string& s, const char* delimiter)
{
    std::vector<std::string> elems;
    size_t pos = 0;
    while (pos < s.size())
    {
        if (pos != 0)
        {
            pos++;
        }
        elems.push_back(s.substr(pos, s.find(delimiter, pos)));
        pos = s.find(delimiter, pos);
    }
    return elems;
}

static std::string& ReplaceAll(std::string& str, const std::string& oldVal, const std::string& newVal)
{
    size_t pos = 0;
    while ((pos = str.find(oldVal, pos)) != std::string::npos)
    {
        str.replace(pos, oldVal.length(), newVal);
        pos += newVal.length();
    }
    return str;
}