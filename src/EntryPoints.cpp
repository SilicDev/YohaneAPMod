#include "stdafx.h"

#include <stdint.h>

#include "Archipelago/Archipelago.h"
#include "ModLoader/ModInfo.h"
#include "YOHANE_BID/Functions.h"
#include "YOHANE_BID/Variables.h"

HelperFunctions helperFunctions;
char mod_path[MAX_PATH] = { 0 };

extern "C" __declspec(dllexport) void OnFrame(double delta)
{
    if (!APManager::getInstance().IsRunning() && !APManager::getInstance().IsInit() && main_data != nullptr && main_data->selected_save_slot != 0xA)
    {
        bool success = APManager::getInstance().Init(mod_path);
        if (!success)
        {
            MessageBox(MainWindowHandle, "Failed to initialize AP.\nThe game will now close.\n", "ERROR", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
            ExitProcess(1);
            return;
        }
        APManager::getInstance().Start();
    }
    if (APManager::getInstance().IsRunning() && main_data != nullptr && main_data->selected_save_slot == 0xA)
    {
        APManager::getInstance().Shutdown();
    }
    APManager::getInstance().OnFrame(delta);
}

extern "C" __declspec(dllexport) void __cdecl Init(const char* path, HelperFunctions& helperfunctions, uint32_t modIndex)
{
    helperFunctions = helperfunctions;
    memcpy_s(mod_path, MAX_PATH, path, strlen(path));
    helperFunctions.log_info("Initialising AP mod...");
    helperFunctions.log_info("Finished setting up AP mod!");
}

extern "C" __declspec(dllexport) ModInfo YHNModInfo = { ModLoaderVer };
