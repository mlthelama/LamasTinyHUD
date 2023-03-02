#pragma once
#include "handle/data/page/position_setting.h"

namespace handle {
    class game_menu_setting {
    public:
        static void elden_souls_config(RE::TESForm* a_form,
            handle::position_setting::position_type a_position_type,
            bool a_overwrite);
        static void
            default_config(RE::TESForm*& a_form, handle::position_setting::position_type a_position, bool a_left);
    };
}
