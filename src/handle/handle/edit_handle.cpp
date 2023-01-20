#include "edit_handle.h"
#include "page_handle.h"
#include "handle/data/data_helper.h"
#include "handle/page/position_setting.h"

#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"

namespace handle {
    edit_handle* edit_handle::get_singleton() {
        static edit_handle singleton;
        return std::addressof(singleton);
    }

    void edit_handle::init_edit(const position_setting::position a_position) {
        if (!this->data_) {
            this->data_ = new edit_handle_data();
        }
        const auto page_handle = page_handle::get_singleton();
        uint32_t page = 0;
        if (!config::mcm_setting::get_elder_demon_souls()) {
            //page does not matter for that setting, it will be a queue
            page = page_handle->get_active_page_id();
        }
        logger::trace("init edit for page {}, position {}"sv, page, static_cast<uint32_t>(a_position));

        edit_handle_data* data = this->data_;
        data->page = page;
        data->position = a_position;
        data->data_hold = {};
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

    position_setting::position edit_handle::get_position() const {
        if (const edit_handle_data* data = this->data_; data) {
            return data->position;
        }
        return position_setting::position::total;
    }

    std::vector<data_helper*> edit_handle::get_hold_data() const {
        if (const edit_handle_data* data = this->data_; data && !data->data_hold.empty()) {
            return data->data_hold;
        }
        return {};
    }
}
