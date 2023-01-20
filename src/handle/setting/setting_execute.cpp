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
#include "util/helper.h"
#include "util/string_util.h"

namespace handle {
    using mcm = config::mcm_setting;

    bool setting_execute::execute_settings(const std::vector<slot_setting*>& a_slots, const position_setting::position a_position) {
        logger::trace("got {} settings execute"sv, a_slots.size());

        if (mcm::get_elder_demon_souls() && a_position == position_setting::position::left) {
            const auto key_handle = key_position_handle::get_singleton()->get_key_for_position(position_setting::position::right);
            const auto right_setting = get_position_setting_for_key(key_handle);
            auto form = right_setting->slot_settings.front()->form;
            const auto type = right_setting->slot_settings.front()->type;
            //elden always got just one entry
            if ( form && util::helper::is_two_handed(form) && (type == slot_setting::slot_type::magic || type == slot_setting::slot_type::weapon)) {
                //not allowed to execute
                //update ui would be nice until right is not two handed any mor
                //could check every swap for right if left is "locked"
                logger::debug("not allowed to swap because right. form {} is two handed", util::string_util::int_to_hex(form->formID));
                return false;
            }
        }
        
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
        return true;
    }

    position_setting* setting_execute::get_position_setting_for_key(const uint32_t a_key) {
        const auto position = key_position_handle::get_singleton()->get_position_for_key(a_key);
        if (position == position_setting::position::total) {
            logger::warn("nothing to do, nothing set. return."sv);
            return nullptr;
        }

        const auto page_handle = page_handle::get_singleton();
        position_setting* position_setting;
        uint32_t page;
        if (mcm::get_elder_demon_souls()) {
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

    void setting_execute::execute_setting(slot_setting*& a_slot, RE::PlayerCharacter*& a_player) {
        switch (a_slot->type) {
            case slot_setting::slot_type::unset:
                logger::warn("nothing to do, nothing set"sv);
                break;
            case slot_setting::slot_type::consumable:
                item::potion::consume_potion(a_slot->form, a_player);
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
