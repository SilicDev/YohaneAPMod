#include "Items.h"

#include <memory>

#include "Archipelago.h"

#include "../ModLoader/FunctionHook.h"
#include "../ModLoader/ModInfo.h"
#include "../YOHANE_BID/Functions.h"
#include "../YOHANE_BID/HelperFunctions.h"
#include "../YOHANE_BID/Variables.h"

extern HelperFunctions helperFunctions;

std::shared_ptr<FunctionHook<int32_t, Item[1000], uint32_t, int32_t>> set_item_count;
std::shared_ptr<FunctionHook<char*, ItemDBEntry*>> find_item_name;
std::shared_ptr<FunctionHook<char*, ItemDBEntry*>> find_item_description;
std::shared_ptr<FunctionHook<char, void*>> get_equipment_slots;

std::unordered_map<ItemID, std::pair<ItemID, ItemID>> progressive_character_map = {
    {ItemID::PROGRESSIVE_CHIKA,{ItemID::CHIKA, ItemID::CHIKA_UPGRADE}},
    {ItemID::PROGRESSIVE_RIKO,{ItemID::RIKO, ItemID::RIKO_UPGRADE}},
    {ItemID::PROGRESSIVE_KANAN,{ItemID::KANAN, ItemID::KANAN_UPGRADE}},
    {ItemID::PROGRESSIVE_DIA,{ItemID::DIA, ItemID::DIA_UPGRADE}},
    {ItemID::PROGRESSIVE_YOU,{ItemID::YOU, ItemID::YOU_UPGRADE}},
    {ItemID::PROGRESSIVE_MARI,{ItemID::MARI, ItemID::MARI_UPGRADE}},
    {ItemID::PROGRESSIVE_HANAMARU,{ItemID::HANAMARU, ItemID::HANAMARU_UPGRADE}},
    {ItemID::PROGRESSIVE_RUBY,{ItemID::RUBY, ItemID::RUBY_UPGRADE}},
};

std::unordered_map<ItemID, ItemID> character_upgrade_map = {
    {ItemID::CHIKA, ItemID::CHIKA_UPGRADE},
    {ItemID::RIKO, ItemID::RIKO_UPGRADE},
    {ItemID::KANAN, ItemID::KANAN_UPGRADE},
    {ItemID::DIA, ItemID::DIA_UPGRADE},
    {ItemID::YOU, ItemID::YOU_UPGRADE},
    {ItemID::MARI, ItemID::MARI_UPGRADE},
    {ItemID::HANAMARU, ItemID::HANAMARU_UPGRADE},
    {ItemID::RUBY, ItemID::RUBY_UPGRADE},
};

std::unordered_map<ItemID, std::pair<uint32_t, uint32_t>> character_item_flags =
{
    {ItemID::LAILAPS, {0x20, 0x0}},
    {ItemID::CHIKA, {0x80, 0x80}},
    {ItemID::RIKO, {0x200, 0x400}},
    {ItemID::KANAN, {0x800, 0x2000}},
    {ItemID::DIA, {0x2000, 0x2000000}},
    {ItemID::YOU, {0x8000, 0x400000}},
    {ItemID::MARI, {0x20000, 0x10000000}},
    {ItemID::HANAMARU, {0x80000, 0x10000}},
    {ItemID::RUBY, {0x200000, 0x80000}},
};

struct UpgradeData
{
    Area area = Area::SUNKEN_TEMPLE;
    std::set<uint8_t> rooms;
    LocationID location = LocationID::NONE;
};

std::unordered_map<ItemID, UpgradeData> upgrade_data = {
    {ItemID::CHIKA_UPGRADE, {Area::SUNKEN_TEMPLE, {11, 12}, LocationID::SUNKEN_TEMPLE_KATYS_MASK_CHEST}},
    {ItemID::RIKO_UPGRADE, {Area::GROTTO, {25, 27, 28}, LocationID::GROTTO_SPELLBOOK_CHEST}},
    {ItemID::KANAN_UPGRADE, {Area::SUNKEN_VOLCANO, {54, 55, 56}, LocationID::SUNKEN_VOLCANO_TONOSAMA_PARTS_CHEST}},
    {ItemID::DIA_UPGRADE, {Area::SEA_OF_TREES, {22, 23}, LocationID::SEA_OF_TREES_SCARLET_DELTA_SUIT_CHEST}},
    {ItemID::YOU_UPGRADE, {Area::SHIPWRECK, {16, 17, 18}, LocationID::SHIPWRECK_POSTAL_GUILD_BAG_CHEST}},
    {ItemID::MARI_UPGRADE, {Area::CORAL_HILL, {8, 9, 10}, LocationID::CORAL_HILL_LOST_MONSTIE_CHEST}},
    {ItemID::HANAMARU_UPGRADE, {Area::RUINS, {19, 24}, LocationID::RUINS_LAPTOP_CHEST}},
    {ItemID::RUBY_UPGRADE, {Area::CRYSTALLINE_GROTTO, {40, 41, 42}, LocationID::CRYSTALLINE_GROTTO_CUTE_POCHETTE_CHEST}},
};

std::unordered_map<ItemID, const char*> item_names = {
    {ItemID::FALLEN_ANGELS_SOARSHOES, "Fallen Angel's Soarshoes" },
    {ItemID::GLOVES_OF_MIGHT, "Gloves of Might" },
    {ItemID::SEA_DEITYS_CHARM, "Sea Deity's Charm" },
    {ItemID::FASHION_FOR_DUMMIES, "Fashion for Dummies"},
    {ItemID::FASHION_FOR_DUMMIES_2, "Fashion for Dummies"},
    {ItemID::LAILAPS, "Lailaps"},
    {ItemID::CHIKA, "Chika"},
    {ItemID::RIKO, "Riko"},
    {ItemID::KANAN, "Kanan"},
    {ItemID::DIA, "Dia"},
    {ItemID::YOU, "You"},
    {ItemID::MARI, "Mari"},
    {ItemID::HANAMARU, "Hanamaru"},
    {ItemID::RUBY, "Ruby"},
    {ItemID::BOSS_TOKEN, "Boss Token"},
    {ItemID::PROGRESSIVE_CHIKA, "Progressive Chika"},
    {ItemID::PROGRESSIVE_RIKO, "Progressive Riko"},
    {ItemID::PROGRESSIVE_KANAN, "Progressive Kanan"},
    {ItemID::PROGRESSIVE_DIA, "Progressive Dia"},
    {ItemID::PROGRESSIVE_YOU, "Progressive You"},
    {ItemID::PROGRESSIVE_MARI, "Progressive Mari"},
    {ItemID::PROGRESSIVE_HANAMARU, "Progressive Hanamaru"},
    {ItemID::PROGRESSIVE_RUBY, "Progressive Ruby"},
    {ItemID::SMALL_YEN, "10000 Yen"},
    {ItemID::MEDIUM_YEN, "25000 Yen"},
    {ItemID::LARGE_YEN, "50000 Yen"},
};

std::unordered_map<ItemID, std::string> item_descriptions = {
    {ItemID::CHIKA_UPGRADE, ItemManager::FormatDescription("A familiar mask. Didn't Chika say she lost hers?")},
    {ItemID::RIKO_UPGRADE, ItemManager::FormatDescription("A mysterious spellbook detailing the use of strong fire magic. Too bad Yohane can't read it, maybe Riko could help?")},
    {ItemID::KANAN_UPGRADE, ItemManager::FormatDescription("An assortment of mechanical parts. Kanan might be able to repair Tonosama with these!")},
    {ItemID::HANAMARU_UPGRADE, ItemManager::FormatDescription("A standard laptop. Hopefully this will help Zuramaru expand her bakery buisiness.")},
    {ItemID::RUBY_UPGRADE, ItemManager::FormatDescription("A fluffy little bag. Ruby will love this!")},
    {ItemID::YOU_UPGRADE, ItemManager::FormatDescription("An old worn out bag of the postal guild. You was looking for hers.")},
    {ItemID::DIA_UPGRADE, ItemManager::FormatDescription("Dia's combat suit. Make sure to return it!")},
    {ItemID::MARI_UPGRADE, ItemManager::FormatDescription("A lost monstie. It must be looking for Mari.")},
    {ItemID::FASHION_FOR_DUMMIES, ItemManager::FormatDescription("An easy to digest guide for better accessorizing.")},
    {ItemID::FASHION_FOR_DUMMIES_2, ItemManager::FormatDescription("An easy to digest guide for better accessorizing.")},
    {ItemID::WHALE_HARP, ItemManager::FormatDescription("A harp fashioned to look like a whale. It is said its sound can soothe everyone")},
    //{ItemID::SHARK_RIP, ItemManager::FormatDescription("")},
    {ItemID::ICHIMONJI_SCABBARD, ItemManager::FormatDescription("The sheath of a great blade, said to defeat enemies in a single strike.")},
    {ItemID::RIPPLE_SHELL, ItemManager::FormatDescription("A shell with ripple patterns engraved on it.")},
    {ItemID::WORLD_PINETREE_LUMBER, ItemManager::FormatDescription("Wood imbued with the magic of the world tree.")},
    {ItemID::PHANTOM_JEWEL, ItemManager::FormatDescription("A gemstone that seems to only partially reside in this realm.")},
    {ItemID::RUSTY_ROD, ItemManager::FormatDescription("A rusty piece of metal. Polished up it could still make for a good weapon.")},
    {ItemID::LUNAR_GRINDSTONE, ItemManager::FormatDescription("A grindstone made from celestial material.")},
    {ItemID::TWINKLING_STARDUST, ItemManager::FormatDescription("Stardust from a comet filled with small amounts of magical energy.")},
    {ItemID::LUCENT_MATTER, ItemManager::FormatDescription("A glowing rock filled with small amounts of magical energy.")},
    {ItemID::PRINCESSS_DIARY, ItemManager::FormatDescription("The diary of a princess from a far away land.")},
    {ItemID::NUMAZU_STAR, ItemManager::FormatDescription("A rare stone formation ressembling a star that can only be found around Numazu.")},
    {ItemID::TORN_COLLAR, ItemManager::FormatDescription("A wornout collar. Maybe with a bit of handy work it can be repaired.")},
    {ItemID::ENCHANTED_OPTICAL_LENS, ItemManager::FormatDescription("A lens enhanced with magical powers, massively improving the users sight.")},
    {ItemID::LADY_OF_THE_LAKES_FIN, ItemManager::FormatDescription("A fin given by the lady of the lake.")},
    {ItemID::LADY_OF_THE_LAKES_BROMIDE, ItemManager::FormatDescription("A special salt given by the lady of the lake.")},
    {ItemID::BRIGHT_RED_CLOTH, ItemManager::FormatDescription("A bright cloth. Just looking at it fills anyone with DETERMINATION.")},
    {ItemID::HOLE_FILLED_CUBE, ItemManager::FormatDescription("A cube with many holes. Magical energy seems to pulse through it.")},
    //{ItemID::CANOLA_BOUQUET, ItemManager::FormatDescription("")},
    {ItemID::MINIATURE_TRAIN, ItemManager::FormatDescription("A miniature replica of the train servicing Numazu station.")},
    {ItemID::HUGE_CONCH, ItemManager::FormatDescription("A massive conch of an animal. It looks like it can somehow be charged.")},
    {ItemID::LAILAPS, ItemManager::FormatDescription("Yohane's trusted partner. Her claws strike anyone who wishes to harm Yohane.")},
    {ItemID::CHIKA, ItemManager::FormatDescription("A young girl working at an inn. Her Justogun can push heavy blocks away.")},
    {ItemID::RIKO, ItemManager::FormatDescription("A traveling veterinarian with magical powers. Her flame spells deal swiftly with icy enemies.")},
    {ItemID::KANAN, ItemManager::FormatDescription("A mechanic always looking for more parts for her creations. Her robot Tonosama can break certain floors.")},
    {ItemID::DIA, ItemManager::FormatDescription("The Chief Secretary of the Numazu Administrative Bureau. Her Ion Blade deals more damage against fiery enemies.")},
    {ItemID::YOU, ItemManager::FormatDescription("A cheerful message-girl working in Numazu. Her cannon can help Yohane cross big gaps.")},
    {ItemID::MARI, ItemManager::FormatDescription("A demon lord who is rarely seen outside her castle. The frosty monsties she commands make quick work of electric enemies.")},
    {ItemID::HANAMARU, ItemManager::FormatDescription("Yohane's childhood friend. Her rolling attack destroys spikes.")},
    {ItemID::RUBY, ItemManager::FormatDescription("A descendent of a fairy tribe. Her cotton candy can absorb enemy attacks.")},
    {ItemID::BOSS_TOKEN, ItemManager::FormatDescription("A sphere of unknown feelings. Maybe they explain why this dungeon exists?")},
    {ItemID::PROGRESSIVE_CHIKA, ItemManager::FormatDescription("Progressive Chika")},
    {ItemID::PROGRESSIVE_RIKO, ItemManager::FormatDescription("Progressive Riko")},
    {ItemID::PROGRESSIVE_KANAN, ItemManager::FormatDescription("Progressive Kanan")},
    {ItemID::PROGRESSIVE_DIA, ItemManager::FormatDescription("Progressive Dia")},
    {ItemID::PROGRESSIVE_YOU, ItemManager::FormatDescription("Progressive You")},
    {ItemID::PROGRESSIVE_MARI, ItemManager::FormatDescription("Progressive Mari")},
    {ItemID::PROGRESSIVE_HANAMARU, ItemManager::FormatDescription("Progressive Hanamaru")},
    {ItemID::PROGRESSIVE_RUBY, ItemManager::FormatDescription("Progressive Ruby")},
    {ItemID::SMALL_YEN, ItemManager::FormatDescription("A small bundle of money.")},
    {ItemID::MEDIUM_YEN, ItemManager::FormatDescription("A bundle of money.")},
    {ItemID::LARGE_YEN, ItemManager::FormatDescription("A big bundle of money.")},
};

int32_t SetItemOverride(Item* inventory, uint32_t item_id, int32_t amount)
{
    return ItemManager::getInstance().SetItemCount(item_id, amount);
}

char* FindItemNameOverride(ItemDBEntry* item)
{
    return ItemManager::getInstance().GetItemName(item);
}

char* FindItemDescriptionOverride(ItemDBEntry* item)
{
    return ItemManager::getInstance().GetItemDescription(item);
}

char GetEquipSlotsOverride(void* data)
{
    int8_t slot = *(int8_t*)(*(uint64_t*)((uint64_t)data + 0x570) + 0x1170);
    return ItemManager::getInstance().GetEquipmentSlots(slot);
}

void ItemManager::Init(const char* path)
{
    OnItemClear();
    if (set_item_count == nullptr)
    {
        set_item_count = std::make_shared<FunctionHook<int32_t, Item[1000], uint32_t, int32_t>>(::SetItemCount);
        set_item_count->Hook(SetItemOverride);
    }
    if (find_item_name == nullptr)
    {
        find_item_name = std::make_shared<FunctionHook<char*, ItemDBEntry*>>(FindItemName);
        find_item_name->Hook(FindItemNameOverride);
    }
    if (find_item_description == nullptr)
    {
        find_item_description = std::make_shared<FunctionHook<char*, ItemDBEntry*>>(FindItemDescription);
        find_item_description->Hook(FindItemDescriptionOverride);
    }
    if (get_equipment_slots == nullptr)
    {
        get_equipment_slots = std::make_shared<FunctionHook<char, void*>>(GetEquipSlots);
        get_equipment_slots->Hook(GetEquipSlotsOverride);
    }
}

void ItemManager::OnFrame(double delta)
{
    main_data->current_save.progression_flags &= 0xDB6DB6FF7FFFFF;
    if (received_items[(int64_t)ItemID::BOSS_TOKEN] >= 8 && main_data->current_save.area == Area::SUNKEN_TEMPLE &&
        (main_data->current_save.room == 9 || main_data->current_save.room == 10))
    {
        main_data->current_save.progression_flags |= 0x800000; // open Infernal Altar
    }
    main_data->current_save.character_unlocks &= 0xFFD5555F;
    for (auto& c: character_item_flags)
    {
        if (received_items[(int64_t)c.first] != 0)
        {
            main_data->current_save.character_unlocks |= c.second.first;
            main_data->current_save.progression_flags |= (uint64_t)c.second.second << 24;
        }
    }

    for (auto& upgrade : upgrade_data)
    {
        bool in_room = main_data->current_save.area == upgrade.second.area && upgrade.second.rooms.find(main_data->current_save.room) != upgrade.second.rooms.end();
        if (received_items[(int64_t)upgrade.first] != 0 &&
            (!in_room || APManager::getInstance().IsLocationChecked(upgrade.second.location)))
        {
            main_data->current_save.inventory[(int64_t)upgrade.first].data.count = 1;
        }
        else
        {
            main_data->current_save.inventory[(int64_t)upgrade.first].data.count = 0;
        }
    }

    if (main_data->current_save.inventory[(int64_t)ItemID::MUSICAL_SCORE].data.count == 0 && musical_scores != 0)
    {
        main_data->current_save.inventory[(int64_t)ItemID::MUSICAL_SCORE].data.count = 1;
        musical_scores -= 1;
    }

    for (auto& item : character_upgrade_map)
    {
        if (received_items[(int64_t)item.first] != 0)
        {
            int64_t id = (int64_t)item.second - 1;
            if (!APManager::getInstance().IsUpgradeHintTriggered(id))
            {
                if (APManager::getInstance().VanillaUpgradeHintsEnabled())
                {
                    AP_CreateHints({ (int64_t)upgrade_data[item.second].location }, AP_GetPlayerID());
                }
                APManager::getInstance().SetUpgradeHintTriggered(id);
            }
        }
    }
}

void ItemManager::OnItemClear()
{
    items_processed = 0;
    items_received = 0;
    musical_scores = 0;
    received_items.clear();
}

void ItemManager::OnItemRecv(int64_t item_id, bool notify)
{
    items_received++;
    //helperFunctions.log_debug("Received %s: (%d|%d)", APManager::getInstance().GetItemName(item_id), items_received, items_processed);
    if (items_received > items_processed)
    {
        AddItemCount(main_data->current_save.inventory, item_id, 1);
        if (notify)
            DisplayMessage(APManager::getInstance().GetItemName(item_id));
        items_processed++;
    }
    else if ((item_id > item_db.stats->max || item_id <= (int64_t)ItemID::MARI_UPGRADE) && !(item_id >= (int64_t)ItemID::SMALL_YEN && item_id <= (int64_t)ItemID::LARGE_YEN))
    {
        // important non Vanilla items have to be readded
        AddItemCount(main_data->current_save.inventory, item_id, 1);
    }
}

uint32_t ItemManager::SetItemCount(int64_t item_id, uint32_t amount)
{
    uint32_t out = 0;
    if (item_id == (int64_t)ItemID::FASHION_FOR_DUMMIES_2)
    {
        item_id = (int64_t)ItemID::FASHION_FOR_DUMMIES;
        amount += received_items[item_id];
    }

    received_items[item_id] += 1;
    out = received_items[item_id];
    if (item_id == (int64_t)ItemID::MUSICAL_SCORE)
    {
        musical_scores += 1;
    }
    else if (item_id < 1000)
    {
        out = set_item_count->CallOriginal(main_data->current_save.inventory, item_id, amount);
    }
    if (item_id >= (int64_t)ItemID::PROGRESSIVE_CHIKA && item_id <= (int64_t)ItemID::PROGRESSIVE_MARI)
    {
        if (received_items[item_id] == 1)
        {
            received_items[(int64_t)progressive_character_map[(ItemID)item_id].first] += 1;
        }
        if (received_items[item_id] > 1)
        {
            int32_t upgrade_id = (int64_t)progressive_character_map[(ItemID)item_id].second;
            received_items[upgrade_id] += 1;
            set_item_count->CallOriginal(main_data->current_save.inventory, upgrade_id, amount);
        }
    }
    if (item_id >= (int64_t)ItemID::SMALL_YEN && item_id <= (int64_t)ItemID::LARGE_YEN)
    {
        switch (item_id - (int64_t)ItemID::SMALL_YEN)
        {
        case 0:
            main_data->current_save.yen += 10000;
            break;
        case 1:
            main_data->current_save.yen += 25000;
            break;
        case 2:
            main_data->current_save.yen += 50000;
            break;
        }
    }

    if (item_id >= (int64_t)ItemID::FALLEN_ANGELS_SOARSHOES && item_id <= (int64_t)ItemID::SEA_DEITYS_CHARM)
    {
        main_data->current_save.equipped_abilities |= 1 << (item_id - (int64_t)ItemID::FALLEN_ANGELS_SOARSHOES);
    }
    if (item_id >= (int64_t)ItemID::CROSSBOW && item_id <= (int64_t)ItemID::WEAPONS_MAX)
    {
        if (main_data->current_save.equipped_weapon == 0)
        {
            main_data->current_save.equipped_weapon = (uint32_t)item_id;
        }
    }
    /*if (item_id >= (int64_t)ItemID::FORTUNE_TELLERS_VEIL && item_id <= (int64_t)ItemID::EQUIPMENTS_MAX)
    {
        uint32_t slots = main_data->current_save.inventory[(uint32_t)ItemID::FASHION_FOR_DUMMIES].data.count +
            main_data->current_save.inventory[(uint32_t)ItemID::FASHION_FOR_DUMMIES_2].data.count + 1;
        if (main_data->current_save.equipped_accessory1 == 0)
        {
            main_data->current_save.equipped_accessory1 = (uint32_t)item_id;
        }
        else if (main_data->current_save.equipped_accessory2 == 0 && slots > 1)
        {
            main_data->current_save.equipped_accessory2 = (uint32_t)item_id;
        }
        else if (main_data->current_save.equipped_accessory3 == 0 && slots > 2)
        {
            main_data->current_save.equipped_accessory3 = (uint32_t)item_id;
        }
    }*/
    return out;
}

void ItemManager::AddProcessedItem(int64_t item_id)
{
    items_processed++;
}

char* ItemManager::GetItemName(ItemDBEntry* item)
{
    return GetItemName(((uint64_t)item - (uint64_t)item_db.data) / item_db.stats->entry_size);
}

char* ItemManager::GetItemDescription(ItemDBEntry* item)
{
    return GetItemDescription(((uint64_t)item - (uint64_t)item_db.data) / item_db.stats->entry_size);
}

char* ItemManager::GetItemName(int64_t item_id)
{
    auto& item_data = APManager::getInstance().GetItemData();
    if (item_names.find((ItemID)item_id) != item_names.end())
    {
        return (char*)item_names[(ItemID)item_id];
    }
    else if (item_data.find(item_id) != item_data.end())
    {
        return item_data[item_id].name.data();
    }
    else if (item_id < item_db.stats->max - 1)
    {
        return find_item_name->CallOriginal(item_db.data + item_id);
    }
    else
        return "APItem";
}

char* ItemManager::GetItemDescription(int64_t item_id)
{
    auto& item_data = APManager::getInstance().GetItemData();
    if (item_descriptions.find((ItemID)item_id) != item_descriptions.end())
    {
        return (char*)item_descriptions[(ItemID)item_id].data();
    }
    else if (item_data.find(item_id) != item_data.end())
    {
        return item_data[item_id].description.data();
    }
    else if (item_id < item_db.stats->max - 1)
    {
        return find_item_description->CallOriginal(item_db.data + item_id);
    }
    else
        return "A mysterious item from another world. Maybe you can return it?";
}

int8_t ItemManager::GetEquipmentSlots(int8_t slot)
{
    SaveData* data = &main_data->current_save;
    if (slot < 10)
    {
        data = &main_data->loaded_saves[slot];
    }
    return 1 + data->inventory[(int64_t)ItemID::FASHION_FOR_DUMMIES].data.count + data->inventory[(int64_t)ItemID::FASHION_FOR_DUMMIES_2].data.count;
}

uint64_t ItemManager::GetProcessedItems() const
{
    return items_processed;
}

uint64_t ItemManager::GetMusicalScores() const
{
    return musical_scores;
}

std::unordered_map<int64_t, uint64_t> ItemManager::GetReceivedItems() const
{
    return received_items;
}

void ItemManager::SetProcessedItems(uint64_t items)
{
    items_processed = items;
}

void ItemManager::SetMusicalScores(uint64_t musical_scores)
{
    this->musical_scores = musical_scores;
}

std::string ItemManager::FormatDescription(std::string desc)
{
    // might need a more complex check for colours
    for (int i = 0x30; i < desc.size(); i += 0x30)
    {
        i = desc.find_last_of(' ', i) + 1;
        desc.insert(i, "\n");
    }
    return desc;
}
