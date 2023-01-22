#pragma once
#include "handle/data/page/slot_setting.h"

namespace equip {
    class equip_slot {
    public:
        using action_type = handle::slot_setting::acton_type;

        static RE::BGSEquipSlot* get_right_hand_slot();
        static RE::BGSEquipSlot* get_left_hand_slot();

        static bool unequip_if_equipped(RE::TESBoundObject*& a_obj,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);

        static void unequip_hand(const RE::BGSEquipSlot* a_slot, RE::PlayerCharacter*& a_player, action_type a_action);
        static void unequip_object_ft_dummy_dagger(const RE::BGSEquipSlot*& a_slot,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);

        //not used
        static void equip_unarmed(RE::PlayerCharacter*& a_player);
        //not used
        static void unequip_spell_by_slot(RE::Actor* a_actor, RE::SpellItem* a_spell, const RE::BGSEquipSlot* a_slot);
        //not used
        // 0 - Left hand,  1 - Right hand
        static void unequip_spell(RE::BSScript::IVirtualMachine* a_vm,
            RE::VMStackID a_stack_id,
            RE::Actor* a_actor,
            RE::SpellItem* a_spell,
            uint32_t a_slot);
    };
}
