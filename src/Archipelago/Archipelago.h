#pragma once

#include <stdint.h>
#include <set>
#include <unordered_map>
#include <string>

#include "../APCpp/Archipelago.h"

#include "Items.h"
#include "Locations.h"

namespace AP
{
    enum AP_ItemClassification
    {
        FILLER = 0b0000,
        PROGRESSION = 0b0001,
        USEFUL = 0b0010,
        TRAP = 0b0100,
    };
}

struct ItemData
{
    int64_t id = 0;
    std::string name;
    std::string description;
    int flags = 0;
    int64_t location = 0;
};

class APManager
{
private:
    APManager() {};

public:
    static APManager& getInstance()
    {
        static APManager instance;
        return instance;
    }

    APManager(APManager const&) = delete;
    void operator=(APManager const&) = delete;

    bool Init(const char* path);
    bool IsInit() const;

    void LoadAPSave();
    void WriteAPSave();

    bool IsRunning() const;
    void Start();
    void Shutdown();

    void OnFrame(double delta);

    void OnItemClear();
    void OnItemRecv(int64_t item_id, bool notify);
    void OnLocationChecked(int64_t location_id);
    void OnLocationInfo(std::vector<AP_NetworkItem> location_info);
    void OnBounced(AP_Bounce bounced);
    void OnDeathLink(std::string source, std::string cause);
    void OnDamageLink(std::string source, uint32_t damage, std::string cause);

    void SendGoal();

    void SendDeath(std::string cause);
    void UpdateDeathlink(bool deathlink);
    void UpdateDeathlinkGroup(std::string group);
    void SendDamage(uint32_t damage, std::string cause);
    void UpdateDamagelink(bool damagelink);
    void UpdateDamagelinkGroup(std::string group);

    void CheckLocation(LocationID location_id);
    bool IsLocationChecked(LocationID location_id);
    char* GetItemName(int64_t item_id);
    char* GetItemDescription(int64_t item_id);

    void ParseRecipeData(std::string recipes);
    void VerifyWorldVersion(std::string version);
    void ParseHintTypes(std::string hints);
    void ParseUpgradeHints(std::string hints);

    std::unordered_map<int64_t, ItemData>& GetItemData();
    std::unordered_map<int64_t, ItemData>& GetLocationItemData();

    bool CraftSanityEnabled() const;
    bool VanillaUpgradeHintsEnabled() const;
    bool APUpgradeHintsEnabled() const;
    bool IsUpgradeHintTriggered(uint64_t hint) const;
    void SetUpgradeHintTriggered(uint64_t hint);

    void ProcessDeathLinkCmd(std::string args);
    void ProcessDeathLinkGroupCmd(std::string args);
    void ProcessDamageLinkCmd(std::string args);
    void ProcessDamageLinkGroupCmd(std::string args);
    void ProcessInventoryCmd(std::string args);
    void ProcessInformationCmd(std::string args);

    std::string FormatPlayerNameForLog(std::string& player);
    std::string FormatNetworkItemForLog(AP_NetworkItem& item);
private:
    void PrintGameFlags();

    bool running = false;
    bool goaled = false;
    bool locations_scouted = false;
    bool is_valid_save = true;
    bool is_save_checked = false;
    bool is_valid_version = false;
    bool is_version_checked = false;

    std::string player_name;
    std::string seed_name;
    AP_ConnectionStatus last_connection = AP_ConnectionStatus::Disconnected;

    uint8_t last_area = 0;

    std::set<int64_t> checked_locations;
    std::set<int64_t> locations_checked;
    std::set<int64_t> queued_locations;
    std::unordered_map<int64_t, ItemData> item_data;
    std::unordered_map<int64_t, ItemData> location_item_data;

    std::unordered_map<uint8_t, std::set<std::pair<int64_t, int64_t>>> upgrade_hints;

    bool deathlink_enabled = false;
    bool can_send_deathlink = false;
    bool damagelink_enabled = false;
    std::string deathlink_group;
    std::string damagelink_group;
    uint32_t last_health = 0;
    uint32_t last_max_health = 0;

    bool craftsanity = false;
    bool vanilla_upgrade_hints_enabled = false;
    bool ap_upgrade_hints_enabled = false;
    uint8_t upgrade_hints_triggered = 0;
};
