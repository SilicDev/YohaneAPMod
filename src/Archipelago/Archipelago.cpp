#include "Archipelago.h"

#include <algorithm>
#include <filesystem>
#include <windows.h>

#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

#include "../ModLoader/IniFile.h"
#include "../ModLoader/ModInfo.h"
#include "../ModLoader/string.h"

#include "../YOHANE_BID/Functions.h"
#include "../YOHANE_BID/HelperFunctions.h"
#include "../YOHANE_BID/Variables.h"

constexpr char* gameTitle = "YOHANE THE PARHELION -BLAZE in the DEEPBLUE-";
constexpr int modVersion[3] = { 0, 0, 4 };
constexpr int minimum_world_version[3] = {0, 2, 0};

constexpr char* ap_config_section = "AP Config";

extern HelperFunctions helperFunctions;

Json::Reader reader;
Json::FastWriter writer;

bool has_inited = false;
const char* mod_path;

CommandHandler deathlink_cmd = { "deathlink", "[toggle]", "Sets the deathlink setting for AP",
            [](const char* args) { APManager::getInstance().ProcessDeathLinkCmd(args); } };
CommandHandler deathlink_group_cmd = { "deathlink_group", "[group]", "Sets the deathlink group for AP",
            [](const char* args) { APManager::getInstance().ProcessDeathLinkGroupCmd(args); } };
CommandHandler damagelink_cmd = { "damagelink", "[toggle]", "Sets the damagelink setting for AP",
            [](const char* args) { APManager::getInstance().ProcessDamageLinkCmd(args); } };
CommandHandler damagelink_group_cmd = { "damagelink_group", "[group]", "Sets the damagelink group for AP",
            [](const char* args) { APManager::getInstance().ProcessDamageLinkGroupCmd(args); } };
CommandHandler inventory_cmd = { "inventory", "", "Print the AP received items.",
            [](const char* args) { APManager::getInstance().ProcessInventoryCmd(args); } };
CommandHandler silic_asked_cmd = { "silic_asked", "[type]", "Print additional information about the game state.",
            [](const char* args) { APManager::getInstance().ProcessInformationCmd(args); } };
CommandHandler reset_processed_items_cmd = { "reset_processed_items", "", "",
            [](const char* args) { ItemManager::getInstance().SetProcessedItems(0); APManager::getInstance().WriteAPSave(); } };

std::shared_ptr<FunctionHook<float, EquipmentEffect>> on_get_equipment_effect_strength;
float GetEquipmentEffectStrengthOverride(EquipmentEffect effect)
{
    if (effect == EquipmentEffect::INCREASE_DROPRATE)
        return APManager::getInstance().GetDropRateIncrease() + on_get_equipment_effect_strength->CallOriginal(effect);
    return on_get_equipment_effect_strength->CallOriginal(effect);
}

void CryptData(uint64_t* data, uint64_t size)
{
    size = size / 8;
    uint64_t hash = Inti_BaseKey("yhn_ap.oss");
    for (int i = 0; i < size; i++)
    {
        *(data + i) = *(data + i) ^ hash;
    }
}

bool APManager::Init(const char* path) {
    mod_path = path;
    if (!has_inited)
    {
        helperFunctions.set_unhandled_text_handler([](const char* text) { AP_Say(text); });
        helperFunctions.register_command(deathlink_cmd);
        helperFunctions.register_command(deathlink_group_cmd);
        helperFunctions.register_command(damagelink_cmd);
        helperFunctions.register_command(damagelink_group_cmd);
        helperFunctions.register_command(inventory_cmd);
        helperFunctions.register_command(silic_asked_cmd);
        helperFunctions.register_command(reset_processed_items_cmd);
        on_get_equipment_effect_strength = std::make_shared<FunctionHook<float, EquipmentEffect>>(GetEquipmentEffectStrength);
        has_inited = true;
    }
    helperFunctions.log_info("Initialising AP connection...");
    char buf[MAX_PATH] = { 0 };
    char* fmt = "%s\\config.ini";
    sprintf_s(buf, sizeof(buf), fmt, path);
    FILE* f = nullptr;
    errno_t err = fopen_s(&f, buf, "r");
    IniFile* config;
    if (err == ENOENT)
    {
        helperFunctions.log_error("ERROR: Couldn't find AP config.ini. Creating template file.");
        f = nullptr;
        err = fopen_s(&f, buf, "w");
        if (err != 0)
        {
            helperFunctions.log_error("ERROR: Failed to create template file.");
            return false;
        }
        config = new IniFile(f);
        fclose(f);
        config->createSection(ap_config_section);
        config->setString(ap_config_section, "ip", "archipelago.gg:38281");
        config->setString(ap_config_section, "player_name", "");
        config->setString(ap_config_section, "psswd", "");
        helperFunctions.log_debug("Saving template.");
        config->save(buf);
        delete config;
        return false;
    }
    else if (err != 0)
    {
        helperFunctions.log_error("ERROR: Couldn't open AP config.ini.");
        return false;
    }
    else
    {
        config = new IniFile(f);
        fclose(f);
    }
    AP_SetLoggingCallback([](std::string s) { helperFunctions.log_debug((char*)s.c_str()); });
    player_name = config->getString(ap_config_section, "player_name").c_str();
    AP_Init(config->getString(ap_config_section, "ip").c_str(),
        gameTitle,
        player_name.c_str(),
        config->getString(ap_config_section, "psswrd").c_str());
    delete config;

    LocationManager::getInstance().Init(path);
    ItemManager::getInstance().Init(path);

    AP_SetItemClearCallback([](void) { APManager::getInstance().OnItemClear(); });
    AP_SetItemRecvCallback([](uint64_t item_id, bool notify) { APManager::getInstance().OnItemRecv(item_id, notify); });
    AP_SetLocationCheckedCallback([](uint64_t location_id) { APManager::getInstance().OnLocationChecked(location_id); });
    AP_SetLocationInfoCallback([](std::vector<AP_NetworkItem> location_info) { APManager::getInstance().OnLocationInfo(location_info); });
    AP_RegisterBouncedCallback([](AP_Bounce bounced) { APManager::getInstance().OnBounced(bounced); });
    // Slot data
    AP_RegisterSlotDataRawCallback("recipes", [](std::string recipes) { APManager::getInstance().ParseRecipeData(recipes); });
    AP_RegisterSlotDataIntCallback("early_chika_blocks_moved", [](int blocks_moved) { if (blocks_moved != 0) main_data->current_save.flags[0xF6] |= 2; });
    AP_RegisterSlotDataIntCallback("death_link", [this](int deathlink) { deathlink_enabled = deathlink; });
    AP_RegisterSlotDataRawCallback("death_link_group", [this](std::string group) { 
        Json::Value val;
        reader.parse(group, val);
        deathlink_group = val.asString();
    });
    AP_RegisterSlotDataIntCallback("damage_link", [this](int damagelink) { damagelink_enabled = damagelink; });
    AP_RegisterSlotDataRawCallback("damage_link_group", [this](std::string group) {
        Json::Value val;
        reader.parse(group, val);
        damagelink_group = val.asString();
    });
    AP_RegisterSlotDataRawCallback("world_version", [](std::string version) { APManager::getInstance().VerifyWorldVersion(version); });
    AP_RegisterSlotDataIntCallback("craftsanity", [this](int craftsanity) { this->craftsanity = craftsanity; });
    AP_RegisterSlotDataRawCallback("upgrade_hints", [](std::string hints) { APManager::getInstance().ParseHintTypes(hints); });
    AP_RegisterSlotDataRawCallback("upgrades", [](std::string hints) { APManager::getInstance().ParseUpgradeHints(hints); });
    AP_RegisterSlotDataIntCallback("drop_rate_increase", [this](int drop_rate_increase) { this->drop_rate_increase = drop_rate_increase; });

    LoadAPSave();

    fmt = "%s - AP Mod Vers. %d.%d.%d";
    sprintf_s(buf, sizeof(buf), fmt, gameTitle, modVersion[0], modVersion[1], modVersion[2]);
    BOOL success = SetWindowTextA(MainWindowHandle, buf);
    if (!success)
    {
        DWORD err = GetLastError();
        helperFunctions.log_error("Error changing window title: 0x%X!", err);
        SetLastError(0);
    }
    return true;
}

bool APManager::IsInit() const
{
    return AP_IsInit();
}

void APManager::LoadAPSave()
{
    char buf[MAX_PATH] = { 0 };
    char* fmt = "%s\\SAVEDATA\\yhnGameAP%03d";
    sprintf_s(buf, sizeof(buf), fmt, mod_path, main_data->selected_save_slot);
    FILE* file;
    errno_t err = fopen_s(&file, buf, "rb");
    if (err)
    {
        // assume new file
        helperFunctions.log_debug("Couldn't open AP save, creating a new one.");
        is_valid_save = true;
    }
    else
    {
        if (main_data->current_save.game_flags < 0x6 && main_data->current_save.save_time == 0)
        {
            // assume new file
            helperFunctions.log_debug("Game in early state, new save assumed.");
            is_valid_save = true;
            fclose(file);
        }
        else
        {
            fseek(file, 0, SEEK_END);
            uint64_t size = ftell(file);
            fseek(file, 0, SEEK_SET);
            uint64_t* buf = new uint64_t[size / sizeof(uint64_t)];
            fread_s(buf, size, sizeof(uint64_t), size / sizeof(uint64_t), file);
            fclose(file);
            CryptData(buf, size);
            std::string text = std::string((char*)buf);
            Json::Value value;
            reader.parse(text, value);
            seed_name = value["seed_name"].asString();
            ItemManager::getInstance().SetProcessedItems(value["received_items"].asUInt64());
            ItemManager::getInstance().SetMusicalScores(value["musical_scores"].asUInt64());
            upgrade_hints_triggered = value["upgrade_hints"].asUInt();
            if (player_name != value["playername"].asString())
            {
                helperFunctions.log_warn("Player name mismatch! AP is shutting down.");
                is_valid_save = false;
            }
            else
            {
                is_valid_save = true;
            }
        }
    }
}

void APManager::WriteAPSave()
{
    Json::Value value;
    value["seed_name"] = seed_name;
    value["playername"] = player_name;
    value["received_items"] = ItemManager::getInstance().GetProcessedItems();
    value["musical_scores"] = ItemManager::getInstance().GetMusicalScores();
    value["upgrade_hints"] = upgrade_hints_triggered;
    value["team"] = 1;
    std::string data = writer.write(value);
    uint64_t size = data.size() + (8 - data.size() % 8);
    data.resize(size);
    CryptData((uint64_t*)data.data(), data.size());
    char buf[MAX_PATH] = { 0 };
    char* fmt = "%s\\SAVEDATA\\yhnGameAP%03d";
    sprintf_s(buf, sizeof(buf), fmt, mod_path, main_data->selected_save_slot);
    FILE* file;
    errno_t err = fopen_s(&file, buf, "wb");
    if (err)
    {
        helperFunctions.log_error("Failed to open AP save file!");
    }
    else
    {
        fwrite(data.data(), sizeof(uint64_t), size / 8, file);
        fclose(file);
    }
}

bool APManager::IsRunning() const
{
    return running;
}

void APManager::Start()
{
    running = true;
    AP_Start();
}

void APManager::Shutdown()
{
    AP_Shutdown();
    running = false;
    goaled = false;
    is_valid_save = false;
    is_valid_version = false;
    is_version_checked = false;
    last_area = 0;
    locations_scouted = false;

    seed_name.clear();
    queued_locations.clear();
    checked_locations.clear();
    locations_checked.clear();
    item_data.clear();
    location_item_data.clear();

    deathlink_enabled = false;
    damagelink_enabled = false;
    damagelink_group.clear();
    damagelink_group.clear();

    craftsanity = false;
    drop_rate_increase = 0;
    vanilla_upgrade_hints_enabled = false;
    ap_upgrade_hints_enabled = false;
    upgrade_hints_triggered = 0;
}

void APManager::OnFrame(double delta)
{
    if (running)
    {
        AP_ConnectionStatus status = AP_GetConnectionStatus();
        if (last_connection != status)
        {
            if (status == AP_ConnectionStatus::Authenticated)
            {
                DisplayMessage("Connected to AP");
                LocationManager::getInstance().OnConnect();
            }
            else if (status == AP_ConnectionStatus::Disconnected)
            {
                DisplayMessage("Reconnecting...");
            }
        }
        if (status == AP_ConnectionStatus::ConnectionRefused)
        {
            if (item_messages == 0)
            {
                DisplayMessage("Failed to connect to AP!");
            }
        }
        else if (status == AP_ConnectionStatus::Authenticated)
        {
            if (!is_valid_version && is_version_checked)
            {
                running = false;
                return;
            }
            if (!is_save_checked && is_valid_save)
            {
                AP_RoomInfo roominfo;
                AP_GetRoomInfo(&roominfo);

                if (seed_name.empty())
                {
                    seed_name = roominfo.seed_name;
                }
                else if (seed_name != roominfo.seed_name)
                {
                    helperFunctions.log_warn("Slot mismatch! AP is shutting down.");
                    is_valid_save = false;
                }

            }
            if (!is_valid_save)
            {
                running = false;
                return;
            }
            if (main_data->saving)
            {
                WriteAPSave();
            }
            if (AP_IsMessagePending())
            {
                AP_Message* msg = AP_GetLatestMessage();
                std::string receiver;
                std::string sender;
                switch (msg->type)
                {
                case AP_MessageType::Plaintext:
                    helperFunctions.log_info(msg->text.c_str());
                    break;
                case AP_MessageType::ItemSend:
                    helperFunctions.log_info("%s sent %s to %s",
                        FormatPlayerNameForLog(((AP_ItemSendMessage*)msg)->item.playerName).c_str(),
                        FormatNetworkItemForLog(((AP_ItemSendMessage*)msg)->item).c_str(),
                        FormatPlayerNameForLog(((AP_ItemSendMessage*)msg)->recvPlayer).c_str());
                    break;
                case AP_MessageType::ItemRecv:
                    helperFunctions.log_info("Received %s from %s",
                        FormatNetworkItemForLog(((AP_ItemRecvMessage*)msg)->item).c_str(),
                        FormatPlayerNameForLog(((AP_ItemRecvMessage*)msg)->sendPlayer).c_str());
                    break;
                case AP_MessageType::Hint:
                    helperFunctions.log_info("%s's %s is at %s's <color/darkcyan>%s</color> - Found: %s", 
                        FormatPlayerNameForLog(((AP_HintMessage*)msg)->recvPlayer).c_str(),
                        FormatNetworkItemForLog(((AP_HintMessage*)msg)->item).c_str(),
                        FormatPlayerNameForLog(((AP_HintMessage*)msg)->sendPlayer).c_str(),
                        ((AP_HintMessage*)msg)->location.c_str(),
                        ((AP_HintMessage*)msg)->checked ? "<color/lightgreen>True</color>" : "<color/red>False</color>");
                    break;
                case AP_MessageType::Chat:
                    helperFunctions.log_info("%s: %s", FormatPlayerNameForLog(((AP_ChatMessage*)msg)->player).c_str(), ((AP_ChatMessage*)msg)->message.c_str());
                    break;
                case AP_MessageType::ServerChat:
                    helperFunctions.log_info("<color/white>Server</color>: %s", ((AP_ServerChatMessage*)msg)->message.c_str());
                    break;
                }
                AP_ClearLatestMessage();
            }
            if (!locations_scouted)
            {
                AP_SendLocationScouts(LocationManager::getInstance().GetActiveLocations(), 0);
                locations_scouted = true;
            }
            AP_RoomInfo info;
            AP_GetRoomInfo(&info);
            if ((deathlink_enabled && std::find(info.tags.begin(), info.tags.end(), "DeathLink" + deathlink_group) == info.tags.end()) ||
                (!deathlink_enabled && std::find(info.tags.begin(), info.tags.end(), "DeathLink" + deathlink_group) != info.tags.end()))
            {
                UpdateDeathlink(deathlink_enabled);
                UpdateDeathlinkGroup(deathlink_group);
            }
            if ((damagelink_enabled && std::find(info.tags.begin(), info.tags.end(), "SharedDamage" + damagelink_group) == info.tags.end()) ||
                (!damagelink_enabled && std::find(info.tags.begin(), info.tags.end(), "SharedDamage" + damagelink_group) != info.tags.end()))
            {
                UpdateDamagelink(damagelink_enabled);
                UpdateDamagelinkGroup(damagelink_group);
            }
            ItemManager::getInstance().OnFrame(delta);
            LocationManager::getInstance().OnFrame(delta);
            if (queued_locations.size() != 0)
            {
                for (auto& id : queued_locations)
                {
                    locations_checked.insert(id);
                }
                AP_SendItem(queued_locations);
                queued_locations.clear();
            }
            byte area = (byte)main_data->current_save.area;
            if ((main_data->current_save.game_flags & 0x8) != 0)
            {
                // In parlor
                area = 0;
            }
            if (area != last_area)
            {
                std::vector<int64_t> slots = { AP_GetPlayerID() };
                Json::Value msg;
                msg["type"] = "MapUpdate";
                msg["mapId"] = area;
                AP_SendBounce({
                    nullptr,
                    &slots,
                    nullptr,
                    writer.write(msg)
                    });
                last_area = area;
            }
            uint64_t flags_struct_ptr = *((uint64_t*)(*((uint64_t*)(*((uint64_t*)flags_struct + 5)) + 1)) + 1);
            if ((main_data->current_save.game_flags & 1) != 0 || *(uint8_t*)(flags_struct_ptr + 0xCE) != 0)
            {
                APManager::getInstance().SendGoal();
            }
            if (deathlink_enabled)
            {
                bool is_dead = *(uint8_t*)(flags_struct_ptr + 0x360);
                if (!is_dead && !can_send_deathlink)
                {
                    can_send_deathlink = true;
                }
                else if (is_dead && can_send_deathlink)
                {
                    SendDeath("%YOU% let Yohane run out of HP.");
                    can_send_deathlink = false;
                }
            }
            if (damagelink_enabled)
            {
                uint64_t yohane_struct_ptr = *((uint64_t*)flags_struct_ptr + 69);
                uint32_t health = *((uint32_t*)yohane_struct_ptr + 0xA);
                uint32_t max_health = *((uint32_t*)yohane_struct_ptr + 0xB);
                if (health < last_health && max_health == last_max_health)
                {
                    SendDamage(last_health - health, "%YOU% let Yohane get hit.");
                }
                last_health = health;
                last_max_health = max_health;
            }
        }
        last_connection = status;
    }
}

void APManager::OnItemClear()
{
    ItemManager::getInstance().OnItemClear();
    LoadAPSave();
}

void APManager::OnItemRecv(int64_t item_id, bool notify)
{
    ItemManager::getInstance().OnItemRecv(item_id, notify);
}

void APManager::OnLocationChecked(int64_t location_id)
{
    if (checked_locations.find(location_id) == checked_locations.end())
        checked_locations.insert(location_id);
    if (checked_locations.find(location_id) != checked_locations.end() && locations_checked.find(location_id) == locations_checked.end())
    {
        LocationManager::getInstance().OnLocationChecked(location_id);
        locations_checked.emplace(location_id);
    }
}

void APManager::OnLocationInfo(std::vector<AP_NetworkItem> location_info)
{
    auto& build_description = [](AP_NetworkItem item) {
        std::string description = item.playerName + "'s " + item.itemName + " is a mysterious Item from another world.";
        if ((item.flags & AP::PROGRESSION) != 0)
        {
            description += " It seems to be important.";
        }
        description += " Maybe you can return it?";
        return ItemManager::FormatDescription(description);
    };
    for (auto& item : location_info)
    {
        if (item.location >= (uint64_t)LocationID::RECIPE_01 && item.location <= (uint64_t)LocationID::RECIPE_93)
        {
            if (item.player == AP_GetPlayerID())
            {
                if (item.item < item_db.stats->max)
                {
                    (item_create_db.data + (item.location - 700))->result_id = item.item;
                    item_data[item.location] = {
                        item.location,
                        std::to_string(item.location - 700) + ") " + item.itemName,
                        GetItemDescription(item.item),
                        item.flags,
                        item.location
                    };
                    location_item_data[item.location] = item_data[item.location];
                }
                else
                {
                    (item_create_db.data + (item.location - 700))->result_id = item.location;
                    item_data[item.location] = {
                        item.location,
                        std::to_string(item.location - 700) + ") " + item.itemName,
                        GetItemDescription(item.item),
                        item.flags,
                        item.location
                    };
                    location_item_data[item.location] = item_data[item.location];
                }
            }
            else
            {
                (item_create_db.data + (item.location - 700))->result_id = item.location;
                item_data[item.location] = {
                    item.location,
                    std::to_string(item.location - 700) + ") " + item.playerName + "'s " + item.itemName,
                    build_description(item),
                    item.flags,
                    item.location
                };
                location_item_data[item.location] = item_data[item.location];
            }
        }
        else
        {
            if (item.player == AP_GetPlayerID())
            {
                if (item.item > item_db.stats->max)
                {
                    item_data[item.item] = {
                        item.item,
                        item.itemName,
                        build_description(item),
                        item.flags,
                        item.location
                    };
                    location_item_data[item.location] = item_data[item.item];
                }
                else
                {
                    location_item_data[item.location] = {
                        item.item,
                        item.itemName,
                        GetItemDescription(item.item),
                        item.flags,
                        item.location
                    };
                }
            }
            else
            {
                // non recipe foreign items
                int64_t item_id = 0x8000 + item_data.size();
                item_data[item_id] = {
                    item_id,
                    item.playerName + "'s " + item.itemName,
                    build_description(item),
                    item.flags,
                    item.location
                };
                location_item_data[item.location] = item_data[item_id];
            }
        }
    }
}

void APManager::OnBounced(AP_Bounce bounced)
{
    if (bounced.tags != nullptr)
    {
        for (int i = 0; i < bounced.tags->size(); i++)
        {
            std::string tag = bounced.tags->at(i);
            if (tag == "DeathLink" + damagelink_group)
            {
                Json::Value data;
                reader.parse(bounced.data, data);
                std::string source = data["source"].asString();
                if (source != player_name)
                {
                    std::string cause = data["cause"].isNull() ? "" : data["cause"].asString();
                    OnDeathLink(source, cause);
                }
            }
            if (tag == "SharedDamage" + damagelink_group)
            {
                Json::Value data;
                reader.parse(bounced.data, data);
                std::string source = data["source"].asString();
                if (source != player_name)
                {
                    int32_t damage = data["damage_points"].asInt();
                    std::string cause = data["cause"].isNull() ? "" : data["cause"].asString();
                    OnDamageLink(source, damage, cause);
                }
            }
        }
    }
}

void APManager::OnDeathLink(std::string source, std::string cause)
{
    std::string message = "Received death";
    if (!cause.empty())
    {
        message += +": " + cause;
    }
    else
    {
        message += " from " + source;
    }
    helperFunctions.log_info(message.c_str());
    uint64_t flags_struct_ptr = *((uint64_t*)(*((uint64_t*)(*((uint64_t*)flags_struct + 5)) + 1)) + 1);
    *(uint8_t*)(flags_struct_ptr + 0x360) = 1;
    *(uint8_t*)(flags_struct_ptr + 0xCC) = 1;
    can_send_deathlink = false;
}

void APManager::OnDamageLink(std::string source, uint32_t damage, std::string cause)
{
    std::string message = "Received " + std::to_string(damage) + " damage";
    if (!cause.empty())
    {
        message += +": " + cause;
    }
    else
    {
        message += " from " + source;
    }
    helperFunctions.log_info(message.c_str());
    uint64_t flags_struct_ptr = *((uint64_t*)(*((uint64_t*)(*((uint64_t*)flags_struct + 5)) + 1)) + 1);
    uint64_t yohane_struct_ptr = *((uint64_t*)flags_struct_ptr + 69);
    *((uint32_t*)yohane_struct_ptr + 0xA) = max(*((uint32_t*)yohane_struct_ptr + 0xA) - damage, 0);
}

void APManager::SendGoal()
{
    if (!goaled)
    {
        AP_StoryComplete();
        goaled = true;
    }
}

void APManager::SendDeath(std::string cause)
{
    helperFunctions.log_info("DeathLink: Sending death to your friends...");
    std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
    AP_Bounce b;
    Json::Value v;
    v["time"] = (int64_t)std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count();
    v["source"] = player_name; // Name and Shame >:D
    if (!cause.empty())
    {
        std::string cause_pname = cause;
        constexpr std::string_view pname_you{ "%YOU%" };
        size_t pname_marker = cause_pname.find(pname_you);

        if (pname_marker != std::string::npos)
            cause_pname.replace(pname_marker, pname_you.size(), player_name);
        v["cause"] = cause_pname;
    }
    b.data = writer.write(v);
    b.games = nullptr;
    b.slots = nullptr;
    std::vector<std::string> tags = { std::string("DeathLink") };
    b.tags = &tags;
    AP_SendBounce(b);
}

void APManager::UpdateDeathlink(bool deathlink)
{
    AP_RoomInfo roominfo;
    AP_GetRoomInfo(&roominfo);
    std::vector<std::string> tags = roominfo.tags;
    if (deathlink)
    {
        tags.emplace_back("DeathLink" + deathlink_group);
    }
    else
    {
        auto iter = std::find(tags.begin(), tags.end(), "DeathLink" + deathlink_group);
        if (iter != tags.end())
            tags.erase(iter);
    }
    AP_UpdateTags(tags);
}

void APManager::UpdateDeathlinkGroup(std::string group)
{
    if (group == deathlink_group) return;
    AP_RoomInfo roominfo;
    AP_GetRoomInfo(&roominfo);
    std::vector<std::string> tags = roominfo.tags;
    auto iter = std::find(tags.begin(), tags.end(), "DeathLink" + deathlink_group);
    bool deathlink = iter != tags.end();
    if (deathlink)
    {
        tags.erase(iter);
    }
    deathlink_group = group;
    if (deathlink)
    {
        tags.emplace_back("DeathLink" + deathlink_group);
    }
    AP_UpdateTags(tags);
}

void APManager::SendDamage(uint32_t damage, std::string cause)
{
    helperFunctions.log_info("SharedDamage: Sending %d damage to your friends...", damage);
    std::vector<std::string> tags = { "SharedDamage" + damagelink_group };
    Json::Value msg;
    std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
    msg["time"] = (int64_t)std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count();
    msg["source"] = player_name;
    if (!cause.empty())
    {
        std::string cause_pname = cause;
        constexpr std::string_view pname_you{ "%YOU%" };
        size_t pname_marker = cause_pname.find(pname_you);

        if (pname_marker != std::string::npos)
            cause_pname.replace(pname_marker, pname_you.size(), player_name);
        msg["cause"] = cause_pname;
    }
    msg["damage_points"] = damage;
    msg["uuid"] = AP_GetUUID();
    AP_SendBounce({
        {},
        {},
        &tags,
        writer.write(msg)
        });

}

void APManager::UpdateDamagelink(bool damagelink)
{
    AP_RoomInfo roominfo;
    AP_GetRoomInfo(&roominfo);
    std::vector<std::string> tags = roominfo.tags;
    if (damagelink)
    {
        tags.emplace_back("SharedDamage" + damagelink_group);
    }
    else
    {
        auto iter = std::find(tags.begin(), tags.end(), "SharedDamage" + damagelink_group);
        if (iter != tags.end())
            tags.erase(iter);
    }
    AP_UpdateTags(tags);
}

void APManager::UpdateDamagelinkGroup(std::string group)
{
    if (group == damagelink_group) return;
    AP_RoomInfo roominfo;
    AP_GetRoomInfo(&roominfo);
    std::vector<std::string> tags = roominfo.tags;
    auto iter = std::find(tags.begin(), tags.end(), "SharedDamage" + damagelink_group);
    bool damagelink = iter != tags.end();
    if (damagelink)
    {
        tags.erase(iter);
    }
    damagelink_group = group;
    if (damagelink)
    {
        tags.emplace_back("SharedDamage" + damagelink_group);
    }
    AP_UpdateTags(tags);
}

void APManager::CheckLocation(LocationID location_id)
{
    if (locations_checked.find((int64_t)location_id) == locations_checked.end())
        queued_locations.insert((int64_t)location_id);
}

bool APManager::IsLocationChecked(LocationID location_id)
{
    return locations_checked.find((int64_t)location_id) != locations_checked.end();
}

char* APManager::GetItemName(int64_t item_id)
{
    return ItemManager::getInstance().GetItemName(item_id);
}

char* APManager::GetItemDescription(int64_t item_id)
{
    return ItemManager::getInstance().GetItemDescription(item_id);
}

void APManager::ParseRecipeData(std::string recipes)
{
    auto byteswap = [](uint16_t num) { return ((num & 0xFF) << 8) + ((num & 0xFF00) >> 8); };
    Json::Value value;
    reader.parse(recipes, value);
    recipes = value.asString();
    for (int i = 1; i < item_create_db.stats->max; i++)
    {
        ItemCreateDBEntry& entry = item_create_db.data[i];
        uint16_t ingredient = byteswap(std::stoul(recipes.substr(0, 4), nullptr, 16));
        uint32_t id = ingredient & 0x3FF;
        uint32_t amount = (ingredient & 0xFC00) >> 10;
        entry.ingredient1_id = id;
        entry.ingredient1_count = amount;

        ingredient = byteswap(std::stoul(recipes.substr(4, 4), nullptr, 16));
        id = ingredient & 0x3FF;
        amount = (ingredient & 0xFC00) >> 10;
        entry.ingredient2_id = id;
        entry.ingredient2_count = amount;

        ingredient = byteswap(std::stoul(recipes.substr(8, 4), nullptr, 16));
        id = ingredient & 0x3FF;
        amount = (ingredient & 0xFC00) >> 10;
        entry.ingredient3_id = id;
        entry.ingredient3_count = amount;

        ingredient = byteswap(std::stoul(recipes.substr(12, 4), nullptr, 16));
        id = ingredient & 0x3FF;
        amount = (ingredient & 0xFC00) >> 10;
        entry.ingredient4_id = id;
        entry.ingredient4_count = amount;
        recipes = recipes.substr(16);
    }
}

void APManager::VerifyWorldVersion(std::string version)
{
    int version_tuple[3] = { 0, 0, 0 };
    Json::Value val;
    reader.parse(version, val);
    version = val.asString();
    uint64_t pos = version.find_last_of('.');
    if (pos == std::string::npos)
    {
        version_tuple[2] = std::stoul(version);
    }
    else
    {
        uint64_t pos2 = version.find_last_of('.', pos - 1);
        if (pos2 == std::string::npos)
        {
            version_tuple[1] = std::stoul(version.substr(0, pos));
            version_tuple[2] = std::stoul(version.substr(pos + 1));
        }
        else
        {
            version_tuple[0] = std::stoul(version.substr(0, pos2));
            version_tuple[1] = std::stoul(version.substr(pos2 + 1, pos - pos2 - 1));
            version_tuple[2] = std::stoul(version.substr(pos + 1));
        }
    }
    if (version_tuple[0] > minimum_world_version[0] || (version_tuple[0] == minimum_world_version[0] && version_tuple[1] > minimum_world_version[1]) ||
        (version_tuple[0] == minimum_world_version[0] && version_tuple[1] == minimum_world_version[1] && version_tuple[2] >= minimum_world_version[2]))
    {
        is_valid_version = true;
    }
    else
    {
        is_valid_version = false;
        helperFunctions.log_warn("Version mismatch detected! World version is %s, expected %d.%d.%d!", version.c_str(),
            minimum_world_version[0], minimum_world_version[1], minimum_world_version[2]);
    }
    is_version_checked = true;
}

void APManager::ParseHintTypes(std::string hints)
{
    Json::Value value;
    reader.parse(hints, value);
    for (int i = 0; i < value.size(); i++)
    {
        std::string hint = value[i].asString();
        if (hint == "AP")
        {
            ap_upgrade_hints_enabled = true;
        }
        if (hint == "Vanilla")
        {
            vanilla_upgrade_hints_enabled = true;
        }
    }
}

void APManager::ParseUpgradeHints(std::string hints)
{
    Json::Value value;
    reader.parse(hints, value);
    for (int i = 0; i < value.size(); i++)
    {
        for (int j = 0; j < value[i].size(); j++)
        {
            upgrade_hints[i].insert({ value[i][j][0].asInt(), value[i][j][1].asInt64() });
        }
    }
}

std::unordered_map<int64_t, ItemData>& APManager::GetItemData()
{
    return item_data;
}

std::unordered_map<int64_t, ItemData>& APManager::GetLocationItemData()
{
    return location_item_data;
}

bool APManager::CraftSanityEnabled() const
{
    return craftsanity;
}

bool APManager::VanillaUpgradeHintsEnabled() const
{
    return vanilla_upgrade_hints_enabled;
}

bool APManager::APUpgradeHintsEnabled() const
{
    return ap_upgrade_hints_enabled;
}

bool APManager::IsUpgradeHintTriggered(uint64_t hint) const
{
    return (upgrade_hints_triggered & (1 << hint)) != 0;
}

void APManager::SetUpgradeHintTriggered(uint64_t hint)
{
    if ((upgrade_hints_triggered & (1 << hint)) == 0)
    {
        if (ap_upgrade_hints_enabled)
        {
            for (auto& loc : upgrade_hints[(uint8_t)hint])
            {
                AP_CreateHints({ loc.second }, loc.first);
            }
        }
        upgrade_hints_triggered |= (1 << hint);
    }
}

void APManager::ProcessDeathLinkCmd(std::string args)
{
    if (AP_GetConnectionStatus() != AP_ConnectionStatus::Authenticated)
    {
        helperFunctions.log_error("Must be connected to the AP server to use this command!");
        return;
    }
    if (!running)
    {
        helperFunctions.log_error("Must have selected a valid save to use this command!");
        return;
    }
    if (args.empty())
    {
        deathlink_enabled = !deathlink_enabled;
    }
    else
    {
        args = trim(args);
        std::vector<std::string> arg_list = split(args, " \t\n\r\f\v");
        std::string toggle = arg_list[0];
        std::transform(toggle.begin(), toggle.end(), toggle.begin(), ::tolower);
        if (toggle == "on" || toggle == "true" || toggle == "yes" || toggle == "1")
        {
            deathlink_enabled = true;
        }
        else
        {
            deathlink_enabled = false;
        }
    }
    if (deathlink_enabled)
        helperFunctions.log_info("Death Link turned on!");
    else
        helperFunctions.log_info("Death Link turned off!");
    UpdateDeathlink(deathlink_enabled);
}

void APManager::ProcessDeathLinkGroupCmd(std::string args)
{
    if (AP_GetConnectionStatus() != AP_ConnectionStatus::Authenticated)
    {
        helperFunctions.log_error("Must be connected to the AP server to use this command!");
        return;
    }
    if (!running)
    {
        helperFunctions.log_error("Must have selected a valid save to use this command!");
        return;
    }
    if (args.empty())
    {
        if (!deathlink_group.empty())
        {
            UpdateDeathlinkGroup(args);
            helperFunctions.log_info("Death Link group changed to global default group");
            return;
        }
        helperFunctions.log_info("Already in global default group");
    }
    else
    {
        std::vector<std::string> arg_list = split(args, " \t\n\r\f\v");
        std::string group = arg_list[0];
        if (group != deathlink_group)
        {
            UpdateDeathlinkGroup(group);
            helperFunctions.log_info("Death Link group changed to '%s'", group);
        }
        else
        {
            helperFunctions.log_info("Already in Damage Link group '%s'", group);
        }
    }
}

void APManager::ProcessDamageLinkCmd(std::string args)
{
    if (AP_GetConnectionStatus() != AP_ConnectionStatus::Authenticated)
    {
        helperFunctions.log_error("Must be connected to the AP server to use this command!");
        return;
    }
    if (!running)
    {
        helperFunctions.log_error("Must have selected a valid save to use this command!");
        return;
    }
    if (args.empty())
    {
        damagelink_enabled = !damagelink_enabled;
    }
    else
    {
        args = trim(args);
        std::vector<std::string> arg_list = split(args, " ");
        std::string toggle = arg_list[0];
        std::transform(toggle.begin(), toggle.end(), toggle.begin(), ::tolower);
        if (toggle == "on" || toggle == "true" || toggle == "yes" || toggle == "1")
        {
            damagelink_enabled = true;
        }
        else
        {
            damagelink_enabled = false;
        }
    }
    if (damagelink_enabled)
        helperFunctions.log_info("Damage Link turned on!");
    else
        helperFunctions.log_info("Damage Link turned off!");
    UpdateDamagelink(damagelink_enabled);
}

void APManager::ProcessDamageLinkGroupCmd(std::string args)
{
    if (AP_GetConnectionStatus() != AP_ConnectionStatus::Authenticated)
    {
        helperFunctions.log_error("Must be connected to the AP server to use this command!");
        return;
    }
    if (!running)
    {
        helperFunctions.log_error("Must have selected a valid save to use this command!");
        return;
    }
    if (args.empty())
    {
        if (!damagelink_group.empty())
        {
            UpdateDamagelinkGroup(args);
            helperFunctions.log_info("Damage Link group changed to global default group");
            return;
        }
        helperFunctions.log_info("Already in global default group");
    }
    else
    {
        std::vector<std::string> arg_list = split(args, " ");
        std::string group = arg_list[0];
        if (group != damagelink_group)
        {
            UpdateDamagelinkGroup(group);
            helperFunctions.log_info("Damage Link group changed to '%s'", group);
        }
        else
        {
            helperFunctions.log_info("Already in Damage Link group '%s'", group);
        }
    }
}

void APManager::ProcessInventoryCmd(std::string args)
{
    auto items = ItemManager::getInstance().GetReceivedItems();
    std::string message = "Received Items: ";
    for (auto& item : items)
    {
        char buf[0x100] = { 0 };
        sprintf_s(buf, 0x100, "%s (%lld), ", GetItemName(item.first), item.second);
        message += buf;
    }
    helperFunctions.log_info(message.c_str());
}

void APManager::ProcessInformationCmd(std::string args)
{
    if (args.empty()) // no type, print everything
    {
        PrintGameFlags();
    }
    else
    {
        std::vector<std::string> arg_list = split(args, " ");
        for (size_t i = 0; i < arg_list.size(); i++)
        {
            if (arg_list.at(i) == "flags")
            {
                PrintGameFlags();
            }
        }
    }
}

std::string APManager::FormatPlayerNameForLog(std::string& player)
{
    if (player == player_name)
    {
        player = "<color/pink>" + player + "</color>";
    }
    else
    {
        player = "<color/gold>" + player + "</color>";
    }
    return player;
}

std::string APManager::FormatNetworkItemForLog(AP_NetworkItem& item)
{
    std::string item_name = item.itemName;
    if (item.flags == AP::FILLER)
    {
        item_name = "<color/cyan>" + item_name + "</color>";
    }
    else if (item.flags == AP::PROGRESSION)
    {
        item_name = "<color/purple>" + item_name + "</color>";
    }
    else if (item.flags == AP::USEFUL)
    {
        item_name = "<color/lightblue>" + item_name + "</color>";
    }
    else if (item.flags == (AP::PROGRESSION | AP::USEFUL))
    {
        item_name = "<color/blue>" + item_name + "</color>";
    }
    else if (item.flags == AP::TRAP)
    {
        item_name = "<color/lightred>" + item_name + "</color>";
    }
    else if (item.flags == (AP::TRAP | AP::PROGRESSION)) // why would you do that?
    {
        item_name = "<color/gold>" + item_name + "</color>";
    }
    else if (item.flags == (AP::TRAP | AP::USEFUL))
    {
        item_name = "<color/red>" + item_name + "</color>";
    }
    else if (item.flags == (AP::TRAP | AP::PROGRESSION | AP::USEFUL)) // why would you do that?
    {
        item_name = "<color/yellow>" + item_name + "</color>";
    }
    else // unknown classification
    {
        item_name = "<color/darkgrey>" + item_name + "</color>";
    }
    return item_name;
}

void APManager::PrintGameFlags()
{
    std::string message = "Important Game Flags: \n";

    char buf[0x100] = { 0 };
    sprintf_s(buf, 0x100, "Bosses: 0x%08X, ", main_data->current_save.bosses_defeated);
    message += buf;
    std::fill_n(buf, 0x100, 0);
    sprintf_s(buf, 0x100, "Rescues: 0x%04X, ", (uint16_t)(main_data->current_save.progression_flags >> 8));
    message += buf;
    std::fill_n(buf, 0x100, 0);
    sprintf_s(buf, 0x100, "Quests: 0x%08X, ", (uint32_t)(main_data->current_save.progression_flags >> 24));
    message += buf;
    std::fill_n(buf, 0x100, 0);
    helperFunctions.log_info(message.c_str());
}
