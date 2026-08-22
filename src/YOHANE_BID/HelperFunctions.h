#pragma once
#include <stdint.h>

#include "Functions.h"
#include "Variables.h"

extern "C" static uint32_t __fastcall Inti_Hash(char* text)
{
    size_t len = strlen(text);
    uint32_t hash = 0xCDE723A5;
    for (uint32_t i = 0; i < len; i++)
    {
        hash = (hash + text[i]) * 141;
    }
    return hash;
}

extern "C" static uint64_t __fastcall Inti_BaseKey(char* text)
{
    size_t len = strlen(text);
    uint64_t hash = 0xA1B34F58CAD705B2;
    for (uint32_t i = 0; i < len; i++)
    {
        hash = (hash + text[i]) * 141;
    }
    return hash;
}

extern "C" static char* __fastcall GetItemDescription(uint32_t item_id)
{
    if (item_id < item_db.stats->max - 1)
    {
        return FindItemDescription(item_db.data + item_id);
    }
    else
        return nullptr;
}

extern "C" static char* __fastcall GetItemName(uint32_t item_id)
{
    if (item_id < item_db.stats->max - 1)
    {
        return FindItemName(item_db.data + item_id);
    }
    else
        return nullptr;
}
