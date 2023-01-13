#pragma once
#include "page_handle.h"
#include "util/constant.h"

namespace handle {
    class edit_handle {
    public:
        static edit_handle* get_singleton();
        void init_edit(page_setting::position a_position);

        void set_hold_data(std::vector<data_helper*> a_data) const;

        [[nodiscard]] uint32_t get_page() const;
        [[nodiscard]] page_setting::position get_position() const;
        [[nodiscard]] std::vector<data_helper*> get_hold_data() const;

        edit_handle(const edit_handle&) = delete;
        edit_handle(edit_handle&&) = delete;

        edit_handle& operator=(const edit_handle&) const = delete;
        edit_handle& operator=(edit_handle&&) const = delete;

    private:
        edit_handle()
            : data_(nullptr) {}

        ~edit_handle() = default;

        struct edit_handle_data {
            uint32_t page = util::page_count;
            page_setting::position position = page_setting::position::total;

            std::vector<data_helper*> data_hold;
        };

        edit_handle_data* data_;
    };
}
