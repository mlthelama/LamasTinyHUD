#pragma once
#include "handle/data/page/slot_setting.h"

namespace equip {
    class magic {
    public:
        using action_type = handle::slot_setting::action_type;

        static void cast_magic(RE::TESForm* a_form,
            action_type a_action,
            const RE::BGSEquipSlot* a_slot,
            RE::PlayerCharacter*& a_player);
        static void cast_scroll(const RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player);
        static void equip_or_cast_power(RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player);
        static void equip_shout(RE::TESForm* a_form, RE::PlayerCharacter*& a_player);

    private:
        static RE::MagicSystem::CastingSource get_casting_source(const RE::BGSEquipSlot* a_slot);
    };
}
