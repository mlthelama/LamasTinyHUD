#include "equip_slot.h"

#include "item/inventory.h"
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

    void equip_slot::unequip_if_equipped(const bool a_left,
        RE::PlayerCharacter*& a_player,
        RE::ActorEquipManager*& a_actor_equip_manager) {
        if (const auto inv_entry = a_player->GetEquippedEntryData(a_left); inv_entry != nullptr) {
            logger::trace("Item {} is equipped, unequipping"sv, inv_entry->GetDisplayName());
            a_actor_equip_manager->UnequipObject(a_player, inv_entry->object);
        }

        if (const auto object = a_player->GetEquippedObject(a_left); object != nullptr) {
            logger::trace("Object {} is equipped, unequipping"sv, object->GetName());
            const auto bound_object = object->As<RE::TESBoundObject>();
            a_actor_equip_manager->UnequipObject(a_player, bound_object);
        }
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
}
