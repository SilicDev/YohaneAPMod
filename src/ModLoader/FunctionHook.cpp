#include "FunctionHook.h"

#include "Memory.h"
#include "ModInfo.h"

extern HelperFunctions helperFunctions;

void FunctionHookBase::Hook(uintptr_t hook)
{
    helperFunctions.register_hook(this, hook);
}

uintptr_t FunctionHookBase::Unhook()
{
    if (hookaddr == NULL)
        return NULL;
    uintptr_t hook = hookaddr;
    hookaddr = NULL;
    helperFunctions.unregister_hook(this);
    return hook;
}