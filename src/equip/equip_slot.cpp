#include "equip_slot.h"
#include "util/offset.h"

namespace equip {
    RE::BGSEquipSlot* equip_slot::get_right_hand_slot() {
        using func_t = decltype(&get_right_hand_slot);
        const REL::Relocation<func_t> func{REL::ID(offset::get_right_hand_slot)};
        return func();
    }

    RE::BGSEquipSlot* equip_slot::get_left_hand_slot() {
        using func_t = decltype(&get_left_hand_slot);
        const REL::Relocation<func_t> func{REL::ID(offset::get_left_hand_slot)};
        return func();
    }
}
