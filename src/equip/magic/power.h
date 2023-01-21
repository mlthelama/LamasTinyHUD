#pragma once
#include "handle/data/page/slot_setting.h"

namespace magic {
    class power {
    public:
        using action_type = handle::slot_setting::acton_type;

        //not used
        static std::vector<RE::TESForm*> get_powers();
        static void equip_or_cast_power(RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player);
    };
}
