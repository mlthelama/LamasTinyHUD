#pragma once
#include "key_position.h"

namespace handle {
    class set_data {
    public:
        static void set_slot_data();

    private:
        static void set_empty_slot(int a_pos, key_position*& a_key_pos);
        static void set_slot(page_setting::position a_pos,
            uint32_t a_form,
            uint32_t a_type,
            uint32_t a_hand,
            uint32_t a_action,
            uint32_t a_form_left,
            uint32_t a_type_left,
            uint32_t a_action_left,
            key_position*& a_key_pos);
    };
}
