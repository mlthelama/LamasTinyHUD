#pragma once
#include "key_position.h"
#include "data/data_helper.h"
#include "page/page_setting.h"
#include "ui/image_path.h"

namespace handle {
    class page_handle {
    public:
        static page_handle* get_singleton();
        void init_page(uint32_t a_page,
            page_setting::position a_position,
            const std::vector<data_helper*>& data_helpers,
            float a_slot_offset,
            float a_key_offset,
            slot_setting::hand_equip a_hand,
            uint32_t a_opacity,
            key_position*& a_key_pos);

        void set_active_page(uint32_t a_page) const;

        [[nodiscard]] page_setting* get_page_setting(uint32_t a_page, page_setting::position a_position) const;
        [[nodiscard]] std::map<page_setting::position, page_setting*> get_page(uint32_t a_page) const;
        [[nodiscard]] std::map<uint32_t, std::map<page_setting::position, page_setting*>> get_pages() const;
        [[nodiscard]] std::map<page_setting::position, page_setting*> get_active_page() const;
        [[nodiscard]] uint32_t get_active_page_id() const;
        [[nodiscard]] uint32_t get_next_page_id() const;
        [[nodiscard]] uint32_t get_active_page_id_for_position(page_setting::position a_pos) const;
        [[nodiscard]] uint32_t get_next_page_id_for_position(page_setting::position a_pos) const;

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

        static void get_equip_slots(slot_setting::slot_type a_type,
            slot_setting::hand_equip a_hand,
            RE::BGSEquipSlot*& a_slot,
            bool a_left);

        static ui::icon_image_type get_icon_type(slot_setting::slot_type a_type, RE::TESForm*& a_form);
        static void get_icon_for_weapon_type(RE::TESForm*& a_form, ui::icon_image_type& a_icon);
        static void get_icon_for_spell(RE::TESForm*& a_form, ui::icon_image_type& a_icon);
        static void get_icon_for_consumable(RE::TESForm*& a_form, ui::icon_image_type& a_icon);
        static void get_item_count(RE::TESForm*& a_form, int32_t& a_count, slot_setting::slot_type a_type);
        static void get_icon_for_item(RE::TESForm*& a_form, ui::icon_image_type& a_icon);

        struct page_handle_data {
            std::map<uint32_t, std::map<page_setting::position, page_setting*>> page_settings;
            uint32_t active_page = 0;
            std::vector<page_setting::position> active_page_per_positions;
        };

        page_handle_data* data_;
    };
}
