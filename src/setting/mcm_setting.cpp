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

    static float hud_image_scale_width;
    static float hud_image_scale_height;
    static float hud_image_position_width;
    static float hud_image_position_height;
    static float hud_slot_position_offset;
    static float hud_key_position_offset;
    static float icon_scale_width;
    static float icon_scale_height;
    static float key_icon_scale_width;
    static float key_icon_scale_height;
    static float slot_count_text_offset;
    static bool draw_toggle_button;
    static float toggle_key_offset_x;
    static float toggle_key_offset_y;
    static float current_items_offset_x;
    static float current_items_offset_y;
    static float slot_item_name_offset_x;
    static float slot_item_name_offset_y;

    static uint32_t background_transparency;
    static uint32_t background_icon_transparency;
    static uint32_t icon_transparency;
    static uint32_t key_transparency;
    static uint32_t text_transparency;
    static uint32_t icon_transparency_blocked;
    static float slot_count_text_font_size;
    static float current_items_font_size;
    static uint32_t current_items_red;
    static uint32_t current_items_green;
    static uint32_t current_items_blue;
    static uint32_t slot_count_red;
    static uint32_t slot_count_green;
    static uint32_t slot_count_blue;
    static uint32_t slot_button_feedback;
    static bool draw_current_items_text;

    static bool action_check;
    static bool empty_hand_setting;
    static bool hide_outside_combat;
    static float fade_timer_outside_combat;
    static bool disable_input_quick_loot;
    static bool elder_demon_souls;
    static uint32_t max_page_count;

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
            draw_toggle_button = mcm.GetBoolValue("Controls", "bDrawToggleButton", true);

            hud_image_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleWidth", 0.23));
            hud_image_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudImageScaleHeight", 0.23));
            hud_image_position_width = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fHudImagePositionWidth",
                140));
            hud_image_position_height = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fHudImagePositionHeight",
                140));
            hud_slot_position_offset = static_cast<float>(mcm.
                GetDoubleValue("HudSetting", "fHudSlotPositionOffset", 105));
            hud_key_position_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fHudKeyPositionOffset", 38));
            icon_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fIconScaleWidth", 0.10));
            icon_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fIconScaleHeight", 0.10));
            key_icon_scale_width = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fKeyIconScaleWidth", 0.38));
            key_icon_scale_height = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fKeyIconScaleHeight", 0.38));
            toggle_key_offset_x = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fToggleKeyOffsetX",
                115));
            toggle_key_offset_y = static_cast<float>(mcm.GetDoubleValue("HudSetting",
                "fToggleKeyOffsetY",
                115));
            current_items_offset_x = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fCurrentItemsOffsetX", -400));
            current_items_offset_y = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fCurrentItemsOffsetY", 130));
            slot_count_text_offset = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fSlotCountTextOffset", 20));
            slot_item_name_offset_x = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fSlotItemNameOffsetX", -65));
            slot_item_name_offset_y = static_cast<float>(mcm.GetDoubleValue("HudSetting", "fSlotItemNameOffsetY", 65));

            background_transparency = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting",
                "uBackgroundTransparency",
                255));
            background_icon_transparency = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting",
                "uBackgroundIconTransparency",
                255));
            icon_transparency = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uIconTransparency", 125));
            key_transparency = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uKeyTransparency", 255));
            text_transparency = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uTextTransparency", 255));
            icon_transparency_blocked = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting",
                "uIconTransparencyBlocked",
                50));
            slot_count_text_font_size = static_cast<float>(mcm.GetDoubleValue("GraphicSetting",
                "fSlotCountTextFontSize",
                20));
            current_items_font_size = static_cast<float>(mcm.GetDoubleValue("GraphicSetting",
                "fCurrentItemsFontSize",
                20));

            current_items_red = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uCurrentItemsRed", 255));
            current_items_green = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uCurrentItemsGreen", 255));
            current_items_blue = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uCurrentItemsBlue", 255));
            slot_count_red = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uSlotCountRed", 255));
            slot_count_green = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uSlotCountGreen", 255));
            slot_count_blue = static_cast<uint32_t>(mcm.GetLongValue("GraphicSetting", "uSlotCountBlue", 255));
            slot_button_feedback = static_cast<uint32_t>(mcm.
                GetLongValue("GraphicSetting", "uSlotButtonFeedback", 175));
            draw_current_items_text = mcm.GetBoolValue("GraphicSetting", "bDrawCurrentItemsText", false);

            action_check = mcm.GetBoolValue("MiscSetting", "bActionCheck", false);
            empty_hand_setting = mcm.GetBoolValue("MiscSetting", "bEmptyHandSetting", true);
            hide_outside_combat = mcm.GetBoolValue("MiscSetting", "bHideOutsideCombat", false);
            fade_timer_outside_combat = static_cast<float>(mcm.GetDoubleValue("MiscSetting",
                "fFadeTimerOutsideCombat",
                5));
            disable_input_quick_loot = mcm.GetBoolValue("MiscSetting", "bDisableInputQuickLoot", false);
            elder_demon_souls = mcm.GetBoolValue("MiscSetting", "bElderDemonSouls", false);
            max_page_count = static_cast<uint32_t>(mcm.GetLongValue("MiscSetting", "uMaxPageCount", 2));
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
    float mcm_setting::get_key_icon_scale_width() { return key_icon_scale_width; }
    float mcm_setting::get_key_icon_scale_height() { return key_icon_scale_height; }
    float mcm_setting::get_slot_count_text_offset() { return slot_count_text_offset; }
    bool mcm_setting::get_draw_toggle_button() { return draw_toggle_button; }
    float mcm_setting::get_toggle_key_offset_x() { return toggle_key_offset_x; }
    float mcm_setting::get_toggle_key_offset_y() { return toggle_key_offset_y; }
    float mcm_setting::get_current_items_offset_x() { return current_items_offset_x; }
    float mcm_setting::get_current_items_offset_y() { return current_items_offset_y; }
    float mcm_setting::get_slot_item_name_offset_x() { return slot_item_name_offset_x; }
    float mcm_setting::get_slot_item_name_offset_y() { return slot_item_name_offset_y; }

    uint32_t mcm_setting::get_background_transparency() { return background_transparency; }
    uint32_t mcm_setting::get_background_icon_transparency() { return background_icon_transparency; }
    uint32_t mcm_setting::get_icon_transparency() { return icon_transparency; }
    uint32_t mcm_setting::get_key_transparency() { return key_transparency; }
    uint32_t mcm_setting::get_text_transparency() { return text_transparency; }
    uint32_t mcm_setting::get_icon_transparency_blocked() { return icon_transparency_blocked; }
    float mcm_setting::get_slot_count_text_font_size() { return slot_count_text_font_size; }
    float mcm_setting::get_current_items_font_size() { return current_items_font_size; }
    uint32_t mcm_setting::get_current_items_red() { return current_items_red; }
    uint32_t mcm_setting::get_current_items_green() { return current_items_green; }
    uint32_t mcm_setting::get_current_items_blue() { return current_items_blue; }
    uint32_t mcm_setting::get_slot_count_red() { return slot_count_red; }
    uint32_t mcm_setting::get_slot_count_green() { return slot_count_green; }
    uint32_t mcm_setting::get_slot_count_blue() { return slot_count_blue; }
    bool mcm_setting::get_draw_current_items_text() { return draw_current_items_text; }
    uint32_t mcm_setting::get_slot_button_feedback() { return slot_button_feedback; }

    bool mcm_setting::get_action_check() { return action_check; }
    bool mcm_setting::get_empty_hand_setting() { return empty_hand_setting; }
    bool mcm_setting::get_hide_outside_combat() { return hide_outside_combat; }
    float mcm_setting::get_fade_timer_outside_combat() { return fade_timer_outside_combat; }
    bool mcm_setting::get_disable_input_quick_loot() { return disable_input_quick_loot; }
    bool mcm_setting::get_elden_demon_souls() { return elder_demon_souls; }
    uint32_t mcm_setting::get_max_page_count() { return max_page_count; }
}
