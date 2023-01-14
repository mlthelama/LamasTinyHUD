#include "edit_handle.h"

namespace handle {
    edit_handle* edit_handle::get_singleton() {
        static edit_handle singleton;
        return std::addressof(singleton);
    }

    void edit_handle::init_edit(const page_setting::position a_position) {
        if (!this->data_) {
            this->data_ = new edit_handle_data();
        }
        const auto page_handle = page_handle::get_singleton();
        auto page = page_handle->get_active_page_id();
        logger::trace("init edit for page {}, position {}"sv, page, static_cast<uint32_t>(a_position));

        std::vector<data_helper*> data_helpers;
        edit_handle_data* data = this->data_;
        data->page = page;
        data->position = a_position;
        data->data_hold = data_helpers;
    }

    void edit_handle::set_hold_data(std::vector<data_helper*> a_data) const {
        if (!this->data_) {
            return;
        }

        edit_handle_data* data = this->data_;
        data->data_hold.clear();
        data->data_hold = std::move(a_data);
    }

    uint32_t edit_handle::get_page() const {
        if (const edit_handle_data* data = this->data_; data) {
            return data->page;
        }
        return {};
    }

    page_setting::position edit_handle::get_position() const {
        if (const edit_handle_data* data = this->data_; data) {
            return data->position;
        }
        return page_setting::position::total;
    }

    std::vector<data_helper*> edit_handle::get_hold_data() const {
        if (const edit_handle_data* data = this->data_; data && !data->data_hold.empty()) {
            return data->data_hold;
        }
        return {};
    }
}
