#include "setting_execute.h"
#include "equip/equip_slot.h"
#include "equip/item/potion.h"
#include "equip/item/weapon.h"
#include "equip/magic/power.h"
#include "equip/magic/shout.h"
#include "equip/magic/spell.h"
#include "handle/handle/key_position_handle.h"
#include "handle/handle/page_handle.h"
#include "handle/page/position_setting.h"
#include "setting/mcm_setting.h"
#include "util/string_util.h"

namespace handle {
    using mcm = config::mcm_setting;

    void setting_execute::execute_settings(const std::vector<slot_setting*>& a_slots) {
        logger::trace("got {} settings execute"sv, a_slots.size());
        std::vector<RE::BGSEquipSlot*> unequip;
        auto player = RE::PlayerCharacter::GetSingleton();
        for (auto slot : a_slots) {
            if (!slot->form && slot->type != slot_setting::slot_type::empty) {
                logger::warn("form is null and not type empty, skipping."sv);
                continue;
            }

            if (slot->form == nullptr && slot->type == slot_setting::slot_type::empty && slot->action ==
                slot_setting::acton_type::unequip) {
                unequip.push_back(slot->equip_slot);
            }

            logger::trace("executing setting for type {}, action {}, form {}, left {} ..."sv,
                static_cast<uint32_t>(slot->type),
                static_cast<uint32_t>(slot->action),
                slot->form ? util::string_util::int_to_hex(slot->form->GetFormID()) : "null",
                slot->equip_slot == item::equip_slot::get_left_hand_slot());
            execute_setting(slot, player);
        }

        if (!unequip.empty()) {
            for (const auto slot : unequip) {
                item::equip_slot::unequip_hand(slot, player, slot_setting::acton_type::unequip);
            }
        }
    }

    position_setting* setting_execute::get_page_setting_for_key(const uint32_t a_key) {
        const auto position = key_position_handle::get_singleton()->get_position_for_key(a_key);
        if (position == position_setting::position::total) {
            logger::warn("nothing to do, nothing set. return."sv);
            return nullptr;
        }

        const auto page_handle = page_handle::get_singleton();
        position_setting* page_setting;
        uint32_t page;
        if (mcm::get_elder_demon_souls()) {
            page = page_handle->get_active_page_id_position(position);
            page_setting = page_handle->get_page_setting(page, position);
        } else {
            page = page_handle->get_active_page_id();
            page_setting = page_handle->get_page_setting(page, position);
        }
        if (page_setting == nullptr) {
            logger::warn("nothing to do, nothing set. return."sv);
        }
        logger::debug("page {}, position is {}, setting count {}"sv,
            page,
            static_cast<uint32_t>(position),
            page_setting->slot_settings.size());

        return page_setting;
    }

    void setting_execute::execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player) {
        switch (a_slot->type) {
            case slot_setting::slot_type::unset:
                logger::warn("nothing to do, nothing set"sv);
                break;
            case slot_setting::slot_type::consumable:
                item::potion::consume_potion(a_slot, a_player);
                break;
            case slot_setting::slot_type::magic:
                magic::spell::cast_magic(a_slot->form, a_slot->action, a_slot->equip_slot, a_player);
                break;
            case slot_setting::slot_type::shout:
                magic::shout::equip_shout(a_slot->form, a_player);
                break;
            case slot_setting::slot_type::power:
                magic::power::equip_or_cast_power(a_slot->form, a_slot->action, a_player);
                break;
            case slot_setting::slot_type::weapon:
                item::weapon::equip_weapon_or_shield(a_slot->form, a_slot->equip_slot, a_player);
                break;
            case slot_setting::slot_type::shield:
                item::weapon::equip_weapon_or_shield(a_slot->form, a_slot->equip_slot, a_player, false);
                break;
            case slot_setting::slot_type::armor:
                item::weapon::equip_armor(a_slot->form, a_player);
                break;
            case slot_setting::slot_type::scroll:
                magic::spell::cast_scroll(a_slot->form, a_slot->action, a_player);
                break;
            case slot_setting::slot_type::misc:
                //TODO
                logger::warn("ignoring miscitem."sv);
                break;
            case slot_setting::slot_type::empty:
                item::equip_slot::unequip_hand(a_slot->equip_slot, a_player, a_slot->action);
                break;
        }
    }
}
