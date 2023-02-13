#pragma once
#include <handle/data/page/slot_setting.h>

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

        static void equip_item(const RE::TESForm* a_form,
            RE::BGSEquipSlot*& a_slot,
            RE::PlayerCharacter*& a_player,
            handle::slot_setting::slot_type a_type);

        static void equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);

        static void consume_potion(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);

        static void equip_ammo(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);

        static void un_equip_ammo();

        static uint32_t get_inventory_count(const RE::TESForm* a_form);

        static void find_and_consume_fitting_option(RE::ActorValue a_actor_value, RE::PlayerCharacter*& a_player);
    };
}
