#pragma once
#include <stdint.h>
#include <Windows.h>

#include "Enums.h"

struct DBStats {
    uint32_t max;
    uint32_t entry_size;
    uint32_t unknown_offset;
    uint32_t data_offset;
    uint32_t name_table_offset;
};

template<typename T>
struct DBInfo {
    DBStats* stats;
    void* unknown;
    T* data;
    char** name_table;
};

#pragma pack(1)
struct ItemDBEntry
{
    uint32_t name_offset;
    uint32_t index;
    uint32_t type;
    uint8_t field_0x0c[0x8];
    float stats_1;
    float stats_2;
    float stats_3;
    float stats_4;
    uint8_t field_0x24[0x4];
};
#pragma pack()

struct ExpendablesEffectData
{
    ExpendablesEffect effect;
    float strength;
};

#pragma pack(1)
struct ExpendablesDBEntry
{
    uint32_t name_offset;
    int32_t index;
    ExpendablesEffectData effects[5];
};
#pragma pack()

#pragma pack(1)
struct SampleDBEntry
{
    uint32_t name_offset;
    int32_t s32;
    float v32;
    uint32_t text_offset;
    int32_t type;
};
#pragma pack()

struct EquipmentEffectData
{
    EquipmentEffect effect;
    float strength;
};

#pragma pack(1)
struct EquipmentDBEntry
{
    uint32_t name_offset;
    int32_t index;
    EquipmentEffectData effects[10];
};
#pragma pack()

#pragma pack(1)
struct ItemCreateDBEntry
{
    int32_t name_offset;
    int32_t index;
    uint32_t result_id;
    int32_t result_count;
    uint32_t ingredient1_id;
    int32_t ingredient1_count;
    uint32_t ingredient2_id;
    int32_t ingredient2_count;
    uint32_t ingredient3_id;
    int32_t ingredient3_count;
    uint32_t ingredient4_id;
    int32_t ingredient4_count;
};
#pragma pack()

#pragma pack(1)
struct Settings
{
    LPCRITICAL_SECTION lpCriticalSection;
    uint64_t _time;
    uint64_t field_0x10;
    int16_t field_0x18;
    uint8_t field_0x1a[0x6];
    uint32_t field_0x20;
    uint8_t field_0x24[0x4];
    uint64_t field_0x28;
    uint8_t field_0x30[0x50];
    uint64_t last_save_slot;
    uint8_t field_0x88[0x30];
    union {
        uint64_t data;
        struct {
            uint8_t language;
            uint8_t unk[7];
        };
    } field_0xb8;
    uint8_t field_0xc0[0x200];
    bool damage_numbers;
    bool crafting_alerts;
    bool subtitles;
    uint8_t controller_vibration;
    bool field_0x2c4;
    uint8_t master_volume;
    uint8_t bgm_volume;
    uint8_t se_volume;
    uint8_t voices_volume;
    uint8_t field_0x2c9[0x7];
    uint8_t field_0x2d0[0x2a0];
    int64_t field_0x570;
    uint8_t field_0x578[0x1688];
};
#pragma pack()

#pragma pack(8)
struct Item
{
    uint64_t first_pickup_time;
    uint64_t last_pickup_time;
    union {
        uint16_t data;
        struct {
            uint8_t count;
            uint8_t flags;
        };
    } data;
};
#pragma pack()

struct YenLootTableEntry
{
    uint32_t amount;
    uint32_t weight;
};

struct YenLootTable
{
    YenLootTableEntry* entries;
    uint32_t entry_count;
};

#pragma pack(8)
struct LootTableEntry
{
    uint32_t item;
    uint32_t weight;
};
#pragma pack()

#pragma pack(8)
struct LootTable
{
    int32_t drop_rate;
    LootTableEntry* entries;
    int64_t entry_count;
};
#pragma pack()

#pragma pack(1)
struct SaveData
{
    uint8_t field_0x0[0x40];
    uint64_t save_time;
    uint8_t field_0x48[8];
    uint64_t yen;
    uint32_t game_flags;
    uint8_t field_0x5c[8];
    uint32_t character_unlocks;
    uint8_t field_0x68[8];
    uint64_t progression_flags;
    uint8_t field_0x78[5];
    uint32_t bosses_defeated;
    uint8_t field_0x81[7];
    uint8_t field_0x88[0xD0];
    uint8_t flags[0x1B0];
    uint8_t field_0x308[0x1258];
    Item inventory[1000];
    uint8_t field_0x7320[0x240];
    uint32_t equipped_abilities;
    uint32_t equipped_weapon;
    uint32_t equipped_accessory1;
    uint32_t equipped_accessory2;
    uint32_t equipped_accessory3;
    uint8_t field_0x7574[0x78];
    Area area;
    int16_t field_0x75ed;
    uint8_t room;
    uint8_t field_0x75f0[0x610];
};
#pragma pack()

#pragma pack(1)
struct MainData
{
    Settings settings;
    SaveData loaded_saves[10];
    Settings current_settings;
    SaveData current_save;
    uint8_t saving;
    uint8_t _save_array[10];
    uint8_t field_0x58c0b[0x1d];
    uint8_t current_save_slot;
    uint8_t selected_save_slot;
    uint8_t field_0x58c2a[6];
};
#pragma pack()

#pragma pack(8)
struct IceVariant {
    union {
        char text[16];
        char* buffer;
    } data;
    size_t len;
    size_t capacity = 0xF;
    float float_value;
    uint32_t flags;
    uint32_t utf8_chars;
    uint32_t hash_key;
    uint32_t fraction_digits;
    int64_t int_value;
};
#pragma pack()
