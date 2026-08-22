#include "IniFile.h"
#include <stdexcept>

bool IniSection::hasKey(const std::string& key) const
{
    return data.find(key) != data.end();
}

bool IniSection::isEmpty(const std::string& key) const
{
    auto value = data.find(key);
    if (value == data.end())
        return true;
    return value->second.empty();
}

std::string IniSection::getString(const std::string& key, const std::string& def) const
{
    auto value = data.find(key);
    if (value == data.end())
        return def;
    return value->second;
}

bool IniSection::getBool(const std::string& key, bool def) const
{
    auto value = data.find(key);
    if (value == data.end())
        return def;
    return !_stricmp(value->second.c_str(), "true");
}

template<
    typename Ty,
    std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t>
>
Ty IniSection::getInt(const std::string& key, Ty def) const
{
    auto value = data.find(key);
    if (value == data.end())
        return def;
    try
    {
        return (Ty)std::stoll(value->second);
    }
    catch (std::invalid_argument const& ex)
    {
        return def;
    }
    catch (std::out_of_range const& ex)
    {
        return def;
    }
}

float IniSection::getFloat(const std::string& key, float def) const
{
    auto value = data.find(key);
    if (value == data.end())
        return def;
    return std::stof(value->second);
}

void IniSection::setString(const std::string& key, const std::string& value)
{
    data[key] = value;
}

void IniSection::setBool(const std::string& key, bool value)
{
    std::string str;
    if (value)
        str = "True";
    else
        str = "False";
    data[key] = str;
}

template<
    typename Ty,
    std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t>
>
void IniSection::setInt(const std::string& key, Ty value)
{
    data[key] = std::to_string(value);
}

void IniSection::setFloat(const std::string& key, float value)
{
    data[key] = std::to_string(value);
}

bool IniSection::removeKey(const std::string& key)
{
    if (hasKey(key))
    {
        data.erase(key);
        return true;
    }
    return false;
}

IniFile::IniFile(const char* filename)
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0)
    {
        printf_s("Couldn't open INI file at \"%s\"", filename);
        return;
    }
    load(file);
    fclose(file);
}

IniFile::IniFile(FILE* file)
{
    load(file);
}

IniFile::~IniFile()
{
    clear();
}

bool IniFile::hasSection(const std::string& section) const
{
    return sections.find(section) != sections.end();
}

bool IniFile::hasKey(const std::string& section, const std::string& key) const
{
    auto value = sections.find(section);
    if (value == sections.end())
        return false;
    return value->second->hasKey(key);
}

bool IniFile::isEmpty(const std::string& section, const std::string& key) const
{
    auto value = sections.find(section);
    if (value == sections.end())
        return true;
    return value->second->isEmpty(key);
}

IniSection* IniFile::getSection(const std::string& section)
{
    auto value = sections.find(section);
    return value != sections.end() ? value->second : nullptr;
}

const IniSection* IniFile::getSection(const std::string& section) const
{
    auto value = sections.find(section);
    return value != sections.end() ? value->second : nullptr;
}

IniSection* IniFile::createSection(const std::string& section)
{
    auto value = sections.find(section);
    if (value != sections.end())
        return value->second;
    IniSection* new_section = new IniSection();
    sections[section] = new_section;
    return new_section;
}

std::string IniFile::getString(const std::string& section, const std::string& key, const std::string& def) const
{
    auto value = sections.find(section);
    if (value == sections.end())
        return def;
    return value->second->getString(key, def);
}

bool IniFile::getBool(const std::string& section, const std::string& key, bool def) const
{
    auto value = sections.find(section);
    if (value == sections.end())
        return def;
    return value->second->getBool(key, def);
}

template<
    typename Ty,
    std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t>
>
Ty IniFile::getInt(const std::string& section, const std::string& key, Ty def) const
{
    auto value = sections.find(section);
    if (value == sections.end())
        return def;
    return value->second->getInt(key, def);
}

float IniFile::getFloat(const std::string& section, const std::string& key, float def) const
{
    auto value = sections.find(section);
    if (value == sections.end())
        return def;
    return value->second->getFloat(key, def);
}

void IniFile::setString(const std::string& section, const std::string& key, const std::string& value)
{
    auto val = sections.find(section);
    if (val == sections.end())
        return;
    val->second->setString(key, value);
}

void IniFile::setBool(const std::string& section, const std::string& key, bool value)
{
    auto val = sections.find(section);
    if (val == sections.end())
        return;
    val->second->setBool(key, value);
}

template<
    typename Ty,
    std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t>
>
void IniFile::setInt(const std::string& section, const std::string& key, Ty value)
{
    auto val = sections.find(section);
    if (val == sections.end())
        return;
    val->second->setInt(key, value);
}

void IniFile::setFloat(const std::string& section, const std::string& key, float value)
{
    auto val = sections.find(section);
    if (val == sections.end())
        return;
    val->second->setFloat(key, value);
}

bool IniFile::removeSection(const std::string& section)
{
    if (hasSection(section))
    {
        sections.erase(section);
        return true;
    }
    return false;
}

bool IniFile::removeKey(const std::string& section, const std::string& key)
{
    auto val = sections.find(section);
    if (val == sections.end())
        return false;
    return val->second->removeKey(key);
}

void IniFile::save(const std::string& filename) const
{
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename.c_str(), "w");
    if (err == 0)
    {
        save(file);
        fclose(file);
    }
}

void IniFile::save(FILE* file) const
{
    std::list<std::pair<std::string, IniSection*>> list;
    for (const auto& iter : sections)
    {
        if (iter.first.empty())
            list.emplace_front(iter.first, iter.second);
        else
            list.emplace_back(iter.first, iter.second);
    }

    for (const auto& iter : list)
    {
        if (!iter.first.empty())
        {
            fprintf(file, "[%s]\n", iter.first.c_str());
        }

        for (const auto& kv : iter.second->data)
        {
            fprintf(file, "%s=%s\n", kv.first.c_str(), kv.second.c_str());
        }
    }
}

void IniFile::load(FILE* file)
{
    clear();
    fseek(file, 0, SEEK_SET);

    IniSection* current = new IniSection();
    sections[""] = current;

    while (!feof(file))
    {
        char line[0x800] = { 0 };
        char* ret = fgets(line, sizeof(line), file);
        if (ret == nullptr)
            break;
        const int line_len = (int)strnlen(line, sizeof(line));
        if (line_len == 0)
            continue;
        
        bool bracketstart = false;
        int firstequalspos = -1;
        int endbracketpos = -1;

        std::string buf;
        buf.reserve(line_len);

        for (int pos = 0; pos < line_len; pos++)
        {
            switch (line[pos])
            {
            case ';':
            case '#':
            case '\r':
            case '\n':
                pos = line_len;
                break;
            case '[':
                if (pos == 0)
                    bracketstart = true;
                buf += line[pos];
                break;
            case ']':
                endbracketpos = (int)buf.length();
                buf += line[pos];
                break;
            case '=':
                if (firstequalspos == -1)
                    firstequalspos = (int)buf.length();
                [[fallthrough]];
            default:
                buf += line[pos];
            }
        }

        if (bracketstart && endbracketpos != -1)
        {
            std::string section_name = buf.substr(1, endbracketpos - 1);
            auto section = sections.find(section_name);
            if (section != sections.end())
            {
                current = section->second;
            }
            else
            {
                current = new IniSection();
                sections[section_name] = current;
            }
        }
        else if (!buf.empty())
        {
            std::string key, value;
            if (firstequalspos > -1)
            {
                key = buf.substr(0, firstequalspos);
                value = buf.substr(firstequalspos + 1);
            }
            else
            {
                key = buf;
            }
            current->data[key] = value;
        }
    }
}

void IniFile::clear()
{
    for (auto& section : sections)
    {
        delete section.second;
    }

    sections.clear();
}
