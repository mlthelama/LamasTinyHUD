#pragma once
#include "handle/page/slot_setting.h"

namespace magic {
    class spell {
    public:
        using action_type = handle::slot_setting::acton_type;

        static std::vector<RE::TESForm*> get_spells();
        static void cast_magic(RE::TESForm* a_form, action_type a_action, const RE::BGSEquipSlot* a_slot);
    private:
        static RE::MagicSystem::CastingSource get_casting_source(const RE::BGSEquipSlot* a_slot);
    };
}
