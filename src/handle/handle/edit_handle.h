#pragma once
#include "handle/data/data_helper.h"
#include "handle/data/page/position_setting.h"
#include "setting/mcm_setting.h"

namespace handle {
    class edit_handle {
    public:
        static edit_handle* get_singleton();
        void init_edit(position_setting::position_type a_position);

        void set_hold_data(std::vector<data_helper*> a_data) const;

        [[nodiscard]] uint32_t get_page() const;
        [[nodiscard]] position_setting::position_type get_position() const;
        [[nodiscard]] std::vector<data_helper*> get_hold_data() const;

        edit_handle(const edit_handle&) = delete;
        edit_handle(edit_handle&&) = delete;

        edit_handle& operator=(const edit_handle&) const = delete;
        edit_handle& operator=(edit_handle&&) const = delete;

    private:
        edit_handle() : data_(nullptr) {}

        ~edit_handle() = default;

        struct edit_handle_data {
            uint32_t page = config::mcm_setting::get_max_page_count();
            position_setting::position_type position = position_setting::position_type::total;

            std::vector<data_helper*> data_hold{};
        };

        edit_handle_data* data_;
    };
}
