#pragma once
#include "handle/data/data_helper.h"
#include "handle/data/page/position_setting.h"

namespace util {
    class helper {
    public:
        static std::string get_mod_and_form(const RE::FormID& a_form_id);

        static void write_setting_helper(uint32_t a_page,
            uint32_t a_position,
            const std::vector<data_helper*>& a_data,
            uint32_t a_hand);

        static void read_configs();
        static std::vector<std::string> get_configured_section_page_names(uint32_t a_position = 4);
        static RE::TESForm* get_form_from_mod_id_string(const std::string& a_str);
        static bool is_two_handed(RE::TESForm*& a_form);
        static handle::slot_setting::slot_type get_type(RE::TESForm*& a_form);
        static std::vector<data_helper*> get_hand_assignment(RE::TESForm*& a_form);
        static std::vector<data_helper*> get_hand_assignment(bool a_two_handed = false);
        static void write_notification(const std::string& a_string);
        static data_helper* is_suitable_for_position(RE::TESForm*& a_form,
            handle::position_setting::position_type a_position);
        static void rewrite_settings();
        static bool can_instant_cast(RE::TESForm* a_form, handle::slot_setting::slot_type a_type);
        static bool already_used(const RE::TESForm* a_form,
            handle::position_setting::position_type a_position,
            const std::vector<data_helper*>& a_config_data);
        static std::vector<std::string> search_for_config_files(bool a_elden);
        static void block_location(handle::position_setting* a_position_setting, bool a_condition);
        static RE::ActorValue get_actor_value_effect_from_potion(RE::TESForm* a_form);
        static std::string get_form_name_string_for_section(const std::string& a_str);

    private:
        static std::string get_section_name_for_page_position(uint32_t a_page, uint32_t a_position);
    };
}
