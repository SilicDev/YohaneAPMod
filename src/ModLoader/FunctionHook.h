#pragma once
#include <stdio.h>
#include <exception>
#include <memory>

#include "Memory.h"

class FunctionHookBase
{
public:
    FunctionHookBase(FunctionHookBase&) = delete;
    FunctionHookBase(FunctionHookBase&&) = delete;
    ~FunctionHookBase()
    {
        Unhook();
    }
protected:
    FunctionHookBase(uintptr_t addr) : origaddr(addr) {}

    void Hook(uintptr_t hook);
    uintptr_t Unhook();

protected:
    byte origbytes[12] = { 0 };
    const uintptr_t origaddr;
    uintptr_t hookaddr = NULL;
};

template<typename TRet, typename... TArgs>
class FunctionHook : FunctionHookBase
{
public:
    typedef TRet(*TFunc)(TArgs...);

    FunctionHook(TFunc addr): FunctionHookBase((uintptr_t)addr) {}

    void Hook(TFunc hook)
    {
        FunctionHookBase::Hook((uintptr_t)hook);
    }

    TRet CallOriginal(TArgs... args)
    {
        if (hookaddr != NULL)
        {
            // Restore original function
            byte hookbytes[12] = { 0 };
            memcpy(hookbytes, (void*)origaddr, 12);
            WriteData((void*)origaddr, (void*)origbytes, 12);
            TRet ret = ((TFunc)(origaddr))(args...);
            // Reapply hook
            WriteData((void*)origaddr, (void*)hookbytes, 12);
            return ret;
        }
        else
            return ((TFunc)(origaddr))(args...);
    }

    TFunc GetFunctionAddress()
    {
        return origaddr;
    }

    TFunc GetHook()
    {
        return hookaddr;
    }
};

template<typename... TArgs>
class FunctionHook<void, TArgs...> : FunctionHookBase
{
public:
    typedef void(*TFunc)(TArgs...);

    FunctionHook(TFunc addr) : FunctionHookBase((uintptr_t)addr) {}

    void Hook(TFunc hook)
    {
        FunctionHookBase::Hook((uintptr_t)hook);
    }

    void CallOriginal(TArgs... args)
    {
        if (hookaddr != NULL)
        {
            // Restore original function
            byte hookbytes[12] = { 0 };
            memcpy(hookbytes, (void*)origaddr, 12);
            WriteData((void*)origaddr, (void*)origbytes, 12);
            ((TFunc)(origaddr))(args...);
            // Reapply hook
            WriteData((void*)origaddr, (void*)hookbytes, 12);
        }
        else
            ((TFunc)(origaddr))(args...);
    }

    TFunc GetFunctionAddress()
    {
        return origaddr;
    }

    TFunc GetHook()
    {
        return hookaddr;
    }
};
