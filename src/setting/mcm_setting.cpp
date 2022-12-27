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
            std::stringstream s(mcm.GetValue("TopPage", "sSelectedTopItemForm", ""));
            s >> selected_top_item_form;
            
        };

        read_mcm(mcm_default_setting);
        read_mcm(mcm_config_setting);

        logger::info("finished reading mcm ini files");
    }

    uint32_t mcm_setting::get_top_action_key() { return top_action_key; }
    uint32_t mcm_setting::get_right_action_key() { return right_action_key; }
    uint32_t mcm_setting::get_bottom_action_key() { return bottom_action_key; }
    uint32_t mcm_setting::get_left_action_key() { return left_action_key; }

    uint32_t mcm_setting::get_top_type() { return top_type; }
    uint32_t mcm_setting::get_selected_top_item_form() { return selected_top_item_form; }
}
