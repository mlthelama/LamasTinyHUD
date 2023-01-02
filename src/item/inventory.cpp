#include "inventory.h"

namespace item {
    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        inventory::get_inventory_magic_items() {
        return RE::PlayerCharacter::GetSingleton()->GetInventory(
            [](const RE::TESBoundObject& a_object) { return a_object.IsMagicItem(); });
    }

    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        inventory::get_inventory_weapon_items() {
        return RE::PlayerCharacter::GetSingleton()->GetInventory(
            [](const RE::TESBoundObject& a_object) { return a_object.IsWeapon(); });
    }
}
