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
    static uint32_t selected_top_item_form;
    static uint32_t top_slot_action;
    static uint32_t top_hand_selection;

    static float hud_image_scale_width;
    static float hud_image_scale_height;
    static float hud_image_position_width;
    static float hud_image_position_height;
    static float hud_slot_position_offset;
    static float hud_key_position_offset;

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

            //got more settings, variables, but for now we are ok with these two
            top_type = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uTopType", -1));
            std::stringstream selected_top_item_form_stream(mcm.GetValue("TopPage", "sSelectedTopItemForm", ""));
            selected_top_item_form_stream >> selected_top_item_form;
            top_slot_action = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uTopSlotAction", 0));
            top_hand_selection = static_cast<uint32_t>(mcm.GetLongValue("TopPage", "uTopHandSelection", 0));

            hud_image_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleWidth"));
            hud_image_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleHeight"));
            hud_image_position_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImagePositionWidth"));
            hud_image_position_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImagePositionHeight"));
            hud_slot_position_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudSlotPositionOffset"));
            hud_key_position_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudKeyPositionOffset"));
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
    uint32_t mcm_setting::get_selected_top_item_form() { return selected_top_item_form; }
    uint32_t mcm_setting::get_top_slot_action() { return top_slot_action; }
    uint32_t mcm_setting::get_top_hand_selection() { return top_hand_selection; }

    float mcm_setting::get_hud_image_scale_width() { return hud_image_scale_width; }
    float mcm_setting::get_hud_image_scale_height() { return hud_image_scale_height; }
    float mcm_setting::get_hud_image_position_width() { return hud_image_position_width; }
    float mcm_setting::get_hud_image_position_height() { return hud_image_position_height; }
    float mcm_setting::get_hud_slot_position_offset() { return hud_slot_position_offset; }
    float mcm_setting::get_hud_key_position_offset() { return hud_key_position_offset; }
}
