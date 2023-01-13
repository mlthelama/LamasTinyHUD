#include "mcm_setting.h"
#include <SimpleIni.h>

namespace config {
    static const char* mcm_default_setting = R"(.\Data\MCM\Config\LamasTinyHUD\settings.ini)";
    static const char* mcm_config_setting = R"(.\Data\MCM\Settings\LamasTinyHUD.ini)";

    static uint32_t top_action_key;
    static uint32_t right_action_key;
    static uint32_t bottom_action_key;
    static uint32_t left_action_key;
    static uint32_t toggle_key;
    static uint32_t controller_set;
    static float config_button_hold_time;

    static uint32_t top_type;
    static std::string top_selected_item_form;
    static uint32_t top_slot_action;
    static uint32_t top_hand_selection;
    static uint32_t top_type_left;
    static std::string top_selected_item_form_left;
    static uint32_t top_slot_action_left;

    static uint32_t right_type;
    static std::string right_selected_item_form;
    static uint32_t right_slot_action;
    static uint32_t right_hand_selection;
    static uint32_t right_type_left;
    static std::string right_selected_item_form_left;
    static uint32_t right_slot_action_left;

    static uint32_t bottom_type;
    static std::string bottom_selected_item_form;
    static uint32_t bottom_slot_action;
    static uint32_t bottom_hand_selection;
    static uint32_t bottom_type_left;
    static std::string bottom_selected_item_form_left;
    static uint32_t bottom_slot_action_left;

    static uint32_t left_type;
    static std::string left_selected_item_form;
    static uint32_t left_slot_action;
    static uint32_t left_hand_selection;
    static uint32_t left_type_left;
    static std::string left_selected_item_form_left;
    static uint32_t left_slot_action_left;

    static uint32_t top_type_page_one;
    static std::string top_selected_item_form_page_one;
    static uint32_t top_slot_action_page_one;
    static uint32_t top_hand_selection_page_one;
    static uint32_t top_type_left_page_one;
    static std::string top_selected_item_form_left_page_one;
    static uint32_t top_slot_action_left_page_one;

    static uint32_t right_type_page_one;
    static std::string right_selected_item_form_page_one;
    static uint32_t right_slot_action_page_one;
    static uint32_t right_hand_selection_page_one;
    static uint32_t right_type_left_page_one;
    static std::string right_selected_item_form_left_page_one;
    static uint32_t right_slot_action_left_page_one;

    static uint32_t bottom_type_page_one;
    static std::string bottom_selected_item_form_page_one;
    static uint32_t bottom_slot_action_page_one;
    static uint32_t bottom_hand_selection_page_one;
    static uint32_t bottom_type_left_page_one;
    static std::string bottom_selected_item_form_left_page_one;
    static uint32_t bottom_slot_action_left_page_one;

    static uint32_t left_type_page_one;
    static std::string left_selected_item_form_page_one;
    static uint32_t left_slot_action_page_one;
    static uint32_t left_hand_selection_page_one;
    static uint32_t left_type_left_page_one;
    static std::string left_selected_item_form_left_page_one;
    static uint32_t left_slot_action_left_page_one;

    static float hud_image_scale_width;
    static float hud_image_scale_height;
    static float hud_image_position_width;
    static float hud_image_position_height;
    static float hud_slot_position_offset;
    static float hud_key_position_offset;
    static float icon_scale_width;
    static float icon_scale_height;
    static uint32_t icon_opacity;
    static uint32_t slot_button_feedback;
    static float key_icon_scale_width;
    static float key_icon_scale_height;
    static float slot_count_text_offset;
    static float slot_count_text_font_size;
    static bool draw_toggle_button;
    static float toggle_key_offset_x;
    static float toggle_key_offset_y;

    static bool action_check;
    static bool empty_hand_setting;

    void mcm_setting::read_setting() {
        logger::info("reading mcm ini files");

        const auto read_mcm = [&](const std::filesystem::path& path) {
            CSimpleIniA mcm;
            mcm.SetUnicode();
            mcm.LoadFile(path.string().c_str());

            top_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uTopActionKey", 10));
            right_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uRightActionKey", 11));
            bottom_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uBottomActionKey", 12));
            left_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uLeftActionKey", 13));
            toggle_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uToggleKey", 27));
            controller_set = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uControllerSet", 0));
            config_button_hold_time = static_cast<float>(mcm.GetDoubleValue("Controls", "fConfigButtonHoldTime", 5));

            hud_image_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleWidth", 0.25));
            hud_image_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleHeight", 0.25));
            hud_image_position_width = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fHudImagePositionWidth",
                140));
            hud_image_position_height = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fHudImagePositionHeight",
                140));
            hud_slot_position_offset = static_cast<float>(mcm.
                GetDoubleValue("HudSetting", "fHudSlotPositionOffset", 85));
            hud_key_position_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudKeyPositionOffset", 30));
            icon_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fIconScaleWidth", 0.11));
            icon_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fIconScaleHeight", 0.11));
            icon_opacity = static_cast<uint32_t>(mcm.GetLongValue("HudSetting", "uIconOpacity", 125));
            slot_button_feedback = static_cast<uint32_t>(mcm.GetLongValue("HudSetting", "uSlotButtonFeedback", 200));
            key_icon_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fKeyIconScaleWidth", 0.4));
            key_icon_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fKeyIconScaleHeight", 0.4));
            slot_count_text_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fSlotCountTextOffset", 10));
            slot_count_text_font_size = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fSlotCountTextFontSize",
                20));
            draw_toggle_button = mcm.GetBoolValue("HudSetting", "bDrawToggleButton", true);
            toggle_key_offset_x = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fToggleKeyOffsetX",
                90));
            toggle_key_offset_y = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fToggleKeyOffsetY",
                90));

            action_check = mcm.GetBoolValue("MiscSetting", "bActionCheck", true);
            empty_hand_setting = mcm.GetBoolValue("MiscSetting", "bEmptyHandSetting", true);
        };

        read_mcm(mcm_default_setting);
        read_mcm(mcm_config_setting);


        logger::info("finished reading mcm ini files. return.");
    }

    uint32_t mcm_setting::get_top_action_key() { return top_action_key; }
    uint32_t mcm_setting::get_right_action_key() { return right_action_key; }
    uint32_t mcm_setting::get_bottom_action_key() { return bottom_action_key; }
    uint32_t mcm_setting::get_left_action_key() { return left_action_key; }
    uint32_t mcm_setting::get_toggle_key() { return toggle_key; }
    uint32_t mcm_setting::get_controller_set() { return controller_set; }
    float mcm_setting::get_config_button_hold_time() { return config_button_hold_time; }

    float mcm_setting::get_hud_image_scale_width() { return hud_image_scale_width; }
    float mcm_setting::get_hud_image_scale_height() { return hud_image_scale_height; }
    float mcm_setting::get_hud_image_position_width() { return hud_image_position_width; }
    float mcm_setting::get_hud_image_position_height() { return hud_image_position_height; }
    float mcm_setting::get_hud_slot_position_offset() { return hud_slot_position_offset; }
    float mcm_setting::get_hud_key_position_offset() { return hud_key_position_offset; }
    float mcm_setting::get_icon_scale_width() { return icon_scale_width; }
    float mcm_setting::get_icon_scale_height() { return icon_scale_height; }
    uint32_t mcm_setting::get_icon_opacity() { return icon_opacity; }
    uint32_t mcm_setting::get_slot_button_feedback() { return slot_button_feedback; }
    float mcm_setting::get_key_icon_scale_width() { return key_icon_scale_width; }
    float mcm_setting::get_key_icon_scale_height() { return key_icon_scale_height; }
    float mcm_setting::get_slot_count_text_offset() { return slot_count_text_offset; }
    float mcm_setting::get_slot_count_text_font_size() { return slot_count_text_font_size; }
    bool mcm_setting::get_draw_toggle_button() { return draw_toggle_button; }
    float mcm_setting::get_toggle_key_offset_x() { return toggle_key_offset_x; }
    float mcm_setting::get_toggle_key_offset_y() { return toggle_key_offset_y; }

    bool mcm_setting::get_action_check() { return action_check; }
    bool mcm_setting::get_empty_hand_setting() { return empty_hand_setting; }
}
