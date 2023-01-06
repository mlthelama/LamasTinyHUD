#include "mcm_setting.h"
#include <SimpleIni.h>

namespace config {
    static const char* mcm_default_setting = R"(.\Data\MCM\Config\LamasTinyHUD\settings.ini)";
    static const char* mcm_config_setting = R"(.\Data\MCM\Settings\LamasTinyHUD.ini)";

    static uint32_t top_action_key;
    static uint32_t right_action_key;
    static uint32_t bottom_action_key;
    static uint32_t left_action_key;

    static uint32_t top_type;
    static uint32_t top_selected_item_form;
    static uint32_t top_slot_action;
    static uint32_t top_hand_selection;
    static uint32_t top_type_left;
    static uint32_t top_selected_item_form_left;
    static uint32_t top_slot_action_left;

    static uint32_t right_type;
    static uint32_t right_selected_item_form;
    static uint32_t right_slot_action;
    static uint32_t right_hand_selection;
    static uint32_t right_type_left;
    static uint32_t right_selected_item_form_left;
    static uint32_t right_slot_action_left;

    static uint32_t bottom_type;
    static uint32_t bottom_selected_item_form;
    static uint32_t bottom_slot_action;
    static uint32_t bottom_hand_selection;
    static uint32_t bottom_type_left;
    static uint32_t bottom_selected_item_form_left;
    static uint32_t bottom_slot_action_left;

    static uint32_t left_type;
    static uint32_t left_selected_item_form;
    static uint32_t left_slot_action;
    static uint32_t left_hand_selection;
    static uint32_t left_type_left;
    static uint32_t left_selected_item_form_left;
    static uint32_t left_slot_action_left;

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

    void mcm_setting::read_setting() {
        logger::info("reading mcm ini files");

        const auto read_mcm = [&](const std::filesystem::path& path) {
            CSimpleIniA mcm;
            mcm.SetUnicode();
            mcm.LoadFile(path.string().c_str());

            top_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uTopActionKey", 45));
            right_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uRightActionKey", 46));
            bottom_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uBottomActionKey", 46));
            left_action_key = static_cast<uint32_t>(mcm.GetLongValue("Controls", "uLeftActionKey", 48));

            //got more settings, variables, but for now we are ok
            top_type = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uType", 0));
            std::stringstream selected_top_item_form_stream(mcm.GetValue("TopPage", "sSelectedItemForm", ""));
            selected_top_item_form_stream >> top_selected_item_form;
            top_slot_action = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uSlotAction", 0));
            top_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uHandSelection", 1));
            top_type_left = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uTypeLeft", 0));
            std::stringstream selected_top_item_form_left_stream(
                mcm.GetValue("TopPage", "sSelectedItemFormLeft", ""));
            selected_top_item_form_left_stream >> top_selected_item_form_left;
            top_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            right_type = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uType", 0));
            std::stringstream selected_right_item_form_stream(mcm.GetValue("RightPage", "sSelectedItemForm", ""));
            selected_right_item_form_stream >> right_selected_item_form;
            right_slot_action = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uSlotAction", 0));
            right_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uHandSelection", 1));
            right_type_left = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uTypeLeft", 0));
            std::stringstream selected_right_item_form_left_stream(mcm.GetValue("RightPage",
                "sSelectedItemFormLeft",
                ""));
            selected_right_item_form_left_stream >> right_selected_item_form_left;
            right_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("RightPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            bottom_type = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uType", 0));
            std::stringstream selected_bottom_item_form_stream(mcm.GetValue("BottomPage", "sSelectedItemForm", ""));
            selected_bottom_item_form_stream >> bottom_selected_item_form;
            bottom_slot_action = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uSlotAction", 0));
            bottom_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uHandSelection", 1));
            bottom_type_left = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uTypeLeft", 0));
            std::stringstream selected_bottom_item_form_left_stream(mcm.GetValue("BottomPage",
                "sSelectedItemFormLeft",
                ""));
            selected_bottom_item_form_left_stream >> bottom_selected_item_form_left;
            bottom_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("BottomPage", "uSlotActionLeft", 0));

            //got more settings, variables, but for now we are ok
            left_type = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uType", 0));
            std::stringstream selected_left_item_form_stream(mcm.GetValue("LeftPage", "sSelectedItemForm", ""));
            selected_left_item_form_stream >> left_selected_item_form;
            left_slot_action = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uSlotAction", 0));
            left_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uHandSelection", 1));
            left_type_left = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uTypeLeft", 0));
            std::stringstream selected_left_item_form_left_stream(mcm.GetValue("LeftPage",
                "sSelectedItemFormLeft",
                ""));
            selected_left_item_form_left_stream >> left_selected_item_form_left;
            left_slot_action_left = static_cast<uint32_t>(mcm.GetLongValue("LeftPage", "uSlotActionLeft", 0));

            hud_image_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleWidth"));
            hud_image_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleHeight"));
            hud_image_position_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImagePositionWidth"));
            hud_image_position_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImagePositionHeight"));
            hud_slot_position_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudSlotPositionOffset"));
            hud_key_position_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudKeyPositionOffset"));
            icon_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fIconScaleWidth"));
            icon_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fIconScaleHeight"));
            icon_opacity = static_cast<uint32_t>(mcm.GetLongValue("HudSetting", "uIconOpacity", 255));
            slot_button_feedback = static_cast<uint32_t>(mcm.GetLongValue("HudSetting", "uSlotButtonFeedback"));
            key_icon_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fKeyIconScaleWidth"));
            key_icon_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fKeyIconScaleHeight"));
        };

        read_mcm(mcm_default_setting);
        read_mcm(mcm_config_setting);


        logger::info("finished reading mcm ini files. return.");
    }

    uint32_t mcm_setting::get_top_action_key() { return top_action_key; }
    uint32_t mcm_setting::get_right_action_key() { return right_action_key; }
    uint32_t mcm_setting::get_bottom_action_key() { return bottom_action_key; }
    uint32_t mcm_setting::get_left_action_key() { return left_action_key; }

    uint32_t mcm_setting::get_top_type() { return top_type; }
    uint32_t mcm_setting::get_top_selected_item_form() { return top_selected_item_form; }
    uint32_t mcm_setting::get_top_slot_action() { return top_slot_action; }
    uint32_t mcm_setting::get_top_hand_selection() { return top_hand_selection; }
    uint32_t mcm_setting::get_top_type_left() { return top_type_left; }
    uint32_t mcm_setting::get_top_selected_item_form_left() { return top_selected_item_form_left; }
    uint32_t mcm_setting::get_top_slot_action_left() { return top_slot_action_left; }

    uint32_t mcm_setting::get_right_type() { return right_type; }
    uint32_t mcm_setting::get_right_selected_item_form() { return right_selected_item_form; }
    uint32_t mcm_setting::get_right_slot_action() { return right_slot_action; }
    uint32_t mcm_setting::get_right_hand_selection() { return right_hand_selection; }
    uint32_t mcm_setting::get_right_type_left() { return right_type_left; }
    uint32_t mcm_setting::get_right_selected_item_form_left() { return right_selected_item_form_left; }
    uint32_t mcm_setting::get_right_slot_action_left() { return right_slot_action_left; }

    uint32_t mcm_setting::get_bottom_type() { return bottom_type; }
    uint32_t mcm_setting::get_bottom_selected_item_form() { return bottom_selected_item_form; }
    uint32_t mcm_setting::get_bottom_slot_action() { return bottom_slot_action; }
    uint32_t mcm_setting::get_bottom_hand_selection() { return bottom_hand_selection; }
    uint32_t mcm_setting::get_bottom_type_left() { return bottom_type_left; }
    uint32_t mcm_setting::get_bottom_selected_item_form_left() { return bottom_selected_item_form_left; }
    uint32_t mcm_setting::get_bottom_slot_action_left() { return bottom_slot_action_left; }

    uint32_t mcm_setting::get_left_type() { return left_type; }
    uint32_t mcm_setting::get_left_selected_item_form() { return left_selected_item_form; }
    uint32_t mcm_setting::get_left_slot_action() { return left_slot_action; }
    uint32_t mcm_setting::get_left_hand_selection() { return left_hand_selection; }
    uint32_t mcm_setting::get_left_type_left() { return left_type_left; }
    uint32_t mcm_setting::get_left_selected_item_form_left() { return left_selected_item_form_left; }
    uint32_t mcm_setting::get_left_slot_action_left() { return left_slot_action_left; }

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
}
