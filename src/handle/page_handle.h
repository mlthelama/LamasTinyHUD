#pragma once
#include "key_position.h"
#include "data/data_helper.h"
#include "page/page_setting.h"
#include "ui/image_path.h"

namespace handle {
    class page_handle {
    public:
        static page_handle* get_singleton();
        void init_page([[maybe_unused]] uint32_t a_page,
            page_setting::position a_position,
            const std::vector<data_helper*>& data_helpers,
            float a_slot_offset,
            float a_key_offset,
            slot_setting::hand_equip a_hand,
            uint32_t a_opacity,
            key_position*& a_key_pos);
        [[nodiscard]] page_setting* get_page_setting(page_setting::position a_position) const;
        [[nodiscard]] std::map<page_setting::position, page_setting*> get_page() const;

        page_handle(const page_handle&) = delete;
        page_handle(page_handle&&) = delete;

        page_handle& operator=(const page_handle&) const = delete;
        page_handle& operator=(page_handle&&) const = delete;

    private:
        page_handle()
            : data_(nullptr) {}

        ~page_handle() = default;

        static void get_offset_values(page_setting::position a_position,
            float a_setting,
            float& offset_x,
            float& offset_y);

        static void get_equip_slots(util::selection_type a_type,
            slot_setting::hand_equip a_hand,
            RE::BGSEquipSlot*& a_slot,
            bool a_left);

        static ui::icon_image_type get_icon_type(util::selection_type a_type, RE::TESForm*& a_form);

        struct page_handle_data {
            std::map<page_setting::position, page_setting*> page_settings;
        };

        page_handle_data* data_;
    };
}
