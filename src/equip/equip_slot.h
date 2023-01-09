#pragma once

namespace item {
    class equip_slot {
    public:
        static RE::BGSEquipSlot* get_right_hand_slot();
        static RE::BGSEquipSlot* get_left_hand_slot();

        static void unequip_if_equipped(bool a_left,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);

        static bool unequip_if_equipped(RE::TESBoundObject*& a_obj,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);
    };
}
