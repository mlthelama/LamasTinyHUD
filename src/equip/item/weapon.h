#pragma once

namespace item {
    class weapon {
    public:
        static void equip_weapon_or_shield(const RE::TESForm* a_form,
            const RE::BGSEquipSlot* a_slot,
            RE::PlayerCharacter*& a_player,
            bool a_weapon = true);

        static void equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player);
    };
}
