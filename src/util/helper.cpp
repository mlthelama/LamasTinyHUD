#include "helper.h"
#include "constant.h"
#include "string_util.h"
#include "setting/mcm_setting.h"

namespace util {
    std::string helper::get_mod_and_form(const RE::FormID& a_form_id) {
        std::string form_string;
        if (a_form_id == 0) {
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

            logger::info("form is from {}, local id is {}, translated {}"sv,
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
        if (a_data.size() == 1) {
            type = static_cast<uint32_t>(a_data[0]->type);
            form_string = get_mod_and_form(a_data[0]->form->formID);
            action = static_cast<uint32_t>(a_data[0]->action_type);
        }
        if (a_data.size() == 2) {
            type_left = static_cast<uint32_t>(a_data[1]->type);
            form_string_left = get_mod_and_form(a_data[1]->form->formID);
            action_left = static_cast<uint32_t>(a_data[0]->action_type);
        }
        config::mcm_setting::write_section_setting(section,
            type,
            form_string,
            action,
            a_hand,
            type_left,
            form_string_left,
            action_left);
        config::mcm_setting::read_setting();
    }

    std::string helper::get_section_name_for_page_position(const uint32_t a_page, const uint32_t a_position) {
        return page_position_section_map_[a_page][a_position];
    }
}
