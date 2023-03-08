#pragma once
#include "handle/data/data_helper.h"
#include "handle/data/page/position_setting.h"

namespace processing {
    class game_menu_setting {
    public:
        static void elden_souls_config(RE::TESForm* a_form,
            handle::position_setting::position_type a_position_type,
            bool a_overwrite);
        static void
            default_config(RE::TESForm*& a_form, handle::position_setting::position_type a_position, bool a_left);

        static uint32_t get_selected_form(RE::UI*& a_ui);
        static bool is_need_menu_open(RE::UI*& a_ui);

    private:
        static data_helper* is_suitable_for_position(RE::TESForm*& a_form,
            handle::position_setting::position_type a_position);
        static void write_notification(const std::string& a_string);
    };
}