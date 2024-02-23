﻿#include "page_handle.h"
#include "equip/equip_slot.h"
#include "handle/data/data_helper.h"
#include "handle/data/page/position_setting.h"
#include "handle/data/page/slot_setting.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/helper.h"
#include "util/player/player.h"
#include "util/string_util.h"

namespace handle {
    using mcm = setting::mcm_setting;
    using position_type = position_setting::position_type;

    page_handle* page_handle::get_singleton() {
        static page_handle singleton;
        return std::addressof(singleton);
    }

    void page_handle::init_page(uint32_t a_page,
        const position_type a_position,
        const std::vector<data_helper*>& data_helpers,
        const slot_setting::hand_equip a_hand,
        key_position_handle*& a_key_pos) {
        logger::trace("init page {}, position {}, data_size for settings {}, hand {} ..."sv,
            a_page,
            static_cast<uint32_t>(a_position),
            data_helpers.size(),
            static_cast<uint32_t>(a_hand));
        if (!this->data_) {
            this->data_ = new page_handle_data();
        }

        page_handle_data* data = this->data_;

        auto elden = mcm::get_elden_demon_souls();

        const auto slot_offset_x = mcm::get_hud_slot_position_offset_x();
        const auto slot_offset_y = mcm::get_hud_slot_position_offset_y();
        const auto key_offset = mcm::get_hud_key_position_offset();

        auto* page = new position_setting();
        page->position = a_position;
        page->page = a_page;

        auto* slots = new std::vector<slot_setting*>;
        for (auto* element : data_helpers) {
            logger::trace("processing form {}, type {}, action {}, left {}, actor_value {}"sv,
                element->form ? util::string_util::int_to_hex(element->form->GetFormID()) : "null",
                static_cast<int>(element->type),
                static_cast<uint32_t>(element->action_type),
                element->left,
                static_cast<int>(element->actor_value));
            auto* slot = new slot_setting();
            slot->form = element->form;
            slot->type = element->type;
            slot->action = element->action_type;
            slot->equip = a_hand;
            slot->actor_value = element->actor_value;
            RE::BGSEquipSlot* equip_slot = nullptr;
            get_equip_slots(element->type, a_hand, equip_slot, element->left);
            if (!element->form && slot->type == slot_type::consumable && slot->actor_value != RE::ActorValue::kNone) {
                get_consumable_item_count(slot->actor_value, slot->item_count);
            } else {
                get_item_count(element->form, slot->item_count, element->type);
            }

            if (slot->type == slot_type::consumable || slot->type == slot_type::scroll) {
                slot->display_item_count = true;
            }

            slot->equip_slot = equip_slot;

            slots->push_back(slot);
        }

        page->slot_settings = *slots;

        //for now the right hand or the first setting defines the icon, works well for elden.
        if (slots->front() && slots->front()->form) {
            page->icon_type = get_icon_type(slots->front()->type, slots->front()->form);
        }
        if (slots->size() == 2 && page->icon_type == icon_type::icon_default) {
            logger::debug("Could not find an Icon with first setting, try next");
            page->icon_type = get_icon_type(slots->at(1)->type, slots->at(1)->form);
        }

        //we set the icon type according to the actor value
        if (slots->front()->actor_value != RE::ActorValue::kNone && slots->front()->type == slot_type::consumable) {
            get_consumable_icon_by_actor_value(slots->front()->actor_value, page->icon_type);
        }

        auto* draw = new position_draw_setting();
        draw->width_setting = mcm::get_hud_image_position_width();
        draw->height_setting = mcm::get_hud_image_position_height();
        draw->hud_image_scale_width = mcm::get_hud_image_scale_width();
        draw->hud_image_scale_height = mcm::get_hud_image_scale_height();
        draw->background_transparency = mcm::get_background_transparency();
        draw->key_icon_scale_width = mcm::get_key_icon_scale_width();
        draw->key_icon_scale_height = mcm::get_key_icon_scale_height();
        draw->icon_scale_width = mcm::get_icon_scale_width();
        draw->icon_scale_height = mcm::get_icon_scale_height();
        draw->background_icon_transparency = mcm::get_background_icon_transparency();
        draw->icon_transparency = mcm::get_icon_transparency();
        draw->key_transparency = mcm::get_key_transparency();
        draw->slot_count_transparency = mcm::get_slot_count_transparency();
        draw->slot_item_name_transparency = mcm::get_slot_item_name_transparency();

        draw->slot_count_red = mcm::get_slot_count_red();
        draw->slot_count_green = mcm::get_slot_count_green();
        draw->slot_count_blue = mcm::get_slot_count_blue();
        draw->slot_item_red = mcm::get_slot_item_red();
        draw->slot_item_green = mcm::get_slot_item_green();
        draw->slot_item_blue = mcm::get_slot_item_blue();

        draw->alpha_slot_animation = mcm::get_alpha_slot_animation();
        draw->duration_slot_animation = mcm::get_duration_slot_animation();

        float offset_x = 0.f;
        float offset_y = 0.f;

        get_offset_values(a_position, slot_offset_x, slot_offset_y, offset_x, offset_y);

        draw->offset_slot_x = offset_x;
        draw->offset_slot_y = offset_y;

        get_offset_values(a_position, key_offset, key_offset, offset_x, offset_y);
        draw->offset_key_x = offset_x;
        draw->offset_key_y = offset_y;

        //same for now
        draw->offset_text_x = setting::mcm_setting::get_slot_count_text_offset();
        draw->offset_text_y = setting::mcm_setting::get_slot_count_text_offset();

        if ((elden || mcm::get_draw_item_name_text()) &&
            (a_position == position_type::bottom || a_position == position_type::top)) {
            page->item_name = true;
            get_offset_values(a_position,
                mcm::get_slot_item_name_offset_horizontal_x(),
                mcm::get_slot_item_name_offset_horizontal_y(),
                offset_x,
                offset_y);
            draw->offset_name_text_x = mcm::get_slot_item_name_offset_horizontal_x();
            draw->offset_name_text_y = offset_y;
        } else if ((!elden && mcm::get_draw_item_name_text()) &&
                   (a_position == position_type::left || a_position == position_type::right)) {
            page->item_name = true;
            get_offset_values(a_position,
                mcm::get_slot_item_name_offset_vertical_x(),
                mcm::get_slot_item_name_offset_vertical_y(),
                offset_x,
                offset_y);
            draw->offset_name_text_x = offset_x;
            draw->offset_name_text_y = mcm::get_slot_item_name_offset_vertical_y();
        } else {
            page->item_name = false;
            draw->offset_name_text_x = 0.f;
            draw->offset_name_text_y = 0.f;
        }

        auto* first_slot = slots->front();
        if (first_slot->item_count == 0 && ((first_slot->type == slot_type::consumable) ||
                                               (first_slot->form && first_slot->form->IsInventoryObject() &&
                                                   first_slot->form->formID != util::unarmed))) {
            draw->icon_transparency = setting::mcm_setting::get_icon_transparency_blocked();
        }

        page->draw_setting = draw;

        page->key = a_key_pos->get_key_for_position(a_position);
        page->item_name_font_size = setting::mcm_setting::get_item_name_font_size();
        page->count_font_size = setting::mcm_setting::get_slot_count_text_font_size();

        if (elden) {
            if (first_slot->type != slot_type::empty || slots->size() == 2 && slots->at(1)->type != slot_type::empty) {
                const auto config_page = static_cast<int>(a_page);
                if (const auto current_highest = get_highest_page_id_position(a_position);
                    current_highest < config_page) {
                    set_highest_page_position(config_page, a_position);
                }
            }
        }

        data->page_settings[a_page][a_position] = page;
        logger::trace("done setting page {}, position {}."sv, a_page, static_cast<uint32_t>(a_position));
    }

    void page_handle::init_actives(uint32_t a_page, position_type a_position) {
        if (!this->data_) {
            this->data_ = new page_handle_data();
        }
        page_handle_data* data = this->data_;
        logger::trace("init active page {} for position {}"sv, a_page, static_cast<uint32_t>(a_position));
        data->active_page_per_position[a_position] = a_page;
    }

    void page_handle::set_active_page(const uint32_t a_page) const {
        if (!this->data_) {
            return;
        }
        page_handle_data* data = this->data_;

        logger::trace("set active page to {}"sv, a_page);
        data->active_page = a_page;
    }

    void page_handle::set_active_page_position(const uint32_t a_page, position_type a_pos) const {
        if (!this->data_) {
            return;
        }
        page_handle_data* data = this->data_;
        logger::trace("set active page {} for position {}"sv, a_page, static_cast<uint32_t>(a_pos));
        data->active_page_per_position[a_pos] = a_page;
    }

    void page_handle::set_highest_page_position(int a_page, position_type a_pos) const {
        if (!this->data_) {
            return;
        }
        page_handle_data* data = this->data_;
        logger::trace("set highest page {} for position {}"sv, a_page, static_cast<uint32_t>(a_pos));
        data->highest_set_page_per_position[a_pos] = a_page;
    }

    position_setting* page_handle::get_page_setting(const uint32_t a_page, const position_type a_position) const {
        if (const page_handle_data* data = this->data_; data && !data->page_settings.empty() &&
                                                        data->page_settings.contains(a_page) &&
                                                        data->page_settings.at(a_page).contains(a_position)) {
            return data->page_settings.at(a_page).at(a_position);
        }
        return nullptr;
    }

    std::map<uint32_t, std::map<position_type, position_setting*>> page_handle::get_pages() const {
        if (const page_handle_data* data = this->data_; data && !data->page_settings.empty()) {
            return data->page_settings;
        }
        return {};
    }

    std::map<position_type, position_setting*> page_handle::get_active_page() const {
        if (setting::mcm_setting::get_elden_demon_souls()) {
            std::map<position_type, position_setting*> a_active;
            for (auto i = 0; i < static_cast<int>(position_type::total); ++i) {
                const auto pos = static_cast<position_type>(i);
                const auto page = get_active_page_id_position(pos);
                auto* page_setting = get_page_setting(page, pos);
                a_active.insert({ pos, page_setting });
            }
            return a_active;
        }

        if (const page_handle_data* data = this->data_;
            data && !data->page_settings.empty() && data->page_settings.contains(data->active_page)) {
            return data->page_settings.at(data->active_page);
        }

        return {};
    }

    uint32_t page_handle::get_active_page_id() const {
        if (setting::mcm_setting::get_elden_demon_souls()) {
            return 0;
        }
        if (const page_handle_data* data = this->data_; data) {
            return data->active_page;
        }
        return {};
    }

    uint32_t page_handle::get_next_page_id() const {
        if (const page_handle_data* data = this->data_; data) {
            //let's make it easy for now
            //we start at 0, so it is max count -1
            if (data->active_page < setting::mcm_setting::get_max_page_count() - 1) {
                return data->active_page + 1;
            }
            return 0;
        }
        return {};
    }

    uint32_t page_handle::get_active_page_id_position(const position_type a_position) const {
        if (const page_handle_data* data = this->data_;
            data && !data->active_page_per_position.empty() && data->active_page_per_position.contains(a_position)) {
            return data->active_page_per_position.at(a_position);
        }
        return 0;
    }

    uint32_t page_handle::get_next_page_id_position(const position_type a_position) const {
        if (const page_handle_data* data = this->data_;
            data && !data->active_page_per_position.empty() && data->active_page_per_position.contains(a_position)) {
            if (const auto current = data->active_page_per_position.at(a_position);
                current < mcm::get_max_page_count() - 1) {
                return current + 1;
            }
            return 0;
        }
        return 0;
    }

    //since we reorder 0 to highest is always set
    uint32_t page_handle::get_next_non_empty_setting_for_position(const position_type a_position) const {
        //if non found it will be 0
        const auto next = static_cast<int>(get_next_page_id_position(a_position));
        const auto highest_set = get_highest_page_id_position(a_position);
        if (next > highest_set) {
            return 0;
        }
        return next;
    }

    int page_handle::get_highest_page_id_position(const position_type a_position) const {
        if (const page_handle_data* data = this->data_; data && !data->highest_set_page_per_position.empty() &&
                                                        data->highest_set_page_per_position.contains(a_position)) {
            return data->highest_set_page_per_position.at(a_position);
        }
        return -1;
    }

    void page_handle::get_offset_values(const position_type a_position,
        const float a_setting_x,
        const float a_setting_y,
        float& offset_x,
        float& offset_y) {
        offset_x = 0.f;
        offset_y = 0.f;
        switch (a_position) {
            case position_type::top:
                offset_y = -a_setting_y;
                break;
            case position_type::right:
                offset_x = a_setting_x;
                break;
            case position_type::bottom:
                offset_y = a_setting_y;
                break;
            case position_type::left:
                offset_x = -a_setting_x;
                break;
            case position_type::total:
                break;
        }
    }

    void page_handle::get_equip_slots(const slot_setting::slot_type a_type,
        const slot_setting::hand_equip a_hand,
        RE::BGSEquipSlot*& a_slot,
        const bool a_left) {
        a_slot = nullptr;
        if ((a_type == slot_type::magic || a_type == slot_type::weapon) && a_hand == slot_setting::hand_equip::single ||
            a_type == slot_type::empty) {
            a_slot = a_left ? equip::equip_slot::get_left_hand_slot() : equip::equip_slot::get_right_hand_slot();
        }
    }

    ui::icon_image_type page_handle::get_icon_type(const slot_setting::slot_type a_type, RE::TESForm*& a_form) {
        auto icon = icon_type::icon_default;
        switch (a_type) {
            case slot_type::weapon:
                get_weapon_type_icon(a_form, icon);
                break;
            case slot_type::magic:
                get_spell_icon(a_form, icon);
                break;
            case slot_type::shout:
                icon = icon_type::shout;
                break;
            case slot_type::power:
                icon = icon_type::power;
                break;
            case slot_type::consumable:
                get_consumable_icon(a_form, icon);
                break;
            case slot_type::shield:
                icon = icon_type::shield;
                break;
            case slot_type::armor:
                get_item_icon(a_form, icon);
                break;
            case slot_type::scroll:
                icon = icon_type::scroll;
                break;
            case slot_type::light:
                icon = icon_type::torch;
                break;
            case slot_type::lantern:
                icon = icon_type::lantern;
                break;
            case slot_type::mask:
                icon = icon_type::mask;
                break;
            case slot_type::misc:
            case slot_type::empty:
                icon = icon_type::icon_default;
                break;
        }
        return icon;
    }

    void page_handle::get_weapon_type_icon(RE::TESForm*& a_form, icon_type& a_icon) {
        if (!a_form || !a_form->IsWeapon()) {
            a_icon = icon_type::icon_default;
            return;
        }
        switch (const auto* weapon = a_form->As<RE::TESObjectWEAP>(); weapon->GetWeaponType()) {
            case RE::WEAPON_TYPE::kHandToHandMelee:
                a_icon = icon_type::hand_to_hand;
                break;
            case RE::WEAPON_TYPE::kOneHandSword:
                if (weapon->HasKeywordString("WeapTypeRapier")) {
                    a_icon = icon_type::rapier;
                } else if (weapon->HasKeywordString("WeapTypeKatana")) {
                    a_icon = icon_type::katana;
                } else {
                    a_icon = icon_type::sword_one_handed;
                }
                break;
            case RE::WEAPON_TYPE::kOneHandDagger:
                if (weapon->HasKeywordString("WeapTypeClaw")) {
                    a_icon = icon_type::claw;
                } else {
                    a_icon = icon_type::dagger;
                }
                break;
            case RE::WEAPON_TYPE::kOneHandAxe:
                a_icon = icon_type::axe_one_handed;
                break;
            case RE::WEAPON_TYPE::kOneHandMace:
                if (weapon->HasKeywordString("WeapTypeWhip")) {
                    a_icon = icon_type::whip;
                } else {
                    a_icon = icon_type::mace;
                }
                break;
            case RE::WEAPON_TYPE::kTwoHandSword:
                if (weapon->HasKeywordString("WeapTypePike")) {
                    a_icon = icon_type::pike;
                } else {
                    a_icon = icon_type::sword_two_handed;
                }
                break;
            case RE::WEAPON_TYPE::kTwoHandAxe:
                if (weapon->HasKeywordString("WeapTypeHalberd")) {
                    a_icon = icon_type::halberd;
                } else if (weapon->HasKeywordString("WeapTypeQtrStaff")) {
                    a_icon = icon_type::quarter_staff;
                } else {
                    a_icon = icon_type::axe_two_handed;
                }
                break;
            case RE::WEAPON_TYPE::kBow:
                a_icon = icon_type::bow;
                break;
            case RE::WEAPON_TYPE::kStaff:
                a_icon = icon_type::staff;
                break;
            case RE::WEAPON_TYPE::kCrossbow:
                a_icon = icon_type::crossbow;
                break;
        }
    }

    void page_handle::get_spell_icon(RE::TESForm*& a_form, icon_type& a_icon) {
        if (!a_form && !a_form->Is(RE::FormType::Spell)) {
            return;
        }
        auto* spell = a_form->As<RE::SpellItem>();
        const auto* effect = spell->GetCostliestEffectItem()->baseEffect;
        auto actor_value = effect->GetMagickSkill();
        if (actor_value == RE::ActorValue::kNone) {
            actor_value = effect->data.primaryAV;
        }

        switch (actor_value) {
            case RE::ActorValue::kAlteration:
                a_icon = icon_type::alteration;
                break;
            case RE::ActorValue::kConjuration:
                a_icon = icon_type::conjuration;
                break;
            case RE::ActorValue::kDestruction:
                switch (effect->data.resistVariable) {
                    case RE::ActorValue::kResistFire:
                        a_icon = icon_type::destruction_fire;
                        break;
                    case RE::ActorValue::kResistFrost:
                        a_icon = icon_type::destruction_frost;
                        break;
                    case RE::ActorValue::kResistShock:
                        a_icon = icon_type::destruction_shock;
                        break;
                    default:
                        a_icon = icon_type::destruction;
                }
                break;
            case RE::ActorValue::kIllusion:
                a_icon = icon_type::illusion;
                break;
            case RE::ActorValue::kRestoration:
                //might not fit all spells
                a_icon = icon_type::restoration;
                break;
            default:
                a_icon = icon_type::spell_default;
        }
    }

    void page_handle::get_consumable_icon(RE::TESForm*& a_form, icon_type& a_icon) {
        if (!a_form || !a_form->Is(RE::FormType::AlchemyItem)) {
            return;
        }
        auto* alchemy_potion = a_form->As<RE::AlchemyItem>();

        if (alchemy_potion->IsFood()) {
            a_icon = icon_type::food;
            return;
        }
        if (alchemy_potion->IsPoison()) {
            a_icon = icon_type::poison_default;
            return;
        }

        auto actor_value = util::helper::get_actor_value_effect_from_potion(alchemy_potion, false);
        get_consumable_icon_by_actor_value(actor_value, a_icon);
    }

    void page_handle::get_item_count(RE::TESForm*& a_form, int32_t& a_count, const slot_type a_type) {
        if (a_type == slot_type::empty || !a_form) {
            a_count = 0;
            return;
        }

        if (a_form->IsInventoryObject()) {
            auto* player = RE::PlayerCharacter::GetSingleton();
            for (auto potential_items = player->GetInventory(); const auto& [item, invData] : potential_items) {
                if (invData.second->object->formID == a_form->formID) {
                    a_count = invData.first;
                    break;
                }
            }
        } else {
            a_count = 0;
        }
        logger::trace("Item {}, count {}"sv, a_form->GetName(), a_count);
    }

    void page_handle::get_item_icon(RE::TESForm*& a_form, icon_type& a_icon) {
        if (!a_form && !a_form->IsArmor()) {
            return;
        }
        switch (const auto* armor = a_form->As<RE::TESObjectARMO>(); armor->GetArmorType()) {
            case RE::BIPED_MODEL::ArmorType::kLightArmor:
                a_icon = icon_type::armor_light;
                break;
            case RE::BIPED_MODEL::ArmorType::kHeavyArmor:
                a_icon = icon_type::armor_heavy;
                break;
            case RE::BIPED_MODEL::ArmorType::kClothing:
                a_icon = icon_type::armor_clothing;
                break;
        }
    }
    void page_handle::get_consumable_icon_by_actor_value(RE::ActorValue& a_actor_value, icon_type& a_icon) {
        switch (a_actor_value) {
            case RE::ActorValue::kHealth:
            case RE::ActorValue::kHealRateMult:
            case RE::ActorValue::kHealRate:
                a_icon = icon_type::potion_health;
                break;
            case RE::ActorValue::kStamina:
            case RE::ActorValue::kStaminaRateMult:
            case RE::ActorValue::kStaminaRate:
                a_icon = icon_type::potion_stamina;
                break;
            case RE::ActorValue::kMagicka:
            case RE::ActorValue::kMagickaRateMult:
            case RE::ActorValue::kMagickaRate:
                a_icon = icon_type::potion_magicka;
                break;
            case RE::ActorValue::kResistFire:
                a_icon = icon_type::potion_fire_resist;
                break;
            case RE::ActorValue::kResistShock:
                a_icon = icon_type::potion_shock_resist;
                break;
            case RE::ActorValue::kResistFrost:
                a_icon = icon_type::potion_frost_resist;
                break;
            case RE::ActorValue::kResistMagic:
                a_icon = icon_type::potion_magic_resist;
                break;
            default:
                a_icon = icon_type::potion_default;
        }
    }

    void page_handle::get_consumable_item_count(RE::ActorValue& a_actor_value, int32_t& a_count) {
        auto* player = RE::PlayerCharacter::GetSingleton();
        a_count = 0;
        for (auto potential_items = util::player::get_inventory(player, RE::FormType::AlchemyItem);
             const auto& [item, inv_data] : potential_items) {
            const auto& [num_items, entry] = inv_data;
            auto alchemy_item = item->As<RE::AlchemyItem>();
            if (alchemy_item->IsPoison() || alchemy_item->IsFood()) {
                continue;
            }
            //returns currently only the types we want
            auto actor_value = util::helper::get_actor_value_effect_from_potion(item);
            if (actor_value == RE::ActorValue::kNone) {
                continue;
            }
            if (actor_value == a_actor_value) {
                a_count = a_count + num_items;
            }
        }
    }
}
