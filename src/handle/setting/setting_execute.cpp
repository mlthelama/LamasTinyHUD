#include "setting_execute.h"
#include "equip/equip_slot.h"
#include "equip/item.h"
#include "equip/magic.h"
#include "handle/handle/key_position_handle.h"
#include "handle/handle/page_handle.h"
#include "handle/data/page/position_setting.h"
#include "setting/mcm_setting.h"
#include "util/string_util.h"

namespace handle {
    using mcm = config::mcm_setting;

    void setting_execute::execute_settings(const std::vector<slot_setting*>& a_slots) {
        logger::trace("got {} settings execute"sv, a_slots.size());
        std::vector<RE::BGSEquipSlot*> un_equip;
        auto player = RE::PlayerCharacter::GetSingleton();
        for (auto slot : a_slots) {
            if (mcm::get_elden_demon_souls() && slot->form == nullptr) {
                logger::debug("form is null and I am in elden mode, skipping."sv);
                continue;
            }

            if (!slot->form && slot->type != slot_setting::slot_type::empty) {
                logger::warn("form is null and not type empty, skipping."sv);
                continue;
            }

            if (slot->form == nullptr && slot->type == slot_setting::slot_type::empty && slot->action ==
                slot_setting::acton_type::un_equip) {
                un_equip.push_back(slot->equip_slot);
            }

            logger::trace("executing setting for type {}, action {}, form {}, left {} ..."sv,
                static_cast<uint32_t>(slot->type),
                static_cast<uint32_t>(slot->action),
                slot->form ? util::string_util::int_to_hex(slot->form->GetFormID()) : "null",
                slot->equip_slot == equip::equip_slot::get_left_hand_slot());
            execute_setting(slot, player);
        }

        if (!un_equip.empty()) {
            for (const auto slot : un_equip) {
                equip::equip_slot::un_equip_hand(slot, player, slot_setting::acton_type::un_equip);
            }
        }
    }

    position_setting* setting_execute::get_position_setting_for_key(const uint32_t a_key) {
        const auto position = key_position_handle::get_singleton()->get_position_for_key(a_key);
        if (position == position_setting::position_type::total) {
            logger::warn("nothing to do, nothing set. return."sv);
            return nullptr;
        }

        const auto page_handle = page_handle::get_singleton();
        position_setting* position_setting;
        uint32_t page;
        if (mcm::get_elden_demon_souls()) {
            page = page_handle->get_active_page_id_position(position);
            position_setting = page_handle->get_page_setting(page, position);
        } else {
            page = page_handle->get_active_page_id();
            position_setting = page_handle->get_page_setting(page, position);
        }
        if (position_setting == nullptr) {
            logger::warn("nothing to do, nothing set. return."sv);
        }
        logger::debug("page {}, position is {}, setting count {}"sv,
            page,
            static_cast<uint32_t>(position),
            position_setting->slot_settings.size());

        return position_setting;
    }

    void setting_execute::execute_ammo(const RE::TESForm* a_form) {
        if (a_form) {
            auto player = RE::PlayerCharacter::GetSingleton();
            equip::item::equip_ammo(a_form, player);
        }
    }

    void setting_execute::execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player) {
        switch (a_slot->type) {
            case slot_setting::slot_type::consumable:
                equip::item::consume_potion(a_slot->form, a_player);
                break;
            case slot_setting::slot_type::magic:
                equip::magic::magic::cast_magic(a_slot->form, a_slot->action, a_slot->equip_slot, a_player);
                break;
            case slot_setting::slot_type::shout:
                equip::magic::equip_shout(a_slot->form, a_player);
                break;
            case slot_setting::slot_type::power:
                equip::magic::equip_or_cast_power(a_slot->form, a_slot->action, a_player);
                break;
            case slot_setting::slot_type::weapon:
                equip::item::equip_weapon_or_shield(a_slot->form, a_slot->equip_slot, a_player);
                break;
            case slot_setting::slot_type::shield:
                equip::item::equip_weapon_or_shield(a_slot->form, a_slot->equip_slot, a_player, false);
                break;
            case slot_setting::slot_type::armor:
                equip::item::equip_armor(a_slot->form, a_player);
                break;
            case slot_setting::slot_type::scroll:
                equip::magic::cast_scroll(a_slot->form, a_slot->action, a_player);
                break;
            case slot_setting::slot_type::misc:
                //TODO
                logger::warn("ignoring miscitem."sv);
                break;
            case slot_setting::slot_type::empty:
                equip::equip_slot::un_equip_hand(a_slot->equip_slot, a_player, a_slot->action);
                break;
        }
    }
}
