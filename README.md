# Yohane AP mod

A mod using the [YohaneBIDModLoader](https://github.com/SilicDev/YohaneBIDModLoader/tree/master) to implement the 
Archipelago randomizer for YOHANE THE PARHELION -BLAZE in the DEEPBLUE-

### Setup

Download the zip from [Releases](https://github.com/SilicDev/YohaneAPMod/releases/latest) and place its contents in 
`<path_to_game.exe>/.mods/YohaneAPMod`. Manually copy the APCpp.dll so it is next to the game.exe or run the ```CopyAPCppDll.bat```
to move it to a position the game can find it in.
Add it to the modloader's active_mods in `.mods/.modloader/config.ini` to make the game load the mod.
Enter your connection details in the included config.ini and you are good to go.

### Build 

First clone the project with all of its submodules
```console
git clone --recursive https://github.com/SilicDev/YohaneAPMod.git
```
Then you can build the mod as a regular CMake project
```console
cmake .
cmake --build .
```
The APCpp library may need to be build twice to make sure the dll is available.
