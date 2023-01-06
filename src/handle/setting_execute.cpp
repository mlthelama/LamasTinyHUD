#include "setting_execute.h"
#include "key_position.h"
#include "page_handle.h"
#include "item/potion.h"
#include "item/weapon.h"
#include "magic/power.h"
#include "magic/shout.h"
#include "magic/spell.h"
#include "setting/mcm_setting.h"
#include "util/string_util.h"

namespace handle {
    using mcm = config::mcm_setting;

    void setting_execute::execute_settings(const std::vector<slot_setting*>& a_slots) {
        logger::trace("got {} settings execute"sv, a_slots.size());
        auto player = RE::PlayerCharacter::GetSingleton();
        for (const auto slot : a_slots) {
            logger::trace("executing setting for type {}, action {}, form {} ..."sv,
                static_cast<uint32_t>(slot->type),
                static_cast<uint32_t>(slot->action),
                util::string_util::int_to_hex(slot->form));
            execute_setting(slot, player);
        }
    }

    page_setting* setting_execute::get_page_setting_for_key(const uint32_t a_key) {
        const auto position = key_position::get_singleton()->get_position_for_key(a_key);
        if (position == page_setting::position::total) {
            logger::warn("nothing to do, nothing set. return."sv);
            return nullptr;
        }

        const auto page_setting = page_handle::get_singleton()->get_page_setting(position);
        if (page_setting == nullptr) {
            logger::warn("nothing to do, nothing set. return."sv);
        }
        logger::debug("position is {}, setting count {}"sv,
            static_cast<uint32_t>(position),
            page_setting->slot_settings.size());

        return page_setting;
    }

    void setting_execute::unequip_if_equipped(const bool a_left,
        RE::PlayerCharacter*& a_player,
        RE::ActorEquipManager*& a_actor_equip_manager) {
        if (const auto inv_entry = a_player->GetEquippedEntryData(a_left); inv_entry != nullptr) {
            logger::trace("Item {} is equipped, unequipping"sv, inv_entry->GetDisplayName());
            a_actor_equip_manager->UnequipObject(a_player, inv_entry->object);
        }

        if (const auto object = a_player->GetEquippedObject(a_left); object != nullptr) {
            logger::trace("Object {} is equipped, unequipping"sv, object->GetName());
            const auto bound_object = object->As<RE::TESBoundObject>();
            a_actor_equip_manager->UnequipObject(a_player, bound_object);
        }
    }

    void setting_execute::execute_setting(const slot_setting* a_slot, RE::PlayerCharacter*& a_player) {
        switch (a_slot->type) {
            case util::selection_type::unset:
                logger::warn("nothing to do, nothing set"sv);
                break;
            case util::selection_type::item:
                item::potion::consume_potion(a_slot->form, a_player);
                break;
            case util::selection_type::magic:
                magic::spell::cast_magic(a_slot->form, a_slot->action, a_slot->equip_slot, a_player);
                break;
            case util::selection_type::shout:
                magic::shout::equip_shout(a_slot->form, a_player);
                break;
            case util::selection_type::power:
                magic::power::equip_or_cast_power(a_slot->form, a_slot->action, a_player);
                break;
            case util::selection_type::weapon:
                item::weapon::equip_weapon_or_shield(a_slot->form, a_slot->equip_slot, a_player);
                break;
            case util::selection_type::shield:
                //should work for shields as well
                item::weapon::equip_weapon_or_shield(a_slot->form, a_slot->equip_slot, a_player, false);
                break;
        }
    }
}
