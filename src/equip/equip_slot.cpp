#include "equip_slot.h"
#include "item/inventory.h"
#include "magic/spell.h"
#include "util/offset.h"

namespace item {
    RE::BGSEquipSlot* equip_slot::get_right_hand_slot() {
        using func_t = decltype(&get_right_hand_slot);
        const REL::Relocation<func_t> func{ REL::ID(offset::get_right_hand_slot) };
        return func();
    }

    RE::BGSEquipSlot* equip_slot::get_left_hand_slot() {
        using func_t = decltype(&get_left_hand_slot);
        const REL::Relocation<func_t> func{ REL::ID(offset::get_left_hand_slot) };
        return func();
    }

    bool equip_slot::unequip_if_equipped(RE::TESBoundObject*& a_obj,
        RE::PlayerCharacter*& a_player,
        RE::ActorEquipManager*& a_actor_equip_manager) {
        const auto is_worn = inventory::is_item_worn(a_obj, a_player);
        if (is_worn) {
            a_actor_equip_manager->UnequipObject(a_player, a_obj);
            logger::trace("unequipped {} armor"sv, a_obj->GetName());
        }
        return is_worn;
    }

    void equip_slot::unequip_hand(const RE::BGSEquipSlot* a_slot,
        RE::PlayerCharacter*& a_player,
        const action_type a_action) {
        if (a_action != handle::slot_setting::acton_type::unequip) {
            return;
        }
        RE::TESForm* equipped_object = nullptr;
        if (a_slot == get_left_hand_slot()) {
            equipped_object = a_player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        }
        if (a_slot == get_right_hand_slot()) {
            equipped_object = a_player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        }

        if (equipped_object) {
            logger::debug("Object {} is equipped, is left {}."sv,
                equipped_object->GetName(),
                a_slot == get_left_hand_slot());
            bool did_call = false;
            auto equip_manager = RE::ActorEquipManager::GetSingleton();
            if (equipped_object->IsWeapon()) {
                const auto weapon = equipped_object->As<RE::TESObjectWEAP>();
                equip_manager->UnequipObject(a_player, weapon, nullptr, 1, a_slot);
                did_call = true;
            }
            if (equipped_object->Is(RE::FormType::Armor)) {
                if (const auto armor = equipped_object->As<RE::TESObjectARMO>(); armor->IsShield()) {
                    equip_manager->UnequipObject(a_player, armor, nullptr, 1, a_slot);
                    did_call = true;
                }
            }
            if (equipped_object->Is(RE::FormType::Spell)) {
                //well since the spell can be "stuck" or the effect, lets go dummy dagger
                //unequip_object_ft_dummy_dagger(a_slot, a_player, equip_manager);
                const auto hand = RE::TESForm::LookupByID<RE::TESForm>(0x000001F4)->As<RE::TESObjectWEAP>();
                equip_manager->EquipObject(a_player, hand, nullptr, 1, a_slot);
                did_call = true;
            }
            logger::trace("called unequip for {}, left {}, did call {}"sv,
                equipped_object->GetName(),
                a_slot == get_left_hand_slot(),
                did_call);
        }
    }

    void equip_slot::unequip_object_ft_dummy_dagger(const RE::BGSEquipSlot*& a_slot,
        RE::PlayerCharacter*& a_player,
        RE::ActorEquipManager*& a_actor_equip_manager) {
        const auto dummy = RE::TESForm::LookupByID<RE::TESForm>(0x00020163)->As<RE::TESObjectWEAP>();
        a_actor_equip_manager->EquipObject(a_player, dummy, nullptr, 1, a_slot);
        a_actor_equip_manager->UnequipObject(a_player, dummy, nullptr, 1, a_slot);
    }

    /*void equip_slot::unequip_spell(RE::BSScript::IVirtualMachine* a_vm,
        RE::VMStackID a_stack_id,
        RE::Actor* a_actor,
        RE::SpellItem* a_spell,
        uint32_t a_slot) {
        using func_t = decltype(&unequip_spell);
        REL::Relocation<func_t> func{ RELOCATION_ID(227783, 54666) };
        func(a_vm, a_stack_id, a_actor, a_spell, a_slot);
    }*/
}
