#include "display_tweaks.h"

namespace setting {
    static const char* default_ini_path = R"(.\Data\SKSE\Plugins\SSEDisplayTweaks.ini)";
    static const char* user_ini_path = R"(.\Data\SKSE\Plugins\SSEDisplayTweaks_Custom.ini)";
    
    static float resolution_scale;
    static bool borderless_upscale;
    
    void display_tweaks::load_setting() {
        logger::info("reading ini files");
        const auto read_config = [&](const std::filesystem::path& path) {
            CSimpleIniA ini;
            ini.SetUnicode();
            ini.LoadFile(path.string().c_str());
            
            resolution_scale = static_cast<float>(ini.GetDoubleValue("Render", "ResolutionScale", 1.0f));
            borderless_upscale = ini.GetBoolValue("Render", "BorderlessUpscale", false);
        };
        
        read_config(default_ini_path);
        read_config(user_ini_path);
        
        logger::info("finished reading ini files. return.");
    }
    
    float display_tweaks::get_resolution_scale() { return resolution_scale; }
    
    bool display_tweaks::get_borderless_upscale() { return borderless_upscale; }

}  // config
