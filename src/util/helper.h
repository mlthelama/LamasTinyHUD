#pragma once
#include "handle/data/data_helper.h"

namespace util {
    class helper {
    public:
        static std::string get_mod_and_form(const RE::FormID& a_form_id);

        static void write_setting_helper(uint32_t a_page,
            uint32_t a_position,
            const std::vector<data_helper*>& a_data,
            uint32_t a_hand);

        static void read_configs();

        static std::vector<std::string> get_configured_section_page_names();

        static RE::TESForm* get_form_from_mod_id_string(const std::string& a_str);

    private:
        static std::string get_section_name_for_page_position(uint32_t a_page, uint32_t a_position);
    };
}
