# LamasTinyHUD
A small UI Element

## What does it do
Allows you to bind Weapons, Shields, Spells, Shouts, Powers and Potions/Food to a Hotkey. In order to bind it to a Hotkey the Item needs to be favorited. Before every Execute, it is checked if the Item is still avaliable or in the Inventory.
* Potions/Food will be consumed on the Hotkey
* Powers can be equipped or instant casted with the Hotkey. Instant casting will not replace an already slotted Power or Shout.
* Shouts can only be equipped. Equip will be skipped if it is already slotted.
* Spells, Weapons and Shields can be combined in one Hotkey and will always just be equipped, current equipped Items will be unequipped before.
* Spells can be instant casted, but they cost currently no magicka but give experience

The UI can be moved around with the MCM menu it will adjust after the Setting is closed. The elements are drawn with [ImGui](https://github.com/ocornut/imgui) and the used pngs are loaded at the game start, if other assets are used the scaling might need to be adjusted in the Settings.


## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
* [MCM Helper](https://www.nexusmods.com/skyrimspecialedition/mods/53000)
* for the rest we will see


## Build Dependencies
* [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
* [spdlog](https://github.com/gabime/spdlog)
* [simpleini](https://github.com/brofield/simpleini)
* [stb](https://github.com/nothings/stb) (for file reading into dx11 texture)
* [imgui](https://github.com/ocornut/imgui) (for displaying ui elements)
* [vcpkg](https://github.com/microsoft/vcpkg) 
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [cmake](https://cmake.org) 
  - installed dir needs to be added to the `PATH` environment variable

## Icon Sets

Icon Set|Folder|License|Count
---|---|---|---
[Xelu_Free_Controller&Key_Prompts](https://thoseawesomeguys.com/prompts/)|resources/key|[Creative Commons 0 (CC0)](https://creativecommons.org/publicdomain/zero/1.0/)|85
[Game-Icons](https://game-icons.net)|resources/icon|[CC BY 3.0](https://creativecommons.org/licenses/by/3.0/)|23
none|resources/hud|drawn by me|3

### More info for Game-Icons
File|Link
---|---
bow.png |[source](https://game-icons.net/1x1/lorc/high-shot.html)
crossbow.png |[source](https://game-icons.net/1x1/carl-olsen/crossbow.html)
dagger.png |[source](https://game-icons.net/1x1/lorc/sacrificial-dagger.html)
default.png |[source](https://game-icons.net/1x1/delapouite/perspective-dice-six-faces-random.html)
default_potion.png |[source](https://game-icons.net/1x1/delapouite/magic-potion.html)
default_spell.png |[source](https://game-icons.net/1x1/lorc/rolling-energy.html)
default_spell_alt.png |[source](https://game-icons.net/1x1/lorc/crowned-explosion.html)
food.png |[source](https://game-icons.net/1x1/delapouite/bread.html)
health_potion.png |[source](https://game-icons.net/1x1/caro-asercion/round-potion.html)
mace.png |[source](https://game-icons.net/1x1/lorc/spiked-mace.html)
one_handed_axe.png |[source](https://game-icons.net/1x1/lorc/battered-axe.html)
one_handed_sword.png |[source](https://game-icons.net/1x1/skoll/stiletto.html)
power.png |[source](https://game-icons.net/1x1/lorc/embrassed-energy.html)
shield.png |[source](https://game-icons.net/1x1/willdabeast/round-shield.html)
shout.png |[source](https://game-icons.net/1x1/lorc/shouting.html)
staff.png |[source](https://game-icons.net/1x1/lorc/wizard-staff.html)
two_handed_axe.png |[source](https://game-icons.net/1x1/lorc/battle-axe.html)
two_handed_sword.png |[source](https://game-icons.net/1x1/lorc/broadsword.html)
fire_spell.png|[source](https://game-icons.net/1x1/delapouite/fire-spell-cast.html)
ice_spell.png|[source](https://game-icons.net/1x1/delapouite/ice-spell-cast.html)
shock_spell.png|[source](https://game-icons.net/1x1/delapouite/bolt-spell-cast.html)
heal_spell.png|[source](https://game-icons.net/1x1/delapouite/healing.html)
poison_default.png|[source](https://game-icons.net/1x1/lorc/poison-bottle.html)

## Building
```
git clone https://github.com/mlthelama/LamasTinyHUD.git
cd LamasTinyHUD

git submodule update --init --recursive
cmake -B build -S .
```
