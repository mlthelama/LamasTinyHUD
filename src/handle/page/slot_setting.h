#pragma once

namespace handle {
    class slot_setting {
    public:
        //unequip just makes sense with form == nullptr
        enum class acton_type : std::uint32_t { default_action = 0, instant = 1, unequip = 2 };

        enum class hand_equip : std::uint32_t { single = 0, both = 1, total = 2 };

        enum class slot_type {
            unset = -1,
            weapon = 0,
            magic = 1,
            shield = 2,
            shout = 3,
            power = 4,
            consumable = 5,
            armor = 6,
            scroll = 7,
            misc = 8
        };

        RE::TESForm* form = nullptr;
        slot_type type = slot_type::unset;
        acton_type action = acton_type::default_action;
        hand_equip equip = hand_equip::total;
        RE::BGSEquipSlot* equip_slot = nullptr;
        int32_t item_count = 0;
    };
}
