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
}
