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

            //got more settings, variables, but for now we are ok
            top_type = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uType", 0));
            top_selected_item_form = mcm.GetValue("TopPage", "sSelectedItemForm", "");
            top_slot_action = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uSlotAction", 0));
            top_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uHandSelection", 1));
            top_type_left = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uTypeLeft", 0));
            top_selected_item_form_left = mcm.GetValue("TopPage", "sSelectedItemFormLeft", "");
            top_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            right_type = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uType", 0));
            right_selected_item_form = mcm.GetValue("RightPage", "sSelectedItemForm", "");
            right_slot_action = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uSlotAction", 0));
            right_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uHandSelection", 1));
            right_type_left = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uTypeLeft", 0));
            right_selected_item_form_left = mcm.GetValue("RightPage", "sSelectedItemFormLeft", "");
            right_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            bottom_type = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uType", 0));
            bottom_selected_item_form = mcm.GetValue("BottomPage", "sSelectedItemForm", "");
            bottom_slot_action = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uSlotAction", 0));
            bottom_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uHandSelection", 1));
            bottom_type_left = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uTypeLeft", 0));
            bottom_selected_item_form_left = mcm.GetValue("BottomPage", "sSelectedItemFormLeft", "");
            bottom_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            left_type = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uType", 0));
            left_selected_item_form = mcm.GetValue("LeftPage", "sSelectedItemForm", "");
            left_slot_action = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uSlotAction", 0));
            left_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uHandSelection", 1));
            left_type_left = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uTypeLeft", 0));
            left_selected_item_form_left = mcm.GetValue("LeftPage", "sSelectedItemFormLeft", "");
            left_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            top_type_page_one = static_cast<uint32_t>(mcm.GetLongValue("TopPageSetOne", "uType", 0));
            top_selected_item_form_page_one = mcm.GetValue("TopPageSetOne", "sSelectedItemForm", "");
            top_slot_action_page_one = static_cast<uint32_t>(mcm.GetLongValue("TopPageSetOne", "uSlotAction", 0));
            top_hand_selection_page_one = static_cast<uint32_t>(mcm.GetLongValue("TopPageSetOne", "uHandSelection", 1));
            top_type_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("TopPageSetOne", "uTypeLeft", 0));
            top_selected_item_form_left_page_one = mcm.GetValue("TopPageSetOne", "sSelectedItemFormLeft", "");
            top_slot_action_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("TopPageSetOne",
                "uSlotActionLeft",
                0));

            //got more settings, variables, but for now we are ok
            right_type_page_one = static_cast<uint32_t>(mcm.GetLongValue("RightPageSetOne", "uType", 0));
            right_selected_item_form_page_one = mcm.GetValue("RightPageSetOne", "sSelectedItemForm", "");
            right_slot_action_page_one = static_cast<uint32_t>(mcm.GetLongValue("RightPageSetOne", "uSlotAction", 0));
            right_hand_selection_page_one = static_cast<uint32_t>(mcm.GetLongValue("RightPageSetOne",
                "uHandSelection",
                1));
            right_type_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("RightPageSetOne", "uTypeLeft", 0));
            right_selected_item_form_left_page_one = mcm.GetValue("RightPageSetOne", "sSelectedItemFormLeft", "");
            right_slot_action_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("RightPageSetOne",
                "uSlotActionLeft",
                0));

            //got more settings, variables, but for now we are ok
            bottom_type_page_one = static_cast<uint32_t>(mcm.GetLongValue("BottomPageSetOne", "uType", 0));
            bottom_selected_item_form_page_one = mcm.GetValue("BottomPageSetOne", "sSelectedItemForm", "");
            bottom_slot_action_page_one = static_cast<uint32_t>(mcm.GetLongValue("BottomPageSetOne", "uSlotAction", 0));
            bottom_hand_selection_page_one = static_cast<uint32_t>(mcm.GetLongValue("BottomPageSetOne",
                "uHandSelection",
                1));
            bottom_type_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("BottomPageSetOne", "uTypeLeft", 0));
            bottom_selected_item_form_left_page_one = mcm.GetValue("BottomPageSetOne", "sSelectedItemFormLeft", "");
            bottom_slot_action_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("BottomPageSetOne",
                "uSlotActionLeft",
                0));

            //got more settings, variables, but for now we are ok
            left_type_page_one = static_cast<uint32_t>(mcm.GetLongValue("LeftPageSetOne", "uType", 0));
            left_selected_item_form_page_one = mcm.GetValue("LeftPageSetOne", "sSelectedItemForm", "");
            left_slot_action_page_one = static_cast<uint32_t>(mcm.GetLongValue("LeftPageSetOne", "uSlotAction", 0));
            left_hand_selection_page_one = static_cast<uint32_t>(mcm.
                GetLongValue("LeftPageSetOne", "uHandSelection", 1));
            left_type_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("LeftPageSetOne", "uTypeLeft", 0));
            left_selected_item_form_left_page_one = mcm.GetValue("LeftPageSetOne", "sSelectedItemFormLeft", "");
            left_slot_action_left_page_one = static_cast<uint32_t>(mcm.GetLongValue("LeftPageSetOne",
                "uSlotActionLeft",
                0));

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

    uint32_t mcm_setting::get_top_type() { return top_type; }
    std::string mcm_setting::get_top_selected_item_form() { return top_selected_item_form; }
    uint32_t mcm_setting::get_top_slot_action() { return top_slot_action; }
    uint32_t mcm_setting::get_top_hand_selection() { return top_hand_selection; }
    uint32_t mcm_setting::get_top_type_left() { return top_type_left; }
    std::string mcm_setting::get_top_selected_item_form_left() { return top_selected_item_form_left; }
    uint32_t mcm_setting::get_top_slot_action_left() { return top_slot_action_left; }

    uint32_t mcm_setting::get_right_type() { return right_type; }
    std::string mcm_setting::get_right_selected_item_form() { return right_selected_item_form; }
    uint32_t mcm_setting::get_right_slot_action() { return right_slot_action; }
    uint32_t mcm_setting::get_right_hand_selection() { return right_hand_selection; }
    uint32_t mcm_setting::get_right_type_left() { return right_type_left; }
    std::string mcm_setting::get_right_selected_item_form_left() { return right_selected_item_form_left; }
    uint32_t mcm_setting::get_right_slot_action_left() { return right_slot_action_left; }

    uint32_t mcm_setting::get_bottom_type() { return bottom_type; }
    std::string mcm_setting::get_bottom_selected_item_form() { return bottom_selected_item_form; }
    uint32_t mcm_setting::get_bottom_slot_action() { return bottom_slot_action; }
    uint32_t mcm_setting::get_bottom_hand_selection() { return bottom_hand_selection; }
    uint32_t mcm_setting::get_bottom_type_left() { return bottom_type_left; }
    std::string mcm_setting::get_bottom_selected_item_form_left() { return bottom_selected_item_form_left; }
    uint32_t mcm_setting::get_bottom_slot_action_left() { return bottom_slot_action_left; }

    uint32_t mcm_setting::get_left_type() { return left_type; }
    std::string mcm_setting::get_left_selected_item_form() { return left_selected_item_form; }
    uint32_t mcm_setting::get_left_slot_action() { return left_slot_action; }
    uint32_t mcm_setting::get_left_hand_selection() { return left_hand_selection; }
    uint32_t mcm_setting::get_left_type_left() { return left_type_left; }
    std::string mcm_setting::get_left_selected_item_form_left() { return left_selected_item_form_left; }
    uint32_t mcm_setting::get_left_slot_action_left() { return left_slot_action_left; }

    uint32_t mcm_setting::get_top_type_page_one() { return top_type_page_one; }
    std::string mcm_setting::get_top_selected_item_form_page_one() { return top_selected_item_form_page_one; }
    uint32_t mcm_setting::get_top_slot_action_page_one() { return top_slot_action_page_one; }
    uint32_t mcm_setting::get_top_hand_selection_page_one() { return top_hand_selection_page_one; }
    uint32_t mcm_setting::get_top_type_left_page_one() { return top_type_left_page_one; }
    std::string mcm_setting::get_top_selected_item_form_left_page_one() { return top_selected_item_form_left_page_one; }
    uint32_t mcm_setting::get_top_slot_action_left_page_one() { return top_slot_action_left_page_one; }

    uint32_t mcm_setting::get_right_type_page_one() { return right_type_page_one; }
    std::string mcm_setting::get_right_selected_item_form_page_one() { return right_selected_item_form_page_one; }
    uint32_t mcm_setting::get_right_slot_action_page_one() { return right_slot_action_page_one; }
    uint32_t mcm_setting::get_right_hand_selection_page_one() { return right_hand_selection_page_one; }
    uint32_t mcm_setting::get_right_type_left_page_one() { return right_type_left_page_one; }

    std::string mcm_setting::get_right_selected_item_form_left_page_one() {
        return right_selected_item_form_left_page_one;
    }

    uint32_t mcm_setting::get_right_slot_action_left_page_one() { return right_slot_action_left_page_one; }

    uint32_t mcm_setting::get_bottom_type_page_one() { return bottom_type_page_one; }
    std::string mcm_setting::get_bottom_selected_item_form_page_one() { return bottom_selected_item_form_page_one; }
    uint32_t mcm_setting::get_bottom_slot_action_page_one() { return bottom_slot_action_page_one; }
    uint32_t mcm_setting::get_bottom_hand_selection_page_one() { return bottom_hand_selection_page_one; }
    uint32_t mcm_setting::get_bottom_type_left_page_one() { return bottom_type_left_page_one; }

    std::string mcm_setting::get_bottom_selected_item_form_left_page_one() {
        return bottom_selected_item_form_left_page_one;
    }

    uint32_t mcm_setting::get_bottom_slot_action_left_page_one() { return bottom_slot_action_left_page_one; }

    uint32_t mcm_setting::get_left_type_page_one() { return left_type_page_one; }
    std::string mcm_setting::get_left_selected_item_form_page_one() { return left_selected_item_form_page_one; }
    uint32_t mcm_setting::get_left_slot_action_page_one() { return left_slot_action_page_one; }
    uint32_t mcm_setting::get_left_hand_selection_page_one() { return left_hand_selection_page_one; }
    uint32_t mcm_setting::get_left_type_left_page_one() { return left_type_left_page_one; }

    std::string mcm_setting::get_left_selected_item_form_left_page_one() {
        return left_selected_item_form_left_page_one;
    }

    uint32_t mcm_setting::get_left_slot_action_left_page_one() { return left_slot_action_left_page_one; }

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

    void mcm_setting::write_section_setting(const std::string& a_section,
        const uint32_t a_type,
        const std::string& a_form,
        const uint32_t a_action,
        const uint32_t a_hand,
        const uint32_t a_type_left,
        const std::string& a_form_left,
        const uint32_t a_action_left) {
        logger::trace(
            "writing section {}, type {}, form {}, action {}, hand {}, type_left {}, a_form_left {}, action_left {}"sv,
            a_section,
            a_type,
            a_form,
            a_action,
            a_hand,
            a_type_left,
            a_form_left,
            a_action_left);

        const auto section = a_section.c_str();

        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(mcm_config_setting);

        ini.Delete(section, nullptr);

        ini.SetLongValue(section, "uType", a_type);
        ini.SetValue(section, "sSelectedItemForm", a_form.c_str());
        ini.SetLongValue(section, "uSlotAction", a_action);
        ini.SetLongValue(section, "uHandSelection", a_hand);
        ini.SetLongValue(section, "uTypeLeft", a_type_left);
        ini.SetValue(section, "sSelectedItemFormLeft", a_form_left.c_str());
        ini.SetLongValue(section, "uSlotActionLeft", a_action_left);

        (void)ini.SaveFile(mcm_config_setting);
    }
}
