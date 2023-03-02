#pragma once
#include "handle/data/page/slot_setting.h"

namespace equip {
    class equip_slot {
    public:
        using action_type = handle::slot_setting::acton_type;

        static RE::BGSEquipSlot* get_right_hand_slot();
        static RE::BGSEquipSlot* get_left_hand_slot();
        static RE::BGSEquipSlot* get_voice_slot();
        static bool un_equip_if_equipped(RE::TESBoundObject*& a_obj,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);
        static void un_equip_hand(RE::BGSEquipSlot*& a_slot, RE::PlayerCharacter*& a_player, action_type a_action);
        static void un_equip_object_ft_dummy_dagger(RE::BGSEquipSlot*& a_slot,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);
        // 0 - Left hand,  1 - Right hand, 2 Other
        static void un_equip_spell(RE::BSScript::IVirtualMachine* a_vm,
            RE::VMStackID a_stack_id,
            RE::Actor* a_actor,
            RE::SpellItem* a_spell,
            uint32_t a_slot);
        static void un_equip_shout(RE::BSScript::IVirtualMachine* a_vm,
            RE::VMStackID a_stack_id,
            RE::Actor* a_actor,
            RE::TESShout* a_shout);
        static void un_equip_shout_slot(RE::PlayerCharacter*& a_player);
    };
}
