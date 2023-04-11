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
Allows you to bind Weapons, Shields, Spells, Shouts, Powers and Potions/Food to a Hotkey. In order to bind it to a Hotkey the Item needs to be favorited. Before every Execute, it is checked if the Item is still avaliable or in the Inventory.

### Can be Bound
* Potions/Food will be consumed on the Hotkey
* Powers can be equipped or instant casted with the Hotkey. Instant casting will not replace an already slotted Power or Shout.
* Shouts can only be equipped. Equip will be skipped if it is already slotted.
* Spells, Weapons and Shields can be combined in one Hotkey and will always just be equipped, current equipped Items will be unequipped before.
* Spells that do not need Concentration (=Button holding) can be instant casted, they do not have an animation currently. 
  The Spell will just appear or shoot out of the hand it was set to (still it does not equip it). It is also possible just to equip them.

### Currently can not be bound/set
* Scrolls
* Misc Items (e.g. Mods with Traps)

### UI
* The UI can be moved around with the MCM menu it will adjust after the Setting is closed. 
* The elements are drawn with [ImGui](https://github.com/ocornut/imgui) and the used pngs are loaded at the game start, if other assets are used the scaling might need to be adjusted in the Settings.

### Controller
* Yes there is Controller support, in the MCM you can choose between PS/XBOX prompts

### How is the Setting saved
* It will be saved by the [MCM Helper](https://www.nexusmods.com/skyrimspecialedition/mods/53000) under MCM\Settings\LamasTinyHUD.ini
* Generated [examples](https://github.com/mlthelama/LamasTinyHUD/wiki/Generated-Config-Examples)

### Settings and Checks
* Only favourited will be shown in the Selections in the MCM.
* After the Setting is done, they do not need to be favorited anymore.
* Before, equipping, casting or consuming something, there is a check if the player has the item/spell.
* If a consumable hits 0, it will not be removed. The plugin checks, the bound Item has been looted, bought or dropped.

## Nexusmods
* [ LamasTinyHUD](https://www.nexusmods.com/skyrimspecialedition/mods/82545)

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
