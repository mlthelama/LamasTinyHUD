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

    private:
        static std::string get_section_name_for_page_position(uint32_t a_page, uint32_t a_position);

        //TODO until i feel like renaming and i can generate them
        inline static std::map<uint32_t, std::map<uint32_t, std::string>> page_position_section_map_ = {
            { 0, { { 0, "TopPage" }, { 1, "RightPage" }, { 2, "BottomPage" }, { 3, "LeftPage" } } },
            { 1,
              { { 0, "TopPageSetOne" }, { 1, "RightPageSetOne" }, { 2, "BottomPageSetOne" },
                { 3, "LeftPageSetOne" } } }
        };
    };
}
