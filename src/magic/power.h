#pragma once
#include "handle/page/slot_setting.h"

namespace magic {
    class power {
    public:
        using action_type = handle::slot_setting::acton_type;

        static std::vector<RE::TESForm*> get_powers();
        static void equip_or_cast_power(RE::TESForm* a_form, action_type a_action);
    };
}
