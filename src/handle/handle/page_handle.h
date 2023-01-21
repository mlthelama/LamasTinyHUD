#pragma once
#include "key_position_handle.h"
#include "handle/data/data_helper.h"
#include "handle/data/page/position_setting.h"
#include "ui/image_path.h"

namespace handle {
    class page_handle {
    public:
        static page_handle* get_singleton();
        void init_page(uint32_t a_page,
            position_setting::position_type a_position,
            const std::vector<data_helper*>& data_helpers,
            slot_setting::hand_equip a_hand,
            key_position_handle*& a_key_pos);

        void init_actives(uint32_t a_page, position_setting::position_type a_position);

        void set_active_page(uint32_t a_page) const;
        void set_active_page_position(uint32_t a_page, position_setting::position_type a_pos) const;
        void set_highest_page_position(int a_page, position_setting::position_type a_pos) const;
        
        [[nodiscard]] position_setting* get_page_setting(uint32_t a_page,
            position_setting::position_type a_position) const;
        [[nodiscard]] std::map<position_setting::position_type, position_setting*> get_page(uint32_t a_page) const;
        [[nodiscard]] std::map<uint32_t, std::map<position_setting::position_type, position_setting*>>
            get_pages() const;
        [[nodiscard]] std::map<position_setting::position_type, position_setting*> get_active_page() const;
        [[nodiscard]] uint32_t get_active_page_id() const;
        [[nodiscard]] uint32_t get_next_page_id() const;
        [[nodiscard]] std::map<position_setting::position_type, position_setting*> get_active_page_position(
            position_setting::position_type a_position) const;
        [[nodiscard]] uint32_t get_active_page_id_position(position_setting::position_type a_position) const;
        [[nodiscard]] uint32_t get_next_page_id_position(position_setting::position_type a_position) const;
        [[nodiscard]] uint32_t get_next_non_empty_setting_for_position(position_setting::position_type a_position) const;
        //int for now, because also 0 can be unset
        [[nodiscard]] int get_highest_page_id_position(position_setting::position_type a_position) const;
        


        page_handle(const page_handle&) = delete;
        page_handle(page_handle&&) = delete;

        page_handle& operator=(const page_handle&) const = delete;
        page_handle& operator=(page_handle&&) const = delete;

    private:
        page_handle()
            : data_(nullptr) {}

        ~page_handle() = default;

        static void get_offset_values(position_setting::position_type a_position,
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
            std::map<uint32_t, std::map<position_setting::position_type, position_setting*>> page_settings;
            uint32_t active_page = 0;
            std::map<position_setting::position_type, uint32_t> active_page_per_position;
            std::map<position_setting::position_type, int> highest_set_page_per_position;
        };

        page_handle_data* data_;
    };
}
