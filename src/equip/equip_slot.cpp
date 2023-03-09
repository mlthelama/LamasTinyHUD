#include "equip_slot.h"
#include "util/offset.h"
#include "util/player/player.h"
#include "util/string_util.h"

namespace equip {
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

    RE::BGSEquipSlot* equip_slot::get_voice_slot() {
        using func_t = decltype(&get_voice_slot);
        const REL::Relocation<func_t> func{ REL::ID(offset::get_voice_slot) };
        return func();
    }

    bool equip_slot::un_equip_if_equipped(RE::TESBoundObject*& a_obj,
        RE::PlayerCharacter*& a_player,
        RE::ActorEquipManager*& a_actor_equip_manager) {
        const auto is_worn = is_item_worn(a_obj, a_player);
        if (is_worn) {
            a_actor_equip_manager->UnequipObject(a_player, a_obj);
            logger::trace("unequipped {} armor"sv, a_obj->GetName());
        }
        return is_worn;
    }

    void equip_slot::un_equip_hand(RE::BGSEquipSlot*& a_slot,
        RE::PlayerCharacter*& a_player,
        const action_type a_action) {
        if (a_action != handle::slot_setting::action_type::un_equip) {
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
                un_equip_object_ft_dummy_dagger(a_slot, a_player, equip_manager);
                did_call = true;
            }

            if (equipped_object->Is(RE::FormType::Light)) {
                const auto light = equipped_object->As<RE::TESObjectLIGH>();
                equip_manager->UnequipObject(a_player, light, nullptr, 1, a_slot);
                did_call = true;
            }

            logger::trace("called un equip for {}, left {}, did call {}"sv,
                equipped_object->GetName(),
                a_slot == get_left_hand_slot(),
                did_call);
        }
    }

    void equip_slot::un_equip_object_ft_dummy_dagger(RE::BGSEquipSlot*& a_slot,
        RE::PlayerCharacter*& a_player,
        RE::ActorEquipManager*& a_actor_equip_manager) {
        const auto dummy = RE::TESForm::LookupByID<RE::TESForm>(0x00020163)->As<RE::TESObjectWEAP>();
        //sound false, queue false, force true
        a_actor_equip_manager->EquipObject(a_player, dummy, nullptr, 1, a_slot, false, true, false);
        a_actor_equip_manager->UnequipObject(a_player, dummy, nullptr, 1, a_slot, false, true, false);
    }

    void equip_slot::un_equip_spell(RE::BSScript::IVirtualMachine* a_vm,
        RE::VMStackID a_stack_id,
        RE::Actor* a_actor,
        RE::SpellItem* a_spell,
        uint32_t a_slot) {
        using func_t = decltype(&un_equip_spell);
        const REL::Relocation<func_t> func{ REL::ID(offset::get_un_equip_spell) };
        func(a_vm, a_stack_id, a_actor, a_spell, a_slot);
    }

    void equip_slot::un_equip_shout(RE::BSScript::IVirtualMachine* a_vm,
        RE::VMStackID a_stack_id,
        RE::Actor* a_actor,
        RE::TESShout* a_shout) {
        using func_t = decltype(&un_equip_shout);
        const REL::Relocation<func_t> func{ REL::ID(offset::get_un_equip_shout) };
        func(a_vm, a_stack_id, a_actor, a_shout);
    }

    void equip_slot::un_equip_shout_slot(RE::PlayerCharacter*& a_player) {
        auto selected_power = a_player->GetActorRuntimeData().selectedPower;
        if (selected_power) {
            logger::trace("Equipped form is {}, try to un equip"sv,
                util::string_util::int_to_hex(selected_power->formID));
            if (selected_power->Is(RE::FormType::Shout)) {
                equip::equip_slot::un_equip_shout(nullptr, 0, a_player, selected_power->As<RE::TESShout>());
            } else if (selected_power->Is(RE::FormType::Spell)) {
                //power
                //2=other
                equip::equip_slot::un_equip_spell(nullptr, 0, a_player, selected_power->As<RE::SpellItem>(), 2);
            }
        }
    }

    bool equip_slot::is_item_worn(RE::TESBoundObject*& a_obj, RE::PlayerCharacter*& a_player) {
        auto worn = false;
        for (const auto& [item, inv_data] : util::player::get_inventory(a_player, RE::FormType::Armor)) {
            if (const auto& [count, entry] = inv_data; entry->object->formID == a_obj->formID && entry->IsWorn()) {
                worn = true;
                break;
            }
        }
        return worn;
    }
}
