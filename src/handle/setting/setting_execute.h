#pragma once
#include "handle/data/page/position_setting.h"
#include "handle/data/page/slot_setting.h"

namespace handle {
    class setting_execute {
    public:
        static void execute_settings(const std::vector<slot_setting*>& a_slots, bool a_only_equip = false, bool a_only_instant = false);
        static position_setting* get_position_setting_for_key(uint32_t a_key);
        static void execute_ammo(const RE::TESForm* a_form);

    private:
        static void execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player);
    };
}
