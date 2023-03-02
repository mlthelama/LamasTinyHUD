#include "file_setting.h"
#include "util/constant.h"
#include <SimpleIni.h>

namespace config {
    static const char* ini_path = R"(.\Data\SKSE\Plugins\LamasTinyHUD.ini)";

    CSimpleIniA ini;

    static int log_level;
    static bool draw_key_background;

    static std::string font_file_name;
    static float font_size;
    static bool font_chinese_full;
    static bool font_chinese_simplified_common;
    static bool font_cyrillic;
    static bool font_japanese;
    static bool font_korean;
    static bool font_thai;
    static bool font_vietnamese;

    static std::string default_config;
    static std::string elden_config;

    void file_setting::load_setting() {
        logger::info("reading dll ini files");

        ini.Reset();
        ini.SetUnicode();
        ini.LoadFile(ini_path);

        log_level = ini.GetLongValue("General", "iLogLevel", 2);

        draw_key_background = ini.GetBoolValue("Image", "bDrawKeyBackground", false);

        font_file_name = ini.GetValue("Font", "sName", "");
        font_size = static_cast<float>(ini.GetDoubleValue("Font", "fSize", 20));
        font_chinese_full = ini.GetBoolValue("Font", "bChineseFull", false);
        font_chinese_simplified_common = ini.GetBoolValue("Font", "bChineseSimplifiedCommon", false);
        font_cyrillic = ini.GetBoolValue("Font", "bCyrillic", false);
        font_japanese = ini.GetBoolValue("Font", "bJapanese", false);
        font_korean = ini.GetBoolValue("Font", "bKorean", false);
        font_thai = ini.GetBoolValue("Font", "bThai", false);
        font_vietnamese = ini.GetBoolValue("Font", "bVietnamese", false);

        default_config = ini.GetValue("Config", "sDefault", (util::ini_default_name + util::ini_ending).c_str());
        elden_config = ini.GetValue("Config", "sElden", (util::ini_elden_name + util::ini_ending).c_str());

        logger::info("finished reading dll ini files. return.");
    }

    int file_setting::get_log_level() { return log_level; }
    bool file_setting::get_draw_key_background() { return draw_key_background; }

    std::string file_setting::get_font_file_name() { return font_file_name; }
    float file_setting::get_font_size() { return font_size; }
    bool file_setting::get_font_chinese_full() { return font_chinese_full; }
    bool file_setting::get_font_chinese_simplified_common() { return font_chinese_simplified_common; }
    bool file_setting::get_font_cyrillic() { return font_cyrillic; }
    bool file_setting::get_font_japanese() { return font_japanese; }
    bool file_setting::get_font_korean() { return font_korean; }
    bool file_setting::get_font_thai() { return font_thai; }
    bool file_setting::get_font_vietnamese() { return font_vietnamese; }
    std::string file_setting::get_config_default() { return default_config; }
    std::string file_setting::get_config_elden() { return elden_config; }

    void file_setting::save_setting() {
        (void)ini.SaveFile(ini_path);
        load_setting();
    }

    void file_setting::set_config_default(const std::string& a_config) {
        ini.SetValue("Config", "sDefault", a_config.c_str());
        save_setting();
    }

    void file_setting::set_config_elden(const std::string& a_config) {
        ini.SetValue("Config", "sElden", a_config.c_str());
        save_setting();
    }
}
