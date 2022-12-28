#include "file_setting.h"
#include <SimpleIni.h>

namespace config {
    static const char* ini_path = R"(.\Data\SKSE\Plugins\LamasTinyHUD.ini)";

    static int log_level;

    void file_setting::load_setting() {
        logger::info("reading dll ini files");


        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path);

        log_level = ini.GetLongValue("General", "iLogLevel", 2);


        logger::info("finished reading dll ini files");
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

    int file_setting::get_log_level() {
        return log_level;
    }
};
