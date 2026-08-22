#pragma once

#include <Windows.h>
#include "FunctionHook.h"

static const int ModLoaderVer = 1;

struct CommandHandler
{
    const char* Name;
    const char* ArgsText;
    const char* HelpText;
    void (*ProcessCommand)(const char* args);
};

struct Mod
{
    const char* Name;
    const char* Author;
    const char* Description;
    const char* Version;
    const char* Directory;
    const char* ID;
    HMODULE DLLHandle;
    bool SaveRedirect;

    template<typename T>
    T GetDllExport(const char* name)
    {
        if (!DLLHandle)
            return nullptr;
        return (T)GetProcAddress(DLLHandle, name);
    }
};

struct ModList
{
    Mod* (*begin)();
    Mod* (*end)();
    Mod& (*at)(size_t pos);
    Mod* (*data)();
    size_t(*size)();
    Mod* (*find)(const char* id);
    Mod* (*find_by_name)(const char* name);
    Mod* (*find_by_dir)(const char* dir);
    Mod* (*find_by_dll)(HMODULE dll);

    Mod& operator[](size_t pos)
    {
        return at(pos);
    }
};

typedef void (*console_text_handler)(const char* text);

struct HelperFunctions
{
    int version;
    void(*log_debug)(const char* fmt, ...);
    void(*log_info)(const char* fmt, ...);
    void(*log_warn)(const char* fmt, ...);
    void(*log_error)(const char* fmt, ...);
    void (*register_hook)(FunctionHookBase*, uintptr_t);
    void (*unregister_hook)(FunctionHookBase*);
    void (*register_command)(CommandHandler&);
    console_text_handler (*set_unhandled_text_handler)(console_text_handler);
    void (*replace_file)(const char* orig, const char* replace);
    ModList* mods;
};

typedef void(* ModInitFunc)(const char* path, const HelperFunctions& helper_functions, unsigned int modIndex);

template<typename TRet, typename... TArgs>
struct ModEvent
{
    typedef TRet(*EventHandler)(TArgs... args);

    void(*RegisterHandler)(EventHandler handler);
    TRet(*RaiseEvent)(TArgs... args);
};

struct ModInfo
{
    int version;
    // Optional Init override
    ModInitFunc Init;
};
