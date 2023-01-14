#pragma once
#include "handle/page/slot_setting.h"

namespace magic {
    class spell {
    public:
        using action_type = handle::slot_setting::acton_type;

        //a_single sends is if the user selected both (= configure both hands)
        static std::vector<RE::TESForm*> get_spells(bool a_instant, bool a_single);
        static void cast_magic(RE::TESForm* a_form,
            action_type a_action,
            const RE::BGSEquipSlot* a_slot,
            RE::PlayerCharacter*& a_player);

        static void cast_scroll(const RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player);

    private:
        static RE::MagicSystem::CastingSource get_casting_source(const RE::BGSEquipSlot* a_slot);
    };
}
