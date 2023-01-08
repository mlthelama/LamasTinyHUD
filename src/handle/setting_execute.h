#pragma once
#include "page/page_setting.h"
#include "page/slot_setting.h"

namespace handle {
    class setting_execute {
    public:
        static void execute_settings(const std::vector<slot_setting*>& a_slots);
        static page_setting* get_page_setting_for_key(uint32_t a_key);
        static void unequip_if_equipped(bool a_left,
            RE::PlayerCharacter*& a_player,
            RE::ActorEquipManager*& a_actor_equip_manager);

    private:
        static void execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player);
    };
}
