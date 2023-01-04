#pragma once

namespace equip {
    class equip_slot {
    public:
        static RE::BGSEquipSlot* get_right_hand_slot();
        static RE::BGSEquipSlot* get_left_hand_slot();
    };
}
