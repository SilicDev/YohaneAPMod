#pragma once

#include <stdint.h>
#include <stdio.h>

#include "../ModLoader/Memory.h"
#include "Structs.h"
#include "Enums.h"

FuncPointer(void, SetupZKDB, (void), (0x0a6a30 + base));
FuncPointer(void, SetupBossDB, (void), (0x0a6be0 + base));
FuncPointer(void, SetupItemCreateDB, (void), (0x0a7350 + base));
FuncPointer(void, SetupItemDB, (void), (0x0a7500 + base));
FuncPointer(void, SetupEquipmentDB, (void), (0x0a76b0 + base));
FuncPointer(void, SetupExpendablesDB, (void), (0x0a7860 + base));
FuncPointer(void, SetupSampleDB, (void), (0x0a7a10 + base));
FuncPointer(float, GetEquipmentEffectStrength, (EquipmentEffect), (0x0b6020 + base));
FuncPointer(IceVariant*, CreateVariantFromString, (IceVariant*, char*, uint64_t), (0x0bb360 + base));
FuncPointer(IceVariant*, CreateVariantFromCStr, (IceVariant*, char*), (0x0bb490 + base));
FuncPointer(void, ClearVariant, (IceVariant*), (0x0bbdc0 + base));
FuncPointer(char*, VariantGetCStr, (IceVariant*), (0x0de8d0 + base));
FuncPointer(void, VariantToString, (IceVariant*), (0x0de900 + base));
FuncPointer(uint32_t, VariantGetHash, (IceVariant*), (0x0deb40 + base));
FuncPointer(void, OnEnemyKill, (void*), (0x359e60 + base));
FuncPointer(bool, OnBreakableHit, (void*, int64_t, void*), (0x489bb0 + base));
FuncPointer(bool, CanCraftRecipeID, (uint32_t), (0x667a00 + base));
FuncPointer(char, GetEquipSlots, (void*), (0x66ef30 + base));
FuncPointer(PVOID, DisplayMessage, (char*), (0x67e380 + base));
FuncPointer(PVOID, DisplayYenMessage, (int64_t), (0x67e580 + base));
FuncPointer(int32_t, AddItemCount, (Item[1000], uint32_t, int32_t), (0x47b020 + base));
FuncPointer(bool, RollDropMusicalScore, (void*), (0x938300 + base));
FuncPointer(void, RollLootFor, (uint32_t), (0x938570 + base));
FuncPointer(bool, ShouldShowRecipe, (ItemCreateDBEntry*), (0x93ce40 + base));
FuncPointer(bool, CanCraftRecipe, (ItemCreateDBEntry*), (0x93cf20 + base));
FuncPointer(char*, FindItemName, (ItemDBEntry*), (0x93d3c0 + base));
FuncPointer(char*, FindItemDescription, (ItemDBEntry*), (0x93d4c0 + base));
FuncPointer(int32_t, SetItemCount, (Item[1000], uint32_t, int32_t), (0x93d630 + base));
//FuncPointer(void, LoadEntityParam, (void), (0x93e250 + base));
FuncPointer(char, GetParlorCutsceneIDToPlay, (), (0x972440 + base));
FuncPointer(void*, GetFlagsStructBase, (), (0x09a74e0 + base));
FuncPointer(uint32_t, NextInt, (), (0x09a7620 + base));
FuncPointer(SaveData*, GetSaveGame, (byte), (0x9bb270 + base));
FuncPointer(FILE**, FileOpen, (FILE**, char*, int32_t, int32_t), (0xa50cf0 + base));
FuncPointer(LPVOID, AllocBytes, (uint64_t), (0xc79b44 + base));
FuncPointer(void, FreeMemory, (LPVOID), (0xca42e0 + base));
