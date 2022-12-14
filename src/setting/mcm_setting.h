#pragma once

namespace config {
    class mcm_setting {
    public:
        static void read_setting();

        static uint32_t get_top_action_key();
        static uint32_t get_right_action_key();
        static uint32_t get_bottom_action_key();
        static uint32_t get_left_action_key();
        static uint32_t get_toggle_key();
        static uint32_t get_controller_set();
        static float get_config_button_hold_time();

        static uint32_t get_top_type();
        static std::string get_top_selected_item_form();
        static uint32_t get_top_slot_action();
        static uint32_t get_top_hand_selection();
        static uint32_t get_top_type_left();
        static std::string get_top_selected_item_form_left();
        static uint32_t get_top_slot_action_left();

        static uint32_t get_right_type();
        static std::string get_right_selected_item_form();
        static uint32_t get_right_slot_action();
        static uint32_t get_right_hand_selection();
        static uint32_t get_right_type_left();
        static std::string get_right_selected_item_form_left();
        static uint32_t get_right_slot_action_left();

        static uint32_t get_bottom_type();
        static std::string get_bottom_selected_item_form();
        static uint32_t get_bottom_slot_action();
        static uint32_t get_bottom_hand_selection();
        static uint32_t get_bottom_type_left();
        static std::string get_bottom_selected_item_form_left();
        static uint32_t get_bottom_slot_action_left();

        static uint32_t get_left_type();
        static std::string get_left_selected_item_form();
        static uint32_t get_left_slot_action();
        static uint32_t get_left_hand_selection();
        static uint32_t get_left_type_left();
        static std::string get_left_selected_item_form_left();
        static uint32_t get_left_slot_action_left();

        static uint32_t get_top_type_page_one();
        static std::string get_top_selected_item_form_page_one();
        static uint32_t get_top_slot_action_page_one();
        static uint32_t get_top_hand_selection_page_one();
        static uint32_t get_top_type_left_page_one();
        static std::string get_top_selected_item_form_left_page_one();
        static uint32_t get_top_slot_action_left_page_one();

        static uint32_t get_right_type_page_one();
        static std::string get_right_selected_item_form_page_one();
        static uint32_t get_right_slot_action_page_one();
        static uint32_t get_right_hand_selection_page_one();
        static uint32_t get_right_type_left_page_one();
        static std::string get_right_selected_item_form_left_page_one();
        static uint32_t get_right_slot_action_left_page_one();

        static uint32_t get_bottom_type_page_one();
        static std::string get_bottom_selected_item_form_page_one();
        static uint32_t get_bottom_slot_action_page_one();
        static uint32_t get_bottom_hand_selection_page_one();
        static uint32_t get_bottom_type_left_page_one();
        static std::string get_bottom_selected_item_form_left_page_one();
        static uint32_t get_bottom_slot_action_left_page_one();

        static uint32_t get_left_type_page_one();
        static std::string get_left_selected_item_form_page_one();
        static uint32_t get_left_slot_action_page_one();
        static uint32_t get_left_hand_selection_page_one();
        static uint32_t get_left_type_left_page_one();
        static std::string get_left_selected_item_form_left_page_one();
        static uint32_t get_left_slot_action_left_page_one();

        static float get_hud_image_scale_width();
        static float get_hud_image_scale_height();
        static float get_hud_image_position_width();
        static float get_hud_image_position_height();
        static float get_hud_slot_position_offset();
        static float get_hud_key_position_offset();
        static float get_icon_scale_width();
        static float get_icon_scale_height();
        static uint32_t get_icon_opacity();
        static uint32_t get_slot_button_feedback();
        static float get_key_icon_scale_width();
        static float get_key_icon_scale_height();
        static float get_slot_count_text_offset();
        static float get_slot_count_text_font_size();
        static bool get_draw_toggle_button();
        static float get_toggle_key_offset_x();
        static float get_toggle_key_offset_y();

        static bool get_action_check();
        static bool get_empty_hand_setting();

        static void write_section_setting(const std::string& a_section,
            uint32_t a_type,
            const std::string& a_form,
            uint32_t a_action,
            uint32_t a_hand,
            uint32_t a_type_left,
            const std::string& a_form_left,
            uint32_t a_action_left);
    };
}
