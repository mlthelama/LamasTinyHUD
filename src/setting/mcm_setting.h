#pragma once

namespace config {
    class mcm_setting {
    public:
        static void read_setting();

        static uint32_t get_top_action_key();
        static uint32_t get_right_action_key();
        static uint32_t get_bottom_action_key();
        static uint32_t get_left_action_key();

        static uint32_t get_top_type();
        static uint32_t get_selected_top_item_form();
    };
}
