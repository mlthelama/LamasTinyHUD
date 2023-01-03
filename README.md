# LamasTinyHUD
A small UI Element

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

## Building
```
git clone https://github.com/mlthelama/LamasTinyHUD.git
cd LamasTinyHUD

git submodule update --init --recursive
cmake -B build -S .
```
