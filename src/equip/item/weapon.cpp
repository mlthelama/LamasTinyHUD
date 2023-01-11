#include "weapon.h"
#include "inventory.h"
#include "equip/equip_slot.h"

namespace item {
    void weapon::equip_weapon_or_shield(const RE::TESForm* a_form,
        const RE::BGSEquipSlot* a_slot,
        RE::PlayerCharacter*& a_player,
        const bool a_weapon) {
        auto left = a_slot == equip_slot::get_left_hand_slot();
        logger::trace("try to equip {}, left {}"sv, a_form->GetName(), left);

        RE::TESBoundObject* obj = nullptr;
        RE::ExtraDataList* extra = nullptr;
        std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>> potential_items;
        if (a_weapon) {
            potential_items = inventory::get_inventory_weapon_items(a_player);
        } else {
            potential_items = inventory::get_inventory_armor_items(a_player);
        }

        for (const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                break;
            }
        }

        if (obj == nullptr) {
            logger::warn("could not find selected weapon/shield, maybe it is gone"sv);
            //update ui in this case
            return;
        }
        if (const auto equipped_object = a_player->GetEquippedObject(left);
            equipped_object && equipped_object->formID == obj->formID) {
            logger::debug("Object {} already equipped. return."sv, obj->GetName());
            return;
        }
        if (const auto equipped_object = a_player->GetEquippedObject(left);
            equipped_object && equipped_object->formID == obj->formID) {
            logger::debug("Object {} already equipped. return."sv, obj->GetName());
            return;
        }

        logger::trace("try to equip weapon/shield {}"sv, a_form->GetName());
        const auto equip_manager = RE::ActorEquipManager::GetSingleton();

        equip_manager->EquipObject(a_player, obj, extra, 1, a_slot);
        //TODO add setting for that
        //a_player->DrawWeaponMagicHands(true);
        logger::trace("equipped weapon/shield {}, left {}. return."sv, a_form->GetName(), left);
    }

    void weapon::equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to equip {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        for (const auto& [item, invData] : inventory::get_inventory_armor_items(a_player)) {
            if (invData.second->object->formID == a_form->formID) {
                obj = invData.second->object;
                break;
            }
        }

        if (obj == nullptr) {
            logger::warn("could not find selected armor, maybe it is gone"sv);
            //update ui in this case
            return;
        }
        logger::trace("try to equip weapon/shield {}"sv, a_form->GetName());
        auto equip_manager = RE::ActorEquipManager::GetSingleton();

        if (!equip_slot::unequip_if_equipped(obj, a_player, equip_manager)) {
            equip_manager->EquipObject(a_player, obj);
            logger::trace("equipped armor {}. return."sv, a_form->GetName());
        }
    }
}
