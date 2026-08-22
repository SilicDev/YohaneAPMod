#pragma once
#include <string>
#include <unordered_map>

class IniSection
{
public:
    bool hasKey(const std::string& key) const;
    bool isEmpty(const std::string& key) const;

    std::string getString(const std::string& key, const std::string& def = std::string()) const;

    bool getBool(const std::string& key, bool def = false) const;

    template<
        typename Ty,
        std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t> = nullptr
    >
    Ty getInt(const std::string& key, Ty def = 0) const;

    float getFloat(const std::string& key, float def = 0) const;

    void setString(const std::string& key, const std::string& value);

    void setBool(const std::string& key, bool value);

    template<
        typename Ty,
        std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t> = nullptr
    >
    void setInt(const std::string& key, Ty value);

    void setFloat(const std::string& key, float value);

    bool removeKey(const std::string& key);

protected:
    std::unordered_map<std::string, std::string> data;

    friend class IniFile;
};

class IniFile
{
public:
    IniFile(const char* filename);
    IniFile(FILE* file);
    ~IniFile();

    bool hasSection(const std::string& section) const;
    bool hasKey(const std::string& section, const std::string& key) const;
    bool isEmpty(const std::string& section, const std::string& key) const;

    IniSection* getSection(const std::string& section);
    const IniSection* getSection(const std::string& section) const;
    IniSection* createSection(const std::string& section);

    std::string getString(const std::string& section, const std::string& key, const std::string& def = std::string()) const;

    bool getBool(const std::string& section, const std::string& key, bool def = false) const;

    template<
        typename Ty,
        std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t> = nullptr
    >
    Ty getInt(const std::string& section, const std::string& key, Ty def = 0) const;

    float getFloat(const std::string& section, const std::string& key, float def = 0) const;

    void setString(const std::string& section, const std::string& key, const std::string& value);

    void setBool(const std::string& section, const std::string& key, bool value);

    template<
        typename Ty,
        std::enable_if_t<std::is_integral_v<Ty>, std::nullptr_t> = nullptr
    >
    void setInt(const std::string& section, const std::string& key, Ty value);

    void setFloat(const std::string& section, const std::string& key, float value);

    bool removeSection(const std::string& section);
    bool removeKey(const std::string& section, const std::string& key);

    void save(const std::string& filename) const;
    void save(FILE* file) const;
    void clear();

protected:
    std::unordered_map<std::string, IniSection*> sections;

    void load(FILE* file);
};
