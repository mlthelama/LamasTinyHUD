#pragma once
#include "handle/data/page/slot_setting.h"

namespace equip {
    class item {
    public:
        static void equip_item(const RE::TESForm* a_form,
            RE::BGSEquipSlot*& a_slot,
            RE::PlayerCharacter*& a_player,
            handle::slot_setting::slot_type a_type);
        static void equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);
        static void consume_potion(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);
        static void equip_ammo(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);
        static void un_equip_ammo();
        static void find_and_consume_fitting_option(RE::ActorValue a_actor_value, RE::PlayerCharacter*& a_player);

    private:
        static void poison_weapon(RE::PlayerCharacter*& a_player, RE::AlchemyItem*& a_poison, uint32_t a_count);
    };
}
