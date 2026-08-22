#pragma once

#include <stdint.h>

#include "../ModLoader/Memory.h"
#include "Structs.h"

DataArrayPointer(LootTable*, loot_tables, (0xe28650 + base), 99);
DataArrayPointer(YenLootTable*, yen_loot_tables, (0xe28970 + base), 8);
DataPointer(void*, flags_struct, (0x115B498 + base));
DataPointer(DBInfo<ItemCreateDBEntry>, item_create_db, (0x117f5a0 + base));
DataPointer(DBInfo<ExpendablesDBEntry>, expendable_db, (0x117f5c0 + base));
DataPointer(DBInfo<ItemDBEntry>, item_db, (0x117f5e0 + base));
DataPointer(DBInfo<EquipmentDBEntry>, equipment_db, (0x117f600 + base));
DataPointer(DBInfo<SampleDBEntry>, sample_db, (0x117f620 + base));
DataPointer(MainData*, main_data, (0x166B418 + base));
DataPointer(uint32_t, item_messages, (0x1664150 + base));
DataPointer(HWND, MainWindowHandle, (0x166bbd8 + base));
