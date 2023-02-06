#include "item.h"
#include "equip/equip_slot.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/string_util.h"

namespace equip {
    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        item::get_inventory_weapon_items(RE::PlayerCharacter*& a_player) {
        return a_player->GetInventory([](const RE::TESBoundObject& a_object) { return a_object.IsWeapon(); });
    }

    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        item::get_inventory_armor_items(RE::PlayerCharacter*& a_player) {
        return a_player->GetInventory([](const RE::TESBoundObject& a_object) { return a_object.IsArmor(); });
    }

    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        item::get_inventory(RE::PlayerCharacter*& a_player, RE::FormType a_type) {
        return a_player->GetInventory([a_type](const RE::TESBoundObject& a_object) { return a_object.Is(a_type); });
    }

    bool item::is_item_worn(RE::TESBoundObject*& a_obj, RE::PlayerCharacter*& a_player) {
        auto worn = false;
        for (const auto& [item, inv_data] : get_inventory_armor_items(a_player)) {
            if (const auto& [count, entry] = inv_data; entry->object->formID == a_obj->formID && entry->IsWorn()) {
                worn = true;
                break;
            }
        }
        return worn;
    }

    void item::equip_item(const RE::TESForm* a_form,
        RE::BGSEquipSlot*& a_slot,
        RE::PlayerCharacter*& a_player,
        handle::slot_setting::slot_type a_type) {
        auto left = a_slot == equip_slot::get_left_hand_slot();
        logger::trace("try to equip {}, left {}, type {}"sv, a_form->GetName(), left, static_cast<uint32_t>(a_type));

        if (a_form->formID == util::unarmed) {
            logger::trace("Got unarmed, try to call un equip"sv);
            equip_slot::un_equip_hand(a_slot, a_player, handle::slot_setting::acton_type::un_equip);
            return;
        }

        RE::TESBoundObject* obj = nullptr;
        RE::ExtraDataList* extra = nullptr;
        std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>> potential_items;
        if (a_type == handle::slot_setting::slot_type::weapon) {
            if (!a_form->Is(RE::FormType::Weapon)) {
                logger::warn("object {} is not a weapon. return."sv, a_form->GetName());
                return;
            }
            potential_items = get_inventory_weapon_items(a_player);
        } else if (a_type == handle::slot_setting::slot_type::shield) {
            if (!a_form->Is(RE::FormType::Armor)) {
                logger::warn("object {} is not an armor. return."sv, a_form->GetName());
                return;
            }
            potential_items = get_inventory_armor_items(a_player);
        } else if (a_type == handle::slot_setting::slot_type::light) {
            if (!a_form->Is(RE::FormType::Light)) {
                logger::warn("object {} is not a light. return."sv, a_form->GetName());
                return;
            }
            potential_items = get_inventory(a_player, RE::FormType::Light);
        }

        auto item_count = 0;
        for (const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                item_count = num_items;
                break;
            }
        }

        if (obj == nullptr) {
            logger::warn("could not find selected weapon/shield, maybe it is gone"sv);
            //update ui in this case
            return;
        }
        const auto obj_right = a_player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        const auto obj_left = a_player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        if (left && obj_left && obj_left->formID == obj->formID) {
            logger::debug("Object Left {} is already where it should be already equipped. return."sv, obj->GetName());
            return;
        }
        if (!left && obj_right && obj_right->formID == obj->formID) {
            logger::debug("Object Right {} is already where it should be already equipped. return."sv, obj->GetName());
            return;
        }

        auto equipped_count = 0;
        if (obj_right && obj_right->formID == obj->formID) {
            equipped_count++;
            logger::debug("Object {} already equipped."sv, obj->GetName());
        }

        if (obj_left && obj_left->formID == obj->formID) {
            equipped_count++;
            logger::debug("Object {} already equipped."sv, obj->GetName());
        }

        logger::trace("Got a count of {} in the Inventory {}, Equipped {}"sv,
            obj->GetName(),
            item_count,
            equipped_count);
        if (item_count == equipped_count) {
            //all we have are already equipped
            logger::warn("All Items we have of {} are equipped, return."sv, obj->GetName());
            //try to prevent the game to equip something else
            equip_slot::un_equip_hand(a_slot, a_player, handle::slot_setting::acton_type::un_equip);
            return;
        }

        logger::trace("try to equip weapon/shield/light {}"sv, a_form->GetName());
        const auto equip_manager = RE::ActorEquipManager::GetSingleton();

        equip_manager->EquipObject(a_player, obj, extra, 1, a_slot);
        //TODO add setting for that
        //a_player->DrawWeaponMagicHands(true);
        logger::trace("equipped weapon/shield/light {}, left {}. return."sv, a_form->GetName(), left);
    }

    void item::equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to equip {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        auto item_count = 0;
        for (const auto& [item, inv_data] : get_inventory_armor_items(a_player)) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = inv_data.second->object;
                item_count = num_items;
                break;
            }
        }

        if (!obj || item_count == 0) {
            logger::warn("could not find selected armor, maybe it is gone"sv);
            //update ui in this case
            return;
        }
        logger::trace("try to equip armor/clothing {}"sv, a_form->GetName());

        if (auto equip_manager = RE::ActorEquipManager::GetSingleton();
            !equip_slot::un_equip_if_equipped(obj, a_player, equip_manager)) {
            equip_manager->EquipObject(a_player, obj);
            logger::trace("equipped armor {}. return."sv, a_form->GetName());
        }
    }

    void item::consume_potion(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to consume {}"sv, a_form->GetName());


        RE::TESBoundObject* obj = nullptr;
        uint32_t left = 0;
        for (auto potential_items = get_inventory(a_player, RE::FormType::AlchemyItem);
             const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                left = inv_data.first;
                break;
            }
        }


        if (config::mcm_setting::get_prevent_consumption_of_last_dynamic_potion() && obj && obj->IsDynamicForm() &&
            left == 1) {
            logger::warn(
                "Somehow the game crashes on potions with dynamic id if the count is 0 (happens with or without the mod). So I am not consuming it. Form {}, Name {}"sv,
                util::string_util::int_to_hex(obj->formID),
                obj->GetName());
            return;
        }

        if (!obj || left == 0) {
            logger::warn("could not find selected potion, maybe all have been consumed"sv);
            //TODO update ui in this case
            return;
        }

        if (!obj->Is(RE::FormType::AlchemyItem)) {
            logger::warn("object {} is not an alchemy item. return."sv, obj->GetName());
            return;
        }

        logger::trace("calling drink/eat potion/food {}, count left {}"sv, obj->GetName(), left);

        const auto equip_manager = RE::ActorEquipManager::GetSingleton();
        equip_manager->EquipObject(a_player, obj);
        logger::trace("drank/ate potion/food {}. return."sv, obj->GetName());
    }

    void item::equip_ammo(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to equip {}"sv, a_form->GetName());


        RE::TESBoundObject* obj = nullptr;
        auto left = 0;
        for (auto potential_items = get_inventory(a_player, RE::FormType::Ammo);
             const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                left = inv_data.first;
                break;
            }
        }

        if (!obj || left == 0) {
            logger::warn("could not find selected ammo, maybe all have been used"sv);
            return;
        }

        if (const auto current_ammo = a_player->GetCurrentAmmo(); current_ammo && current_ammo->formID == obj->formID) {
            logger::debug("Ammo {} already equipped, return."sv, obj->GetName());
            return;
        }

        const auto equip_manager = RE::ActorEquipManager::GetSingleton();
        equip_manager->EquipObject(a_player, obj);
        logger::trace("equipped {}. return."sv, obj->GetName());
    }

    uint32_t item::get_inventory_count(const RE::TESForm* a_form) {
        uint32_t count = 0;
        if (!a_form) {
            return count;
        }
        auto player = RE::PlayerCharacter::GetSingleton();
        if (a_form->IsWeapon()) {
            auto weapons = get_inventory(player, RE::FormType::Weapon);
            for (const auto& [item, inv_data] : weapons) {
                if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                    count = num_items;
                    break;
                }
            }
        } else if (a_form->IsArmor()) {
            auto armor = get_inventory(player, RE::FormType::Armor);
            for (const auto& [item, inv_data] : armor) {
                if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                    count = num_items;
                    break;
                }
            }
        }
        logger::trace("got {} in inventory for item {}"sv, count, a_form->GetName());

        return count;
    }
    void item::un_equip_ammo() {
        logger::debug("check if we need to un equip ammo"sv);
        auto player = RE::PlayerCharacter::GetSingleton();
        auto obj = player->GetCurrentAmmo();
        if (!obj || !obj->IsAmmo()) {
            return;
        }

        auto ammo = obj->As<RE::TESAmmo>();
        if (ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt) ||
            ammo->GetRuntimeData().data.flags.none(RE::AMMO_DATA::Flag::kNonBolt)) {
            auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->UnequipObject(player, ammo);
            logger::trace("Called to un equip {}"sv, ammo->GetName());
        }
        logger::trace("Done work. return"sv);
    }
}
