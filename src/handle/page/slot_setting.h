#pragma once
#include "util/constant.h"

namespace handle {
    class slot_setting {
    public:
        enum class acton_type : std::uint32_t { default_action = 0, instant = 1 };

        //TODO , dual = 2 move to a different setting
        enum class hand_equip : std::uint32_t { single = 0, both = 1, total = 2 };

        RE::TESForm* form = nullptr;
        util::selection_type type = util::selection_type::unset;
        acton_type action = acton_type::default_action;
        hand_equip equip = hand_equip::total;
        RE::BGSEquipSlot* equip_slot = nullptr;
    };
}
