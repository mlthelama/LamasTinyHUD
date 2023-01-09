#include "inventory.h"

namespace item {
    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        inventory::get_inventory_magic_items(RE::PlayerCharacter*& a_player) {
        return a_player->GetInventory(
            [](const RE::TESBoundObject& a_object) { return a_object.IsMagicItem(); });
    }

    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        inventory::get_inventory_weapon_items(RE::PlayerCharacter*& a_player) {
        return a_player->GetInventory(
            [](const RE::TESBoundObject& a_object) { return a_object.IsWeapon(); });
    }

    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        inventory::get_inventory_armor_items(RE::PlayerCharacter*& a_player) {
        return a_player->GetInventory(
            [](const RE::TESBoundObject& a_object) { return a_object.IsArmor(); });
    }

    bool inventory::is_item_worn(RE::TESBoundObject*& a_obj, RE::PlayerCharacter*& a_player) {
        auto worn = false;
        for (const auto& [item, inv_data] : get_inventory_armor_items(a_player)) {
            if (const auto& [count, entry] = inv_data; entry->object->formID == a_obj->formID && entry->IsWorn()) {
                worn = true;
                break;
            }
        }
        return worn;
    }
}
