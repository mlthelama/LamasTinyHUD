#include "page_handle.h"
#include "equip/equip_slot.h"
#include "equip/item/inventory.h"
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
                element->form != nullptr ? util::string_util::int_to_hex(element->form->GetFormID()) : "0",
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
            get_item_count(element->form, slot->item_count, element->type);
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

        //TODO for now the right hand or the first setting defines the icon
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
            a_slot = a_left ? item::equip_slot::get_left_hand_slot() : item::equip_slot::get_right_hand_slot();
        }
    }

    ui::icon_image_type page_handle::get_icon_type(const util::selection_type a_type, RE::TESForm*& a_form) {
        auto icon = ui::icon_image_type::icon_default;
        switch (a_type) {
            case util::selection_type::weapon:
                get_icon_for_weapon_type(a_form, icon);
                break;
            case util::selection_type::magic:
                get_icon_for_spell(a_form, icon);
                break;
            case util::selection_type::shout:
                icon = ui::icon_image_type::shout;
                break;
            case util::selection_type::power:
                icon = ui::icon_image_type::power;
                break;
            case util::selection_type::consumable:
                get_icon_for_consumable(a_form, icon);
                break;
            case util::selection_type::shield:
                //kinda useless atm, icon is set by the first setting, basically right hand
                icon = ui::icon_image_type::shield;
                break;
            case util::selection_type::armor:
                get_icon_for_item(a_form, icon);
                break;
        }
        return icon;
    }

    void page_handle::get_icon_for_weapon_type(RE::TESForm*& a_form, ui::icon_image_type& a_icon) {
        switch (const auto weapon = a_form->As<RE::TESObjectWEAP>(); weapon->GetWeaponType()) {
            case RE::WEAPON_TYPE::kHandToHandMelee:
                break;
            case RE::WEAPON_TYPE::kOneHandSword:
                a_icon = ui::icon_image_type::sword_one_handed;
                break;
            case RE::WEAPON_TYPE::kOneHandDagger:
                a_icon = ui::icon_image_type::dagger;
                break;
            case RE::WEAPON_TYPE::kOneHandAxe:
                a_icon = ui::icon_image_type::axe_one_handed;
                break;
            case RE::WEAPON_TYPE::kOneHandMace:
                a_icon = ui::icon_image_type::mace;
                break;
            case RE::WEAPON_TYPE::kTwoHandSword:
                a_icon = ui::icon_image_type::sword_two_handed;
                break;
            case RE::WEAPON_TYPE::kTwoHandAxe:
                a_icon = ui::icon_image_type::axe_two_handed;
                break;
            case RE::WEAPON_TYPE::kBow:
                a_icon = ui::icon_image_type::bow;
                break;
            case RE::WEAPON_TYPE::kStaff:
                a_icon = ui::icon_image_type::staff;
                break;
            case RE::WEAPON_TYPE::kCrossbow:
                a_icon = ui::icon_image_type::crossbow;
                break;
        }
    }

    void page_handle::get_icon_for_spell(RE::TESForm*& a_form, ui::icon_image_type& a_icon) {
        const auto spell = a_form->As<RE::SpellItem>();
        const auto effect = spell->GetCostliestEffectItem()->baseEffect;
        auto actor_value = effect->GetMagickSkill();
        if (actor_value == RE::ActorValue::kNone) {
            actor_value = effect->data.primaryAV;
        }

        switch (actor_value) {
            case RE::ActorValue::kAlteration:
            case RE::ActorValue::kConjuration:
            case RE::ActorValue::kDestruction:
                switch (effect->data.resistVariable) {
                    case RE::ActorValue::kResistFire:
                        a_icon = ui::icon_image_type::spell_fire;
                        break;
                    case RE::ActorValue::kResistFrost:
                        a_icon = ui::icon_image_type::spell_frost;
                        break;
                    case RE::ActorValue::kResistShock:
                        a_icon = ui::icon_image_type::spell_frost;
                        break;
                    default:
                        a_icon = ui::icon_image_type::spell_default_alt;
                }
                break;
            case RE::ActorValue::kIllusion:
                a_icon = ui::icon_image_type::spell_default;
                break;
            case RE::ActorValue::kRestoration:
                //might not fit all spells
                a_icon = ui::icon_image_type::spell_heal;
                break;
            default:
                a_icon = ui::icon_image_type::spell_default;
        }
    }

    void page_handle::get_icon_for_consumable(RE::TESForm*& a_form, ui::icon_image_type& a_icon) {
        const auto alchemy_potion = a_form->As<RE::AlchemyItem>();

        if (alchemy_potion->IsFood()) {
            a_icon = ui::icon_image_type::food;
            return;
        }
        if (alchemy_potion->IsPoison()) {
            a_icon = ui::icon_image_type::poison_default;
            return;
        }

        const auto effect = alchemy_potion->GetCostliestEffectItem()->baseEffect;
        auto actor_value = effect->GetMagickSkill();

        if (actor_value == RE::ActorValue::kNone) {
            actor_value = effect->data.primaryAV;
        }

        switch (actor_value) {
            case RE::ActorValue::kHealth:
            case RE::ActorValue::kHealRateMult:
            case RE::ActorValue::kHealRate:
                a_icon = ui::icon_image_type::potion_health;
                break;
            default:
                a_icon = ui::icon_image_type::potion_default;
        }
    }

    void page_handle::get_item_count(RE::TESForm*& a_form, int32_t& a_count, const util::selection_type a_type) {
        if (a_type != util::selection_type::consumable || a_type == util::selection_type::unset || a_form == nullptr) {
            a_count = 0;
            return;
        }
        auto player = RE::PlayerCharacter::GetSingleton();
        for (auto potential_items = item::inventory::get_inventory_magic_items(player);
             const auto& [item, invData] : potential_items) {
            if (invData.second->object->formID == a_form->formID) {
                a_count = invData.first;
                break;
            }
        }
        logger::trace("Item {}, count {}"sv, a_form->GetName(), a_count);
    }

    void page_handle::get_icon_for_item(RE::TESForm*& a_form, ui::icon_image_type& a_icon) {
        switch (const auto weapon = a_form->As<RE::TESObjectARMO>(); weapon->GetArmorType()) {
            case RE::BIPED_MODEL::ArmorType::kLightArmor:
                a_icon = ui::icon_image_type::armor_light;
                break;
            case RE::BIPED_MODEL::ArmorType::kHeavyArmor:
                a_icon = ui::icon_image_type::armor_heavy;
                break;
            case RE::BIPED_MODEL::ArmorType::kClothing:
                a_icon = ui::icon_image_type::armor_clothing;
                break;
        }
    }
}
