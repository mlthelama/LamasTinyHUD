#include "page_handle.h"

#include "util/string_util.h"

namespace handle {
    page_handle* page_handle::get_singleton() {
        static page_handle singleton;
        return std::addressof(singleton);
    }

    void page_handle::init_page([[maybe_unused]] uint32_t a_page,
        const page_setting::position a_position,
        RE::TESForm* a_form,
        const util::selection_type a_type) {
        logger::trace("init page {}, position {}, form {}, type {} ..."sv,
            a_page,
            static_cast<uint32_t>(a_position),
            util::string_util::int_to_hex(a_form),
            static_cast<uint32_t>(a_type));
        if (!this->data_) {
            this->data_ = new page_handle_data();
        }

        page_handle_data* data = this->data_;

        auto* page = new page_setting();
        page->pos = a_position;

        auto* fade = new fade_setting();
        fade->action = fade_setting::action::unset;
        fade->alpha = fade_setting::alpha::max;

        page->fade_setting = fade;

        auto* slot = new slot_setting();
        slot->form = a_form;
        slot->type = a_type;

        auto* slots = new std::vector<slot_setting*>;
        slots->push_back(slot);

        page->slot_settings = *slots;

        data->page_settings.insert({ a_position, page });

        logger::trace("done setting page {}, position {}, form {}, type {}."sv,
            a_page,
            static_cast<uint32_t>(a_position),
            util::string_util::int_to_hex(a_form),
            static_cast<uint32_t>(a_type));
    }

    page_setting* page_handle::get_page_setting(const page_setting::position a_position) const {
        if (const page_handle_data* data = this->data_;
            data && !data_->page_settings.empty() && data_->page_settings[a_position]) {
            return data_->page_settings[a_position];
        }
        return nullptr;
    }

    std::map<page_setting::position, page_setting*> page_handle::get_page() const {
        if (const page_handle_data* data = this->data_; data && !data_->page_settings.empty()) {
            return data_->page_settings;
        }
        return {};
    }
}
