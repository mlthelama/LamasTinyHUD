#include "slot_setting_handle.h"

namespace handle {
    slot_setting_handle* slot_setting_handle::get_singleton() {
        static slot_setting_handle singleton;
        return std::addressof(singleton);
    }

    void slot_setting_handle::init_top_setting(RE::TESForm* a_form, [[maybe_unused]] util::selection_type a_type) {
        if(!this->data_) {
            this->data_ = new slot_setting_handle_data();
        }
        logger::trace("set top setting from {}"sv, a_form->GetName());

        slot_setting_handle_data* data = this->data_;
        data->top_setting = a_form;
    }

    RE::TESForm* slot_setting_handle::get_top_from() const {
        if (const slot_setting_handle_data* data = this->data_; data && data->top_setting) {
            return data->top_setting;
        }
        return nullptr;
    }
}
