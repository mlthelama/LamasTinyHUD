#include "page_handle.h"
#include "equip/equip_slot.h"
#include "util/string_util.h"

namespace handle {
    page_handle* page_handle::get_singleton() {
        static page_handle singleton;
        return std::addressof(singleton);
    }

    void page_handle::init_page([[maybe_unused]] uint32_t a_page,
        const page_setting::position a_position,
        const std::vector<data_helper*>& data_helpers,
        const float a_slot_offset,
        const float a_key_offset,
        const slot_setting::hand_equip a_hand,
        const uint32_t a_opacity,
        key_position*& a_key_pos) {
        logger::trace("init page {}, position {}, data_size for settings {}, hand {} ..."sv,
            a_page,
            static_cast<uint32_t>(a_position),
            data_helpers.size(),
            static_cast<uint32_t>(a_hand));
        if (!this->data_) {
            this->data_ = new page_handle_data();
        }

        page_handle_data* data = this->data_;

        auto* page = new page_setting();
        page->pos = a_position;

        /*auto* fade = new fade_setting();
        fade->action = fade_setting::action::unset;
        fade->alpha = fade_setting::alpha::max;
        fade->current_alpha = static_cast<uint32_t>(fade_setting::alpha::max);

        page->fade_setting = fade;*/

        auto* slots = new std::vector<slot_setting*>;
        for (const auto element : data_helpers) {
            logger::trace("processing form {}, type {}, action {}, left {}"sv,
                util::string_util::int_to_hex(element->form),
                static_cast<int>(element->type),
                static_cast<uint32_t>(element->action_type),
                element->left);
            auto* slot = new slot_setting();
            slot->form = element->form;
            slot->type = element->type;
            slot->action = element->action_type;
            slot->equip = a_hand;
            RE::BGSEquipSlot* equip_slot = nullptr;
            get_equip_slots(element->type, a_hand, equip_slot, element->left);
            slot->equip_slot = equip_slot;

            slots->push_back(slot);
        }

        page->slot_settings = *slots;

        float offset_x = 0.f;
        float offset_y = 0.f;

        get_offset_values(a_position, a_slot_offset, offset_x, offset_y);

        auto* offset = new offset_setting();
        offset->offset_slot_x = offset_x;
        offset->offset_slot_y = offset_y;

        get_offset_values(a_position, a_key_offset, offset_x, offset_y);
        offset->offset_key_x = offset_x;
        offset->offset_key_y = offset_y;

        page->offset_setting = offset;

        //TODO for now
        page->icon_type = get_icon_type(slots->front()->type, slots->front()->form);
        page->icon_opacity = a_opacity;

        page->key = a_key_pos->get_key_for_position(a_position);

        data->page_settings[a_position] = page;

        logger::trace("done setting page {}, position {}."sv,
            a_page,
            static_cast<uint32_t>(a_position));
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

    void page_handle::get_offset_values(const page_setting::position a_position,
        const float a_setting,
        float& offset_x,
        float& offset_y) {
        offset_x = 0.f;
        offset_y = 0.f;
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        // ReSharper disable once CppIncompleteSwitchStatement
        switch (a_position) {
            case page_setting::position::top:
                offset_y = -a_setting;
                break;
            case page_setting::position::right:
                offset_x = a_setting;
                break;
            case page_setting::position::bottom:
                offset_y = a_setting;
                break;
            case page_setting::position::left:
                offset_x = -a_setting;
                break;
        }
    }

    void page_handle::get_equip_slots(const util::selection_type a_type,
        const slot_setting::hand_equip a_hand,
        RE::BGSEquipSlot*& a_slot,
        const bool a_left) {
        a_slot = nullptr;
        if ((a_type == util::selection_type::magic || a_type == util::selection_type::weapon) && a_hand ==
            slot_setting::hand_equip::single) {
            a_slot = a_left ? equip::equip_slot::get_left_hand_slot() : equip::equip_slot::get_right_hand_slot();
        }
    }

    ui::icon_image_type page_handle::get_icon_type(const util::selection_type a_type,
        [[maybe_unused]] RE::TESForm*& a_form) {
        auto icon = ui::icon_image_type::default_icon;
        switch (a_type) {
            case util::selection_type::weapon:
                icon = ui::icon_image_type::two_handed_sword;
                break;
            case util::selection_type::magic:
                icon = ui::icon_image_type::default_spell;
                break;
            case util::selection_type::shout:
                icon = ui::icon_image_type::shout;
                break;
            case util::selection_type::power:
                icon = ui::icon_image_type::power;
                break;
            case util::selection_type::item:
                icon = ui::icon_image_type::default_potion;
                break;
            case util::selection_type::shield:
                icon = ui::icon_image_type::shield;
                break;
        }
        return icon;
    }
}
