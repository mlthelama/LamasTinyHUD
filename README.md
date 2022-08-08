# LamasTinyHUD
A small UI Element

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
* for the rest we will see


## Build Dependencies
* [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
* [spdlog](https://github.com/gabime/spdlog)
* [simpleini](https://github.com/brofield/simpleini)
* [DKUtil](https://github.com/gottyduke/DKUtil) (for easier imgui handling)
* [tomlplusplus](https://github.com/marzer/tomlplusplus) (to satisfy dkutil dependency)
* [nlohmann-json](https://github.com/nlohmann/json) (to satisfy dkutil dependency)
* [stb](https://github.com/nothings/stb) (for file reading into dx11 texture)
* [imgui](https://github.com/ocornut/imgui) (for displaying ui elements)

## Building
```
git clone https://github.com/mlthelama/LamasTinyHUD.git
cd LamasTinyHUD

git submodule update --init --recursive
cmake -B build -S .
```