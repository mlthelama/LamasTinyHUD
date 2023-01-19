#pragma once
#include "handle/data/data_helper.h"
#include "handle/handle/key_position_handle.h"
#include "handle/page/position_setting.h"

namespace handle {
    class set_setting_data {
    public:
        static void read_and_set_data();

        static void set_new_item_count_if_needed(const RE::TESBoundObject* a_object, int32_t a_count);

        static void set_single_slot(uint32_t a_page,
            position_setting::position a_position,
            const std::vector<data_helper*>& a_data);

        static void set_queue_slot(position_setting::position a_pos, const std::vector<data_helper*>& a_data);

    private:
        static void set_empty_slot(int a_page, int a_pos, key_position_handle*& a_key_pos);
        static void set_slot(uint32_t a_page,
            position_setting::position a_position,
            const std::string& a_form,
            uint32_t a_type,
            uint32_t a_hand,
            uint32_t a_action,
            const std::string& a_form_left,
            uint32_t a_type_left,
            uint32_t a_action_left,
            key_position_handle*& a_key_pos);

        static void set_new_item_count(RE::FormID a_form_id, int32_t a_count);
    };
}
