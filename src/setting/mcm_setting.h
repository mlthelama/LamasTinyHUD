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

        static float get_hud_image_scale_width();
        static float get_hud_image_scale_height();

        static float get_hud_image_position_width();
        static float get_hud_image_position_height();
    };
}
