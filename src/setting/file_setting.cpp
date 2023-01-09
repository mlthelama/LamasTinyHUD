#include "file_setting.h"
#include <SimpleIni.h>

namespace config {
    static const char* ini_path = R"(.\Data\SKSE\Plugins\LamasTinyHUD.ini)";

    static int log_level;
    static float extra_size_for_image;
    static bool draw_key_background;

    void file_setting::load_setting() {
        logger::info("reading dll ini files");


        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path);

        log_level = ini.GetLongValue("General", "iLogLevel", 2);

        extra_size_for_image = static_cast<float>(ini.GetDoubleValue("Image", "fExtraSizeForImage", 1));
        draw_key_background = ini.GetBoolValue("Image", "bDrawKeyBackground", false);

        logger::info("finished reading dll ini files. return.");
    }

    /*static void set_show_window(const bool a_show_value) {
        logger::debug("setting new value for {} is {}"sv, "bShowWindow", a_show_value);
        show_window_ = a_show_value;

        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path_);

        ini.SetBoolValue("Toggle", "bShowWindow", a_show_value);

        (void)ini.SaveFile(ini_path_);
    }*/

    int file_setting::get_log_level() { return log_level; }
    float file_setting::get_extra_size_for_image() { return extra_size_for_image; }
    bool file_setting::get_draw_key_background() { return draw_key_background; }
};
