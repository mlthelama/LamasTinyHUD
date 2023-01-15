#pragma once
#include "handle/page/slot_setting.h"

namespace item {
    class equip_slot {
    public:
        using action_type = handle::slot_setting::acton_type;

        static RE::BGSEquipSlot* get_right_hand_slot();
        static RE::BGSEquipSlot* get_left_hand_slot();

        static bool unequip_if_equipped(RE::TESBoundObject*& a_obj,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);
        static void unequip_hand(const RE::BGSEquipSlot* a_slot, RE::PlayerCharacter*& a_player, action_type a_action);
    };
}
