#pragma once

#include <SimpleIni.h>

class setting {
    inline static const char* ini_path_ = R"(.\Data\SKSE\Plugins\LamasTinyHUD.ini)";

    inline static int log_level_;


public:
    static void load_settings() {
        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path_);

        log_level_ = ini.GetLongValue("General", "iLogLevel", 2);

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

    static int get_log_level() {
        return log_level_;
    }
	
};
