#include "papyrus.h"

#include "handle/setting/set_setting_data.h"
#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"
#include "ui/ui_renderer.h"
#include "util/helper.h"
#include "util/string_util.h"

namespace papyrus {
    static const char* mcm_name = "LamasTinyHUD_MCM";

    void hud_mcm::on_config_close(RE::TESQuest*) {
        logger::info("on config close"sv);
        util::helper::rewrite_settings();
        config::mcm_setting::read_setting();
        handle::set_setting_data::read_and_set_data();

        logger::debug("on config close done. return."sv);
    }

    RE::BSFixedString hud_mcm::get_resolution_width(RE::TESQuest*) {
        return fmt::format(FMT_STRING("{:.2f}"), ui::ui_renderer::get_resolution_width());
    }

    RE::BSFixedString hud_mcm::get_resolution_height(RE::TESQuest*) {
        return fmt::format(FMT_STRING("{:.2f}"), ui::ui_renderer::get_resolution_height());
    }

    std::vector<RE::BSFixedString> hud_mcm::get_section_names(RE::TESQuest*) {
        const auto sections = util::helper::get_configured_section_page_names();
        std::vector<RE::BSFixedString> sections_bs_string;
        for (const auto& section : sections) {
            sections_bs_string.emplace_back(section);
        }
        logger::trace("Returning {} sections"sv, sections_bs_string.size());
        return sections_bs_string;
    }

    RE::BSFixedString hud_mcm::get_page(RE::TESQuest*, const uint32_t a_index) {
        logger::trace("page was requsted for index {}"sv, a_index);
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            return std::to_string(config::custom_setting::get_page_by_section(section));
        }
        return "";
    }

    RE::BSFixedString hud_mcm::get_position(RE::TESQuest*, const uint32_t a_index) {
        logger::trace("position was requsted for index {}"sv, a_index);
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            return std::to_string(config::custom_setting::get_position_by_section(section));
        }
        return "";
    }

    uint32_t hud_mcm::get_selection_type(RE::TESQuest*, const uint32_t a_index, const bool a_left) {
        uint32_t type = 0;
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            if (a_left) {
                type = config::custom_setting::get_type_left_by_section(section);
            } else {
                type = config::custom_setting::get_type_by_section(section);
            }
        }
        logger::trace("return type {} index {}"sv, type, a_index);
        return type;
    }

    RE::BSFixedString hud_mcm::get_form_string(RE::TESQuest*, const uint32_t a_index, const bool a_left) {
        std::string form_string;
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            if (a_left) {
                form_string = config::custom_setting::get_item_form_left_by_section(section);
            } else {
                form_string = config::custom_setting::get_item_form_by_section(section);
            }
        }
        return form_string;
    }

    uint32_t hud_mcm::get_slot_action(RE::TESQuest*, const uint32_t a_index, const bool a_left) {
        uint32_t action = 0;
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            if (a_left) {
                action = config::custom_setting::get_slot_action_left_by_section(section);
            } else {
                action = config::custom_setting::get_slot_action_by_section(section);
            }
        }
        logger::trace("return action {} index {}"sv, action, a_index);
        return action;
    }

    uint32_t hud_mcm::get_hand_selection(RE::TESQuest*, const uint32_t a_index) {
        uint32_t hand = 0;
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            hand = config::custom_setting::get_hand_selection_by_section(section);
        }
        logger::trace("return hand {} index {}"sv, hand, a_index);
        return hand;
    }

    RE::BSFixedString hud_mcm::get_form_name(RE::TESQuest*, const uint32_t a_index, const bool a_left) {
        std::string form_string;
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            if (a_left) {
                form_string = config::custom_setting::get_item_form_left_by_section(section);
            } else {
                form_string = config::custom_setting::get_item_form_by_section(section);
            }
        }

        if (form_string.empty()) {
            return form_string;
        }

        const auto form = util::helper::get_form_from_mod_id_string(form_string);
        if (form == nullptr) {
            return form_string;
        }

        return form->GetName();
    }

    void hud_mcm::reset_section(RE::TESQuest*, const uint32_t a_index) {
        logger::trace("reset section was called for index {}"sv, a_index);
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            config::custom_setting::reset_section(section);
        }
    }

    void hud_mcm::set_action_value(RE::TESQuest*, const uint32_t a_index, const bool a_left, const uint32_t a_value) {
        logger::trace("set action was called for index {}, left {}, value {}"sv, a_index, a_left, a_value);
        if (const auto section = get_section_by_index(a_index); !section.empty()) {
            if (a_left) {
                config::custom_setting::write_slot_action_left_by_section(section, a_value);
            } else {
                config::custom_setting::write_slot_action_by_section(section, a_value);
            }
        }
    }

    bool hud_mcm::Register(RE::BSScript::IVirtualMachine* a_vm) {
        a_vm->RegisterFunction("OnConfigClose", mcm_name, on_config_close);
        a_vm->RegisterFunction("GetResolutionWidth", mcm_name, get_resolution_width);
        a_vm->RegisterFunction("GetResolutionHeight", mcm_name, get_resolution_height);

        a_vm->RegisterFunction("GetSectionNames", mcm_name, get_section_names);
        a_vm->RegisterFunction("GetPage", mcm_name, get_page);
        a_vm->RegisterFunction("GetPosition", mcm_name, get_position);
        a_vm->RegisterFunction("GetSelectionType", mcm_name, get_selection_type);
        a_vm->RegisterFunction("GetFormString", mcm_name, get_form_string);
        a_vm->RegisterFunction("GetSlotAction", mcm_name, get_slot_action);
        a_vm->RegisterFunction("GetHandSelection", mcm_name, get_hand_selection);
        a_vm->RegisterFunction("GetFormName", mcm_name, get_form_name);
        a_vm->RegisterFunction("ResetSection", mcm_name, reset_section);
        a_vm->RegisterFunction("SetActionValue", mcm_name, set_action_value);

        logger::info("Registered {} class. return."sv, mcm_name);
        return true;
    }

    bool hud_mcm::is_size_ok(uint32_t a_idx, uint64_t a_size) {
        if (a_idx > a_size) {
            logger::warn("Index is {} but size is just {}, does not fit. return."sv, a_idx, a_size);
            return false;
        }
        return true;
    }

    std::string hud_mcm::get_section_by_index(const uint32_t a_index) {
        std::string section;
        if (const auto sections = util::helper::get_configured_section_page_names(); !sections.empty() && is_size_ok(
            a_index,
            sections.size())) {
            section = sections.at(a_index);
        }
        logger::trace("got section {} for index {}"sv, section, a_index);
        return section;
    }

    void Register() {
        const auto papyrus = SKSE::GetPapyrusInterface();
        papyrus->Register(hud_mcm::Register);
        logger::info("Registered papyrus functions. return."sv);
    }
}
