#pragma once
#include "handle/data/page/position_setting.h"
#include "handle/data/page/slot_setting.h"

namespace handle {
    class setting_execute {
    public:
        static void execute_settings(const std::vector<slot_setting*>& a_slots);
        static position_setting* get_position_setting_for_key(uint32_t a_key);

    private:
        static void execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player);
    };
}
