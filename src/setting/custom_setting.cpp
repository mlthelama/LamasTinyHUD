#include "custom_setting.h"
#include <SimpleIni.h>

namespace config {
    static const char* ini_path = R"(.\Data\SKSE\Plugins\LamasTinyHUD_Custom.ini)";

    static CSimpleIniA custom_ini;

    void custom_setting::read_setting() {
        //logger::info("reading custom ini file");

        /*CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(ini_path);*/

        custom_ini.SetUnicode();
        custom_ini.LoadFile(ini_path);


        //logger::info("finished reading dll ini files. return.");
    }

    CSimpleIniA::TNamesDepend custom_setting::get_sections() {
        CSimpleIniA::TNamesDepend sections;
        custom_ini.GetAllSections(sections);

        return sections;
    }

    uint32_t custom_setting::get_page_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uPage", 0));
    }

    uint32_t custom_setting::get_position_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uPosition", 0));
    }

    uint32_t custom_setting::get_type_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uType", 0));
    }

    std::string custom_setting::get_item_form_by_section(const std::string& a_section) {
        return custom_ini.GetValue(a_section.c_str(), "sSelectedItemForm", "");
    }

    uint32_t custom_setting::get_slot_action_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uSlotAction", 0));
    }

    uint32_t custom_setting::get_hand_selection_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uHandSelection", 1));
    }

    uint32_t custom_setting::get_type_left_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uTypeLeft", 0));
    }

    std::string custom_setting::get_item_form_left_by_section(const std::string& a_section) {
        return custom_ini.GetValue(a_section.c_str(), "sSelectedItemFormLeft", "");
    }

    uint32_t custom_setting::get_slot_action_left_by_section(const std::string& a_section) {
        return static_cast<uint32_t>(custom_ini.GetLongValue(a_section.c_str(), "uSlotActionLeft", 0));
    }

    void custom_setting::reset_section(const std::string& a_section) {
        read_setting();
        logger::trace("resetting section {}"sv, a_section);
        custom_ini.Delete(a_section.c_str(), nullptr);

        (void)custom_ini.SaveFile(ini_path);
        read_setting();
    }

    void custom_setting::write_slot_action_by_section(const std::string& a_section, const uint32_t a_action) {
        read_setting();
        custom_ini.SetLongValue(a_section.c_str(), "uSlotAction", a_action);

        (void)custom_ini.SaveFile(ini_path);
        read_setting();
    }

    void custom_setting::write_slot_action_left_by_section(const std::string& a_section, const uint32_t a_action) {
        read_setting();
        custom_ini.SetLongValue(a_section.c_str(), "uSlotActionLeft", a_action);

        (void)custom_ini.SaveFile(ini_path);
        read_setting();
    }

    void custom_setting::write_section_setting(const std::string& a_section,
        uint32_t a_page,
        uint32_t a_position,
        uint32_t a_type,
        const std::string& a_form,
        uint32_t a_action,
        uint32_t a_hand,
        uint32_t a_type_left,
        const std::string& a_form_left,
        uint32_t a_action_left) {
        logger::trace(
            "writing section {}, page {}, position {}, type {}, form {}, action {}, hand {}, type_left {}, a_form_left {}, action_left {}"sv,
            a_section,
            a_page,
            a_position,
            a_type,
            a_form,
            a_action,
            a_hand,
            a_type_left,
            a_form_left,
            a_action_left);

        const auto section = a_section.c_str();

        reset_section(section);

        custom_ini.SetLongValue(section, "uPage", a_page);
        custom_ini.SetLongValue(section, "uPosition", a_position);
        custom_ini.SetLongValue(section, "uType", a_type);
        custom_ini.SetValue(section, "sSelectedItemForm", a_form.c_str());
        custom_ini.SetLongValue(section, "uSlotAction", a_action);
        custom_ini.SetLongValue(section, "uHandSelection", a_hand);
        custom_ini.SetLongValue(section, "uTypeLeft", a_type_left);
        custom_ini.SetValue(section, "sSelectedItemFormLeft", a_form_left.c_str());
        custom_ini.SetLongValue(section, "uSlotActionLeft", a_action_left);

        (void)custom_ini.SaveFile(ini_path);
        read_setting();
    }
}
