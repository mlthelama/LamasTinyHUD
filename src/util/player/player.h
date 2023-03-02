#pragma once
#include "handle/data/data_helper.h"

namespace util {

    class player {
    public:
        static std::vector<RE::TESForm*> get_spells(bool a_instant, bool a_single);
        static std::vector<RE::TESForm*> get_powers();
        static std::vector<RE::TESForm*> get_shouts();
        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
            get_inventory_weapon_items(RE::PlayerCharacter*& a_player);
        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
            get_inventory_armor_items(RE::PlayerCharacter*& a_player);
        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
            get_inventory(RE::PlayerCharacter*& a_player, RE::FormType a_type);
        static bool is_item_worn(RE::TESBoundObject*& a_obj, RE::PlayerCharacter*& a_player);
        static uint32_t get_inventory_count(const RE::TESForm* a_form);
        static std::vector<data_helper*> get_hand_assignment(RE::TESForm*& a_form);
        static std::vector<data_helper*> get_hand_assignment(bool a_two_handed = false);
        static void look_for_ammo(bool a_crossbow);
    };
}  // util
