#pragma once
namespace item {
    class weapon {
    public:
        static void equip_weapon(const RE::TESForm* a_form,
            const RE::BGSEquipSlot* a_slot,
            RE::PlayerCharacter*& a_player);
    };
}
