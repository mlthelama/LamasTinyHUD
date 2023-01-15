#include "file_setting.h"
#include <SimpleIni.h>

namespace config {
    static const char* ini_path = R"(.\Data\SKSE\Plugins\LamasTinyHUD.ini)";

    static int log_level;
    static bool draw_key_background;

    void file_setting::load_setting() {
        logger::info("reading dll ini files");


        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path);

        log_level = ini.GetLongValue("General", "iLogLevel", 2);

        draw_key_background = ini.GetBoolValue("Image", "bDrawKeyBackground", false);

        logger::info("finished reading dll ini files. return.");
    }

    int file_setting::get_log_level() { return log_level; }
    bool file_setting::get_draw_key_background() { return draw_key_background; }
};
