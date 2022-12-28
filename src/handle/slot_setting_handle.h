#pragma once
#include "util/constant.h"

namespace handle {
    class slot_setting_handle {
    public:
        static slot_setting_handle* get_singleton();
        void init_top_setting(RE::TESForm* a_form, util::selection_type a_type);
        [[nodiscard]] RE::TESForm* get_top_from() const;
        [[nodiscard]] util::selection_type get_top_type() const;

        slot_setting_handle(const slot_setting_handle&) = delete;
        slot_setting_handle(slot_setting_handle&&) = delete;

        slot_setting_handle& operator=(const slot_setting_handle&) const = delete;
        slot_setting_handle& operator=(slot_setting_handle&&) const = delete;

    private:
        slot_setting_handle()
            : data_(nullptr) {}

        ~slot_setting_handle() = default;

        struct slot_setting_handle_data {
            RE::TESForm* top_setting = nullptr;
            util::selection_type top_type = util::selection_type::unset;
        };

        slot_setting_handle_data* data_;
    };
}
