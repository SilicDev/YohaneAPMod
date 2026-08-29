#include "Locations.h"

#include "../ModLoader/ModInfo.h"
#include "../YOHANE_BID/HelperFunctions.h"
#include "../YOHANE_BID/Variables.h"
#include "Archipelago.h"

extern HelperFunctions helperFunctions;

std::set<LocationID> rescue_locations = {
    LocationID::CHIKA_RESCUE,
    LocationID::KANAN_RESCUE,
    LocationID::DIA_RESCUE,
    LocationID::RUBY_RESCUE,
    LocationID::YOU_RESCUE,
    LocationID::MARI_RESCUE,
    LocationID::RIKO_RESCUE,
    LocationID::HANAMARU_RESCUE,
};

std::set<LocationID> upgrade_quest_locations = {
    LocationID::CHIKA_UPGRADE_QUEST,
    LocationID::RIKO_UPGRADE_QUEST,
    LocationID::KANAN_UPGRADE_QUEST,
    LocationID::HANAMARU_UPGRADE_QUEST,
    LocationID::RUBY_UPGRADE_QUEST,
    LocationID::YOU_UPGRADE_QUEST,
    LocationID::DIA_UPGRADE_QUEST,
    LocationID::MARI_UPGRADE_QUEST,
};

std::set<LocationID> boss_locations = {
    LocationID::SUNKEN_TEMPLE_BOSS,
    LocationID::RUINS_BOSS_1,
    LocationID::RUINS_BOSS_2,
    LocationID::RUINS_BOSS_3,
    LocationID::GROTTO_BOSS,
    LocationID::CORAL_HILL_BOSS,
    LocationID::SEA_OF_TREES_BOSS,
    LocationID::CRYSTALLINE_GROTTO_BOSS,
    LocationID::SUNKEN_VOLCANO_BOSS,
    LocationID::SHIPWRECK_BOSS,
    LocationID::INFERNAL_ALTAR_BOSS,
};

std::set<LocationID> boss_refight_locations = {
    LocationID::SUNKEN_TEMPLE_BOSS_REFIGHT,
    LocationID::RUINS_BOSS_REFIGHT,
    LocationID::GROTTO_BOSS_REFIGHT,
    LocationID::CORAL_HILL_BOSS_REFIGHT,
    LocationID::SEA_OF_TREES_BOSS_REFIGHT,
    LocationID::CRYSTALLINE_GROTTO_BOSS_REFIGHT,
    LocationID::SUNKEN_VOLCANO_BOSS_REFIGHT,
    LocationID::SHIPWRECK_BOSS_REFIGHT,
    LocationID::INFERNAL_ALTAR_BOSS_REFIGHT,
};

std::set<LocationID> chest_locations = {
    LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_LEFT_CHEST,
    LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_RIGHT_CHEST,
    LocationID::SUNKEN_TEMPLE_FISHY_ARCHERY_CHEST,
    LocationID::SUNKEN_TEMPLE_PATHWAY_TO_INFERNAL_ALTAR_CHEST,
    LocationID::SUNKEN_TEMPLE_KATYS_MASK_CHEST,
    LocationID::SUNKEN_TEMPLE_CHIKA_TESTING_GROUNDS_CHEST,
    LocationID::GROTTO_FIRST_SAVE_ROOM_CHEST,
    LocationID::GROTTO_FIRST_WATTERFALL_CHEST,
    LocationID::GROTTO_FIRST_LAKE_CHEST,
    LocationID::GROTTO_SECOND_LAKE_CHEST,
    LocationID::GROTTO_SPELLBOOK_CHEST,
    LocationID::GROTTO_LONG_WATTERFALL_CHEST,
    LocationID::GROTTO_ISOLATED_CLIMB_CHEST,
    LocationID::GROTTO_CAVE_CLIMB_CHEST,
    LocationID::RUINS_SANDY_TRAP_CHEST,
    LocationID::RUINS_VERTICAL_POISON_CHEST,
    LocationID::RUINS_ROLLING_ROCKS_CHEST,
    LocationID::RUINS_LAPTOP_CHEST,
    LocationID::RUINS_HALL_OF_SHAME_CHEST,
    LocationID::SUNKEN_VOLCANO_FIRST_SAVE_ROOM_CHEST,
    LocationID::SUNKEN_VOLCANO_HOTSPRING_CHEST,
    LocationID::SUNKEN_VOLCANO_SOARSHOES_CHEST,
    LocationID::SUNKEN_VOLCANO_SOARSHOES_OBLIGATORY_CHEST,
    LocationID::SUNKEN_VOLCANO_TONOSAMA_PARTS_CHEST,
    LocationID::SHIPWRECK_SEALED_OFF_CHEST,
    LocationID::SHIPWRECK_SPIKEY_BALL_FISH_CHEST,
    LocationID::SHIPWRECK_FINAL_GUARD_CHEST,
    LocationID::SHIPWRECK_GLOVES_OF_MIGHT_CHEST,
    LocationID::SHIPWRECK_POSTAL_GUILD_BAG_CHEST,
    LocationID::CORAL_HILL_SHOARSHOESNT_CHEST,
    LocationID::CORAL_HILL_TELEPORTING_FISH_CHEST,
    LocationID::CORAL_HILL_WALLCRAB_CHEST,
    LocationID::CORAL_HILL_CHIKA_BLOCK_CHEST,
    LocationID::CORAL_HILL_LOST_MONSTIE_CHEST,
    LocationID::CRYSTALLINE_GROTTO_ONE_WAY_SLIDE_CHEST,
    LocationID::CRYSTALLINE_GROTTO_GIANT_CRYSTAL_CHEST,
    LocationID::CRYSTALLINE_GROTTO_ISOLATED_CHEST,
    LocationID::CRYSTALLINE_GROTTO_CUTE_POCHETTE_CHEST,
    LocationID::CRYSTALLINE_GROTTO_MARI_ISSUE_CHEST,
    LocationID::SEA_OF_TREES_POISON_CRAB_CHEST,
    LocationID::SEA_OF_TREES_SCARLET_DELTA_SUIT_CHEST,
    LocationID::SEA_OF_TREES_GOLDEN_SNAIL_CHEST,
    LocationID::SEA_OF_TREES_SLOPE_ROOM_CHEST,
    LocationID::SEA_OF_TREES_YOU_TESTING_GROUNDS_CHEST,
    LocationID::INFERNAL_ALTAR_PURPLE_GOO_CHEST,
    LocationID::INFERNAL_ALTAR_DARK_ROOM_CHEST,
};

std::set<LocationID> crafting_locations = {
    LocationID::RECIPE_01,
    LocationID::RECIPE_02,
    LocationID::RECIPE_03,
    LocationID::RECIPE_04,
    LocationID::RECIPE_05,
    LocationID::RECIPE_06,
    LocationID::RECIPE_07,
    LocationID::RECIPE_08,
    LocationID::RECIPE_09,
    LocationID::RECIPE_10,
    LocationID::RECIPE_11,
    LocationID::RECIPE_12,
    LocationID::RECIPE_13,
    LocationID::RECIPE_14,
    LocationID::RECIPE_15,
    LocationID::RECIPE_16,
    LocationID::RECIPE_17,
    LocationID::RECIPE_18,
    LocationID::RECIPE_19,
    LocationID::RECIPE_20,
    LocationID::RECIPE_21,
    LocationID::RECIPE_22,
    LocationID::RECIPE_23,
    LocationID::RECIPE_24,
    LocationID::RECIPE_25,
    LocationID::RECIPE_26,
    LocationID::RECIPE_27,
    LocationID::RECIPE_28,
    LocationID::RECIPE_29,
    LocationID::RECIPE_30,
    LocationID::RECIPE_31,
    LocationID::RECIPE_32,
    LocationID::RECIPE_33,
    LocationID::RECIPE_34,
    LocationID::RECIPE_35,
    LocationID::RECIPE_36,
    LocationID::RECIPE_37,
    LocationID::RECIPE_38,
    LocationID::RECIPE_39,
    LocationID::RECIPE_40,
    LocationID::RECIPE_41,
    LocationID::RECIPE_42,
    LocationID::RECIPE_43,
    LocationID::RECIPE_44,
    LocationID::RECIPE_45,
    LocationID::RECIPE_46,
    LocationID::RECIPE_47,
    LocationID::RECIPE_48,
    LocationID::RECIPE_49,
    LocationID::RECIPE_50,
    LocationID::RECIPE_51,
    LocationID::RECIPE_52,
    LocationID::RECIPE_53,
    LocationID::RECIPE_54,
    LocationID::RECIPE_55,
    LocationID::RECIPE_56,
    LocationID::RECIPE_57,
    LocationID::RECIPE_58,
    LocationID::RECIPE_59,
    LocationID::RECIPE_60,
    LocationID::RECIPE_61,
    LocationID::RECIPE_62,
    LocationID::RECIPE_63,
    LocationID::RECIPE_64,
    LocationID::RECIPE_65,
    LocationID::RECIPE_66,
    LocationID::RECIPE_67,
    LocationID::RECIPE_68,
    LocationID::RECIPE_69,
    LocationID::RECIPE_70,
    LocationID::RECIPE_71,
    LocationID::RECIPE_72,
    LocationID::RECIPE_73,
    LocationID::RECIPE_74,
    LocationID::RECIPE_75,
    LocationID::RECIPE_76,
    LocationID::RECIPE_77,
    LocationID::RECIPE_78,
    LocationID::RECIPE_79,
    LocationID::RECIPE_80,
    LocationID::RECIPE_81,
    LocationID::RECIPE_82,
    LocationID::RECIPE_83,
    LocationID::RECIPE_84,
    LocationID::RECIPE_85,
    LocationID::RECIPE_86,
    LocationID::RECIPE_87,
    LocationID::RECIPE_88,
    LocationID::RECIPE_89,
    LocationID::RECIPE_90,
    LocationID::RECIPE_91,
    LocationID::RECIPE_92,
    LocationID::RECIPE_93,
};

// location_id -> <offset, mask>
std::unordered_map<LocationID, std::pair<uint16_t, uint8_t>> chest_flags = {
    {LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_LEFT_CHEST, {0x147, 0x20}},
    {LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_RIGHT_CHEST, {0x147, 0x40}},
    {LocationID::SUNKEN_TEMPLE_KATYS_MASK_CHEST, {0x147, 0x80}},
    {LocationID::RUINS_LAPTOP_CHEST, {0x14B, 0x20}},
    {LocationID::RUINS_HALL_OF_SHAME_CHEST, {0x14B, 0x40}},
    {LocationID::RUINS_SANDY_TRAP_CHEST, {0x14B, 0x80}},
    {LocationID::GROTTO_CAVE_CLIMB_CHEST, {0x14F, 0x20}},
    {LocationID::GROTTO_ISOLATED_CLIMB_CHEST, {0x14F, 0x40}},
    {LocationID::GROTTO_LONG_WATTERFALL_CHEST, {0x14F, 0x80}},
    {LocationID::CORAL_HILL_LOST_MONSTIE_CHEST, {0x153, 0x20}},
    {LocationID::CORAL_HILL_SHOARSHOESNT_CHEST, {0x153, 0x80}},
    {LocationID::SEA_OF_TREES_POISON_CRAB_CHEST, {0x157, 0x40}},
    {LocationID::SEA_OF_TREES_SCARLET_DELTA_SUIT_CHEST, {0x157, 0x80}},
    {LocationID::CRYSTALLINE_GROTTO_ONE_WAY_SLIDE_CHEST, {0x15B, 0x20}},
    {LocationID::CRYSTALLINE_GROTTO_GIANT_CRYSTAL_CHEST, {0x15B, 0x40}},
    {LocationID::CRYSTALLINE_GROTTO_MARI_ISSUE_CHEST, {0x15B, 0x80}},
    {LocationID::SHIPWRECK_POSTAL_GUILD_BAG_CHEST, {0x163, 0x40}},
    {LocationID::SHIPWRECK_GLOVES_OF_MIGHT_CHEST, {0x163, 0x80}},
    {LocationID::INFERNAL_ALTAR_DARK_ROOM_CHEST, {0x167, 0x20}},
    {LocationID::INFERNAL_ALTAR_PURPLE_GOO_CHEST, {0x167, 0x40}},

    {LocationID::SUNKEN_TEMPLE_CHIKA_TESTING_GROUNDS_CHEST, {0x194, 0x01}},
    {LocationID::SUNKEN_TEMPLE_FISHY_ARCHERY_CHEST, {0x194, 0x02}},
    {LocationID::SUNKEN_TEMPLE_PATHWAY_TO_INFERNAL_ALTAR_CHEST, {0x194, 0x04}},
    {LocationID::RUINS_ROLLING_ROCKS_CHEST, {0x198, 0x01}},
    {LocationID::RUINS_VERTICAL_POISON_CHEST, {0x198, 0x02}},
    {LocationID::GROTTO_SPELLBOOK_CHEST, {0x19C, 0x01}},
    {LocationID::GROTTO_FIRST_WATTERFALL_CHEST, {0x19C, 0x02}},
    {LocationID::GROTTO_FIRST_LAKE_CHEST, {0x19C, 0x40}},
    {LocationID::GROTTO_FIRST_SAVE_ROOM_CHEST, {0x19C, 0x80}},
    {LocationID::GROTTO_SECOND_LAKE_CHEST, {0x19D, 0x01}},
    {LocationID::CORAL_HILL_CHIKA_BLOCK_CHEST, {0x1A0, 0x01}},
    {LocationID::CORAL_HILL_WALLCRAB_CHEST, {0x1A0, 0x08}},
    {LocationID::CORAL_HILL_TELEPORTING_FISH_CHEST, {0x1A0, 0x10}},
    {LocationID::SEA_OF_TREES_SLOPE_ROOM_CHEST, {0x1A4, 0x01}},
    {LocationID::SEA_OF_TREES_GOLDEN_SNAIL_CHEST, {0x1A4, 0x04}},
    {LocationID::SEA_OF_TREES_YOU_TESTING_GROUNDS_CHEST, {0x1A4, 0x08}},
    {LocationID::CRYSTALLINE_GROTTO_ISOLATED_CHEST, {0x1A8, 0x01}},
    {LocationID::CRYSTALLINE_GROTTO_CUTE_POCHETTE_CHEST, {0x1A8, 0x02}},
    {LocationID::SUNKEN_VOLCANO_SOARSHOES_CHEST, {0x1AD, 0x04}},
    {LocationID::SUNKEN_VOLCANO_TONOSAMA_PARTS_CHEST, {0x1AD, 0x08}},
    {LocationID::SUNKEN_VOLCANO_SOARSHOES_OBLIGATORY_CHEST, {0x1AD, 0x10}},
    {LocationID::SUNKEN_VOLCANO_FIRST_SAVE_ROOM_CHEST, {0x1AD, 0x20}},
    {LocationID::SUNKEN_VOLCANO_HOTSPRING_CHEST, {0x1AD, 0x40}},
    {LocationID::SHIPWRECK_FINAL_GUARD_CHEST, {0x1B0, 0x01}},
    {LocationID::SHIPWRECK_SEALED_OFF_CHEST, {0x1B0, 0x08}},
    {LocationID::SHIPWRECK_SPIKEY_BALL_FISH_CHEST, {0x1B0, 0x10}},
};

std::unordered_map<Area, std::unordered_map<uint8_t, LocationID>> chest_data =
{
    {
        Area::SUNKEN_TEMPLE, 
        {
            {1, LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_LEFT_CHEST},
            {2, LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_RIGHT_CHEST},
            {3, LocationID::SUNKEN_TEMPLE_KATYS_MASK_CHEST},
            {4, LocationID::SUNKEN_TEMPLE_CHIKA_TESTING_GROUNDS_CHEST},
            {5, LocationID::SUNKEN_TEMPLE_FISHY_ARCHERY_CHEST},
            {6, LocationID::SUNKEN_TEMPLE_PATHWAY_TO_INFERNAL_ALTAR_CHEST},
        }
    },
    {
        Area::RUINS,
        {
            {1, LocationID::RUINS_LAPTOP_CHEST},
            {2, LocationID::RUINS_HALL_OF_SHAME_CHEST},
            {3, LocationID::RUINS_SANDY_TRAP_CHEST},
            {4, LocationID::RUINS_ROLLING_ROCKS_CHEST},
            {5, LocationID::RUINS_VERTICAL_POISON_CHEST},
        }
    },
    {
        Area::GROTTO,
        {
            {1, LocationID::GROTTO_CAVE_CLIMB_CHEST},
            {2, LocationID::GROTTO_ISOLATED_CLIMB_CHEST},
            {3, LocationID::GROTTO_LONG_WATTERFALL_CHEST},
            {4, LocationID::GROTTO_SPELLBOOK_CHEST},
            {5, LocationID::GROTTO_FIRST_WATTERFALL_CHEST},
            {10, LocationID::GROTTO_FIRST_LAKE_CHEST},
            {11, LocationID::GROTTO_FIRST_SAVE_ROOM_CHEST},
            {12, LocationID::GROTTO_SECOND_LAKE_CHEST},
        }
    },
    {
        Area::CORAL_HILL,
        {
            {1, LocationID::CORAL_HILL_LOST_MONSTIE_CHEST},
            {3, LocationID::CORAL_HILL_SHOARSHOESNT_CHEST},
            {4, LocationID::CORAL_HILL_CHIKA_BLOCK_CHEST},
            {7, LocationID::CORAL_HILL_WALLCRAB_CHEST},
            {8, LocationID::CORAL_HILL_TELEPORTING_FISH_CHEST},
        }
    },
    {
        Area::SEA_OF_TREES,
        {
            {2, LocationID::SEA_OF_TREES_POISON_CRAB_CHEST},
            {3, LocationID::SEA_OF_TREES_SCARLET_DELTA_SUIT_CHEST},
            {4, LocationID::SEA_OF_TREES_SLOPE_ROOM_CHEST},
            {6, LocationID::SEA_OF_TREES_GOLDEN_SNAIL_CHEST},
            {7, LocationID::SEA_OF_TREES_YOU_TESTING_GROUNDS_CHEST},
        }
    },
    {
        Area::CRYSTALLINE_GROTTO,
        {
            {1, LocationID::CRYSTALLINE_GROTTO_ONE_WAY_SLIDE_CHEST},
            {2, LocationID::CRYSTALLINE_GROTTO_GIANT_CRYSTAL_CHEST},
            {3, LocationID::CRYSTALLINE_GROTTO_MARI_ISSUE_CHEST},
            {4, LocationID::CRYSTALLINE_GROTTO_ISOLATED_CHEST},
            {5, LocationID::CRYSTALLINE_GROTTO_CUTE_POCHETTE_CHEST},
        }
    },
    {
        Area::SUNKEN_VOLCANO,
        {
            {14, LocationID::SUNKEN_VOLCANO_SOARSHOES_CHEST},
            {15, LocationID::SUNKEN_VOLCANO_TONOSAMA_PARTS_CHEST},
            {16, LocationID::SUNKEN_VOLCANO_SOARSHOES_OBLIGATORY_CHEST},
            {17, LocationID::SUNKEN_VOLCANO_FIRST_SAVE_ROOM_CHEST},
            {18, LocationID::SUNKEN_VOLCANO_HOTSPRING_CHEST},
        }
    },
    {
        Area::INFERNAL_ALTAR,
        {
            {1, LocationID::INFERNAL_ALTAR_DARK_ROOM_CHEST},
            {2, LocationID::INFERNAL_ALTAR_PURPLE_GOO_CHEST},
        }
    },
};

std::unordered_map<LocationID, uint32_t> boss_data = {
    {LocationID::SUNKEN_TEMPLE_BOSS, 0x20},
    {LocationID::RUINS_BOSS_1, 0x40},
    {LocationID::RUINS_BOSS_2, 0x80},
    {LocationID::RUINS_BOSS_3, 0x0100},
    {LocationID::GROTTO_BOSS, 0x0200},
    {LocationID::CORAL_HILL_BOSS, 0x0400},
    {LocationID::SEA_OF_TREES_BOSS, 0x0800},
    {LocationID::CRYSTALLINE_GROTTO_BOSS, 0x1000},
    {LocationID::SUNKEN_VOLCANO_BOSS, 0x2000},
    {LocationID::SHIPWRECK_BOSS, 0x4000},
    {LocationID::INFERNAL_ALTAR_BOSS, 0x8000},

    {LocationID::SUNKEN_TEMPLE_BOSS_REFIGHT, 0x010000},
    {LocationID::RUINS_BOSS_REFIGHT, 0x020000},
    {LocationID::GROTTO_BOSS_REFIGHT, 0x040000},
    {LocationID::CORAL_HILL_BOSS_REFIGHT, 0x080000},
    {LocationID::SEA_OF_TREES_BOSS_REFIGHT, 0x100000},
    {LocationID::CRYSTALLINE_GROTTO_BOSS_REFIGHT, 0x200000},
    {LocationID::SUNKEN_TEMPLE_BOSS_REFIGHT, 0x400000},
    {LocationID::SHIPWRECK_BOSS_REFIGHT, 0x800000},
    {LocationID::INFERNAL_ALTAR_BOSS_REFIGHT, 0x01000000},
};

std::unordered_map<LocationID, uint32_t> character_rescue_flags = {
    {LocationID::CHIKA_RESCUE, 0x80},
    {LocationID::KANAN_RESCUE, 0x0100},
    {LocationID::DIA_RESCUE, 0x0200},
    {LocationID::RUBY_RESCUE, 0x0400},
    {LocationID::YOU_RESCUE, 0x0800},
    {LocationID::MARI_RESCUE, 0x1000},
    {LocationID::RIKO_RESCUE, 0x2000},
    {LocationID::HANAMARU_RESCUE, 0x4000},
};

std::unordered_map<LocationID, uint32_t> character_upgrade_flags = {
    {LocationID::CHIKA_UPGRADE_QUEST, 0x200},
    {LocationID::RIKO_UPGRADE_QUEST, 0x1000},
    {LocationID::KANAN_UPGRADE_QUEST, 0x8000},
    {LocationID::HANAMARU_UPGRADE_QUEST, 0x40000},
    {LocationID::RUBY_UPGRADE_QUEST, 0x200000},
    {LocationID::YOU_UPGRADE_QUEST, 0x1000000},
    {LocationID::DIA_UPGRADE_QUEST, 0x8000000},
    {LocationID::MARI_UPGRADE_QUEST, 0x40000000},
};

FuncPointer(bool, OpenChest, (void*, int64_t, uint64_t*, uint64_t), (0x047c220 + base));
std::shared_ptr<FunctionHook<bool, void*, int64_t, uint64_t*, uint64_t>> on_chest_opened;
FuncPointer(bool, CraftRecipe, (void*, uint64_t*, uint64_t*), (0x06a2990 + base));
std::shared_ptr<FunctionHook<bool, void*, uint64_t*, uint64_t*>> on_craft_recipe;

bool OnOpenChestOverride(void* chest_work, int64_t param_1, uint64_t* param_2, uint64_t param_3)
{
    if (APManager::getInstance().IsRunning())
    {
        if (*(uint32_t*)(param_2 + 5) < 10)
        {
            uint32_t** struct_0x5f8 = (uint32_t**)((uint64_t)chest_work + 0x5f8);
            if (*struct_0x5f8 != nullptr)
            {
                (*struct_0x5f8)[0] = 0;
                (*struct_0x5f8)[1] = 0;
            }
            uint32_t chest_item_id = *(uint32_t*)((uint64_t)chest_work + 0x234);
            int32_t chest_id = *(int32_t*)((uint64_t)chest_work + 0x238);
            LocationID location_id = chest_data[main_data->current_save.area][chest_id];
            if ((int64_t)location_id != 0 && !APManager::getInstance().IsLocationChecked(location_id))
            {
                *(uint32_t*)((uint64_t)chest_work + 0x234) = (uint32_t)APManager::getInstance().GetLocationItemData()[(int64_t)location_id].id;
                helperFunctions.log_debug("Found %d", *(uint32_t*)((uint64_t)chest_work + 0x234));
                if (*(int32_t*)((uint64_t)chest_work + 0x234) >= 0)
                    ItemManager::getInstance().AddProcessedItem(chest_item_id);
                APManager::getInstance().CheckLocation(location_id);
            }
        }
    }
    return on_chest_opened->CallOriginal(chest_work, param_1, param_2, param_3);
}

bool OnCraftRecipeOverride(void* work, uint64_t* param_1, uint64_t* param_2)
{
    uint64_t* puVar2 = (uint64_t*)param_2[3];
    if ((*(uint32_t*)(param_2 + 5)) == 0xd && (puVar2[7] == 0) && APManager::getInstance().IsRunning() && APManager::getInstance().CraftSanityEnabled())
    {
        int64_t lVar19 = *(int64_t*)((uint64_t)work + 0x568 + 0x8);
        if (lVar19 != 0 && (*(int64_t*)(lVar19 + 0x160) != 0) && ((*(int32_t*)(base + 0x11632cc) != 1)))
        {
            lVar19 = *(int64_t*)(lVar19 + 0x178);
            if (lVar19 != 0)
            {
                int64_t lVar26 = *(int64_t*)(lVar19 + 0x570);
                int64_t recipe_id = *(uint32_t*)((*(int64_t*)(*(int64_t*)(lVar26 + 0x540) + (int64_t)(*(int32_t*)(lVar26 + 0x558)) * 8)) + 0x2C);
                int32_t result = (item_create_db.data + recipe_id)->result_id;
                if ((result < (int64_t)LocationID::RECIPE_01 || result >(int64_t)LocationID::RECIPE_93))
                {
                    ItemManager::getInstance().AddProcessedItem(recipe_id + 700);
                    main_data->current_save.inventory[recipe_id + 700].data.count = 1;
                }
                APManager::getInstance().CheckLocation((LocationID)(recipe_id + 700));
            }
        }
    }
    return on_craft_recipe->CallOriginal(work, param_1, param_2);
}

void LocationManager::Init(const char* path)
{
    if (on_chest_opened == nullptr)
    {
        on_chest_opened = std::make_shared<FunctionHook<bool, void*, int64_t, uint64_t*, uint64_t>>(OpenChest);
        on_chest_opened->Hook(OnOpenChestOverride);
    }
    if (on_craft_recipe == nullptr)
    {
        on_craft_recipe = std::make_shared<FunctionHook<bool, void*, uint64_t*, uint64_t*>>(CraftRecipe);
        on_craft_recipe->Hook(OnCraftRecipeOverride);
    }
}

void LocationManager::OnConnect()
{
    OnFrameCheckChests();
    OnFrameCheckRecipes();
}

void LocationManager::OnFrame(double delta)
{
    if (main_data->saving)
    {
        OnFrameCheckChests();
        OnFrameCheckRecipes();
    }
    OnFrameCheckBosses();
    OnFrameCheckRescues();
    OnFrameCheckQuests();
}

void LocationManager::OnLocationChecked(int64_t location_id)
{
    if (location_id >= (uint64_t)LocationID::SUNKEN_TEMPLE_CAST_TUTORIAL_LEFT_CHEST && location_id <= (uint64_t)LocationID::CHEST_ID_MAX)
    {
        auto &data = chest_flags[(LocationID)location_id];
        main_data->current_save.flags[data.first] |= data.second;
    }
    if (location_id >= (uint64_t)LocationID::SUNKEN_TEMPLE_BOSS && location_id <= (uint64_t)LocationID::INFERNAL_ALTAR_BOSS_REFIGHT)
    {
        auto& data = boss_data[(LocationID)location_id];
        main_data->current_save.bosses_defeated |= data;
    }
    if (location_id >= (uint64_t)LocationID::RECIPE_01 && location_id <= (uint64_t)LocationID::RECIPE_93)
    {
        main_data->current_save.inventory[location_id].data.count = 1;
    }
    if (location_id >= (uint64_t)LocationID::CHIKA_RESCUE && location_id <= (uint64_t)LocationID::HANAMARU_RESCUE)
    {
        main_data->current_save.progression_flags |= (((uint64_t)character_rescue_flags[(LocationID)location_id]) << 8);
    }
}

std::set<int64_t> LocationManager::GetActiveLocations() 
{
    std::set<int64_t> out;
    for (auto& l : rescue_locations)
    {
        out.emplace((int64_t)l);
    }
    for (auto& l : upgrade_quest_locations)
    {
        out.emplace((int64_t)l);
    }
    for (auto& l : boss_locations)
    {
        out.emplace((int64_t)l);
    }
    for (auto& l : boss_refight_locations)
    {
        out.emplace((int64_t)l);
    }
    for (auto& l : chest_locations)
    {
        out.emplace((int64_t)l);
    }
    if (APManager::getInstance().CraftSanityEnabled())
    {
        for (auto& l : crafting_locations)
        {
            out.emplace((int64_t)l);
        }
    }
    return out;
}

void LocationManager::OnFrameCheckChests()
{
    for (auto& chest : chest_flags)
    {
        auto data = chest.second;
        if ((main_data->current_save.flags[data.first] & data.second) != 0)
        {
            APManager::getInstance().CheckLocation(chest.first);
        }
    }
}

void LocationManager::OnFrameCheckBosses()
{
    for (auto& boss : boss_data)
    {
        if ((main_data->current_save.bosses_defeated & boss.second) != 0)
        {
            APManager::getInstance().CheckLocation(boss.first);
        }
    }
}

void LocationManager::OnFrameCheckRescues()
{
    for (auto& flags : character_rescue_flags)
    {
        if (APManager::getInstance().IsLocationChecked(flags.first))
            main_data->current_save.progression_flags |= ((uint64_t)flags.second << 8);
        else if ((main_data->current_save.progression_flags & ((uint64_t)flags.second << 8)) != 0)
            APManager::getInstance().CheckLocation(flags.first);
    }
}

void LocationManager::OnFrameCheckQuests()
{
    for (auto& flags : character_upgrade_flags)
    {
        if (APManager::getInstance().IsLocationChecked(flags.first))
            main_data->current_save.progression_flags |= ((uint64_t)flags.second << 24);
        else if ((main_data->current_save.progression_flags & ((uint64_t)flags.second << 24)) != 0)
            APManager::getInstance().CheckLocation(flags.first);
    }
}

void LocationManager::OnFrameCheckRecipes()
{
    for (uint32_t i = 0; i < item_create_db.stats->max; i++)
    {
        if (main_data->current_save.inventory[701 + i].data.count != 0)
        {
            APManager::getInstance().CheckLocation((LocationID)(701 + i));
        }
    }
}
