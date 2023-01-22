#pragma once

namespace equip {
    class item {
    public:
        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
            get_inventory_weapon_items(RE::PlayerCharacter*& a_player);

        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
            get_inventory_armor_items(RE::PlayerCharacter*& a_player);

        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
            get_inventory(RE::PlayerCharacter*& a_player, RE::FormType a_type);

        static bool is_item_worn(RE::TESBoundObject*& a_obj, RE::PlayerCharacter*& a_player);

        static void equip_weapon_or_shield(const RE::TESForm* a_form,
            const RE::BGSEquipSlot* a_slot,
            RE::PlayerCharacter*& a_player,
            bool a_weapon = true);

        static void equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);

        static void consume_potion(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);
    };
}
