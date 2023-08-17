#pragma once

namespace config {
    class file_setting {
    public:
        static void load_setting();

        static bool get_is_debug();

        static bool get_draw_key_background();
        static std::string get_image_file_ending();
        static std::string get_key_file_ending();

        static bool get_font_load();
        static std::string get_font_file_name();
        static float get_font_size();
        static bool get_font_chinese_full();
        static bool get_font_chinese_simplified_common();
        static bool get_font_cyrillic();
        static bool get_font_japanese();
        static bool get_font_korean();
        static bool get_font_thai();
        static bool get_font_vietnamese();

        static std::string get_config_default();
        static std::string get_config_elden();

        static void set_config_default(const std::string& a_config);
        static void set_config_elden(const std::string& a_config);

        static bool get_show_ui();
        static void set_show_ui(bool a_show);

    private:
        static void save_setting();
    };
}
