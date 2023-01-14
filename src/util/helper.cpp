#include "helper.h"
#include "constant.h"
#include "string_util.h"
#include "setting/custom_setting.h"
#include "setting/file_setting.h"
#include "setting/mcm_setting.h"

namespace util {
    std::string helper::get_mod_and_form(const RE::FormID& a_form_id) {
        std::string form_string;
        if (!a_form_id) {
            return form_string;
        }

        const auto form = RE::TESForm::LookupByID(a_form_id);
        logger::trace("Item is {}, formid {}, formid not translated {}. return."sv,
            form->GetName(),
            string_util::int_to_hex(form->GetFormID()),
            form->GetFormID());


        if (form->IsDynamicForm()) {
            form_string = fmt::format("{}{}{}",
                dynamic_name,
                delimiter,
                string_util::int_to_hex(form->GetFormID()));
        } else {
            //it is not, search for the file it is from
            auto source_file = form->sourceFiles.array->front()->fileName;
            auto local_form = form->GetLocalFormID();

            logger::trace("form is from {}, local id is {}, translated {}"sv,
                source_file,
                local_form,
                string_util::int_to_hex(local_form));

            form_string = fmt::format("{}{}{}",
                source_file,
                delimiter,
                string_util::int_to_hex(local_form));
        }

        return form_string;
    }

    void helper::write_setting_helper(const uint32_t a_page,
        const uint32_t a_position,
        const std::vector<data_helper*>& a_data,
        const uint32_t a_hand) {
        const auto section = get_section_name_for_page_position(a_page, a_position);
        uint32_t type = 0;
        std::string form_string;
        uint32_t action = 0;

        uint32_t type_left = 0;
        std::string form_string_left;
        uint32_t action_left = 0;

        if (a_data.empty()) {
            return;
        }
        if (!a_data.empty()) {
            type = static_cast<uint32_t>(a_data[0]->type);
            if (a_data[0]->form) {
                form_string = get_mod_and_form(a_data[0]->form->formID);
            } else {
                form_string = "";
            }
            action = static_cast<uint32_t>(a_data[0]->action_type);
        }


        if (a_data.size() == 2) {
            type_left = static_cast<uint32_t>(a_data[1]->type);
            if (a_data[1]->form) {
                form_string_left = get_mod_and_form(a_data[1]->form->formID);
            } else {
                form_string = "";
            }
            action_left = static_cast<uint32_t>(a_data[1]->action_type);
        }

        config::mcm_setting::read_setting();

        config::custom_setting::write_section_setting(section,
            a_page,
            a_position,
            type,
            form_string,
            action,
            a_hand,
            type_left,
            form_string_left,
            action_left);
        config::custom_setting::read_setting();
    }

    void helper::read_configs() {
        config::file_setting::load_setting();
        config::mcm_setting::read_setting();
        config::custom_setting::read_setting();
    }

    std::vector<std::string> helper::get_configured_section_page_names() {
        std::vector<std::string> names;
        for (const auto entries = config::custom_setting::get_sections(); auto entry : entries) {
            names.emplace_back(entry.pItem);
        }
        logger::trace("got {} sections"sv, names.size());
        return names;
    }

    RE::TESForm* helper::get_form_from_mod_id_string(const std::string& a_str) {
        if (!a_str.find(delimiter)) {
            return nullptr;
        }
        RE::TESForm* form;

        std::istringstream string_stream{ a_str };
        std::string plugin, id;

        std::getline(string_stream, plugin, *delimiter);
        std::getline(string_stream, id);
        RE::FormID form_id;
        std::istringstream(id) >> std::hex >> form_id;

        if (plugin.empty()) {
            return nullptr;
        }

        if (plugin == dynamic_name) {
            form = RE::TESForm::LookupByID(form_id);
        } else {
            logger::trace("checking mod {} for form {}"sv, plugin, form_id);

            const auto data_handler = RE::TESDataHandler::GetSingleton();
            form = data_handler->LookupForm(form_id, plugin);
        }

        if (form != nullptr) {
            logger::trace("got form id {}, name {}", string_util::int_to_hex(form->GetFormID()), form->GetName());
        }

        return form;
    }

    std::string helper::get_section_name_for_page_position(const uint32_t a_page, const uint32_t a_position) {
        //for now i will just generate it
        return fmt::format("Page{}Position{}", a_page, a_position);
    }
}
