#include "mcm_setting.h"

#include <SimpleIni.h>

namespace config {
    static const char* mcm_default_setting = R"(.\Data\MCM\Config\LamasTinyHUD\settings.ini)";
    static const char* mcm_config_setting = R"(.\Data\MCM\Settings\LamasTinyHUD.ini)";

    static uint32_t left_action_key;

    void mcm_setting::read_setting() {
        logger::info("reading mcm ini files");

        const auto read_mcm = [&](const std::filesystem::path path) {
            CSimpleIniA mcm;
            mcm.SetUnicode();
            mcm.LoadFile(path.string().c_str());

            left_action_key = static_cast<uint32_t>(mcm.GetLongValue("General", "uLeftActionKey", 48));
        };

        read_mcm(mcm_default_setting);
        read_mcm(mcm_config_setting);

        logger::info("finished reading mcm ini files");
    }

    uint32_t mcm_setting::get_left_action_key() { return left_action_key; }
}
