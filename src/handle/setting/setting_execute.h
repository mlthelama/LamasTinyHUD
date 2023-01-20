#pragma once
#include "handle/page/position_setting.h"
#include "handle/page/slot_setting.h"

namespace handle {
    class setting_execute {
    public:
        static bool execute_settings(const std::vector<slot_setting*>& a_slots, position_setting::position a_position);
        static position_setting* get_position_setting_for_key(uint32_t a_key);

    private:
        static void execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player);
    };
}
