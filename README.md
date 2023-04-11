# LamasTinyHUD
![Platform](https://img.shields.io/static/v1?label=platform&message=windows&color=dimgray&style=plastic&logo=windows)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/mlthelama/LamasTinyHUD?style=plastic)
![GitHub](https://img.shields.io/github/license/mlthelama/LamasTinyHUD?style=plastic)
![GitHub top language](https://img.shields.io/github/languages/top/mlthelama/LamasTinyHUD?style=plastic)
![GitHub language count](https://img.shields.io/github/languages/count/mlthelama/LamasTinyHUD?style=plastic)
![GitHub last commit](https://img.shields.io/github/last-commit/mlthelama/LamasTinyHUD?style=plastic)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/mlthelama/LamasTinyHUD/main.yml?style=plastic)

A small UI Element

## What does it do
Allows you to bind Weapons, Shields, Spells, Shouts, Powers and Potions/Food to a Hotkey. In order to bind it to a Hotkey the Item needs to be favorited. Before every Execute, it is checked if the Item is still avaliable or in the Inventory. There are two settings how the Quick Slots will behave normal is AC Valhalla inspired and the "Elden Souls" by the FromSoftware Games (Setting is in the MCM under "Misc Settings">"Behavior Settings">"Elden Demon Souls")

### Can be Bound
* Weapons
* Shields
* Spells
* Shouts
* Powers
* Potions
* Lanterns (Armor Variants)
* Food
* Torches
* Scrolls
* Mask (Book of Shadow)
* Poisons

### Currently can not be bound/set
* Misc Items (e.g. Mods with Traps)

### UI
* The UI can be moved around with the MCM menu it will adjust after the Setting is closed. 
* The elements are drawn with [ImGui](https://github.com/ocornut/imgui) and the used pngs are loaded at the game start, if other assets are used the scaling might need to be adjusted in the Settings.

### Controller
* Yes there is Controller support, in the MCM you can choose between PS/XBOX prompts

### How is the Setting saved
* The Settings will be saved in an ini File. The Filename can be changed in the MCM under "Misc Settings"
* Generated [examples](https://github.com/mlthelama/LamasTinyHUD/wiki/Generated-Config-Examples)

### Settings and Checks
* Before, equipping, casting or consuming something, there is a check if the player has the item/spell.
* If a consumable hits 0, it will not be removed. The plugin checks, the bound Item has been looted, bought or dropped. (Can be Changed in the MCM via "Cleanup Settings")

## Nexusmods
* [LamasTinyHUD](https://www.nexusmods.com/skyrimspecialedition/mods/82545)

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
* [MCM Helper](https://www.nexusmods.com/skyrimspecialedition/mods/53000)

## Build Dependencies
* [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
* [spdlog](https://github.com/gabime/spdlog)
* [simpleini](https://github.com/brofield/simpleini)
* [nanosvg](https://github.com/memononen/nanosvg) (for rastering the svgfiles)
* [imgui](https://github.com/ocornut/imgui) (for displaying ui elements)
* [vcpkg](https://github.com/microsoft/vcpkg) 
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
  - used commit hash `93895b28ea7bc8cda10f156c5d336f3fc070f8b1`
* [cmake](https://cmake.org) 
  - installed dir needs to be added to the `PATH` environment variable
* [python](https://www.python.org/downloads/)
  - used for auto generating sourcelist and zip file after build

## Release Dependencies
* [Resources](https://github.com/mlthelama/ResourcesForLamasTinyHUD)

## Building 
```
git clone https://github.com/mlthelama/LamasTinyHUD.git
cd LamasTinyHUD

git submodule update --init --recursive
cmake --preset vs2022-windows
cmake --build --preset vs2022-windows --config Release
```
