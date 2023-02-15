#include "equip_event.h"
#include <equip/equip_slot.h>

#include "equip/item.h"
#include "handle/data/ammo_data.h"
#include "handle/handle/ammo_handle.h"
#include "handle/handle/edit_handle.h"
#include "handle/handle/key_position_handle.h"
#include "handle/handle/name_handle.h"
#include "handle/handle/page_handle.h"
#include "handle/setting/setting_execute.h"
#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"
#include "util/helper.h"
#include "util/string_util.h"

namespace event {
    equip_event* equip_event::get_singleton() {
        static equip_event singleton;
        return std::addressof(singleton);
    }

    void equip_event::sink() { RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(get_singleton()); }

    equip_event::event_result equip_event::ProcessEvent(const RE::TESEquipEvent* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>* a_event_source) {
        if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef()) {
            return event_result::kContinue;
        }

        auto form = RE::TESForm::LookupByID(a_event->baseObject);

        if (!form) {
            return event_result::kContinue;
        }

        if (config::mcm_setting::get_draw_current_items_text() &&
            (form->IsWeapon() || form->Is(RE::FormType::Spell) || form->IsAmmo() || form->Is(RE::FormType::Light))) {
            handle::name_handle::get_singleton()->init_names(util::helper::get_hand_assignment());
        }

        //add check if we need to block left
        if (config::mcm_setting::get_elden_demon_souls() && util::helper::is_two_handed(form)) {
            check_if_location_needs_block(form, a_event->equipped);
        }

        if (handle::edit_handle::get_singleton()->get_position() == handle::position_setting::position_type::total) {
            return event_result::kContinue;
        }


        if (config::mcm_setting::get_elden_demon_souls()) {
            work_elden_demon_souls(form, a_event->equipped);
        } else {
            work_default(form, a_event->equipped);
        }

        return event_result::kContinue;
    }

    void equip_event::work_default(RE::TESForm*& a_form, const bool a_equipped) {
        if (const auto edit_handle = handle::edit_handle::get_singleton();
            edit_handle->get_position() != handle::position_setting::position_type::total && a_equipped) {
            data_.clear();
            logger::trace("Player equipped {}"sv, a_form->GetName());
            //always
            const auto type = util::helper::get_type(a_form);
            if (type == handle::slot_setting::slot_type::empty || type == handle::slot_setting::slot_type::weapon ||
                type == handle::slot_setting::slot_type::magic || type == handle::slot_setting::slot_type::shield ||
                type == handle::slot_setting::slot_type::light) {
                data_ = util::helper::get_hand_assignment(a_form);
            }
            //just if equipped

            const auto item = new data_helper();
            //magic, weapon, shield handled outside
            // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
            // ReSharper disable once CppIncompleteSwitchStatement
            switch (type) {
                case handle::slot_setting::slot_type::empty:
                    item->form = nullptr;
                    item->type = type;
                    data_.push_back(item);
                    break;
                case handle::slot_setting::slot_type::shout:
                case handle::slot_setting::slot_type::power:
                case handle::slot_setting::slot_type::armor:
                case handle::slot_setting::slot_type::scroll:
                case handle::slot_setting::slot_type::misc:
                case handle::slot_setting::slot_type::lantern:
                case handle::slot_setting::slot_type::mask:
                    item->form = a_form;
                    item->type = type;
                    data_.push_back(item);
                    break;
                case handle::slot_setting::slot_type::consumable:
                    item->form = nullptr;
                    item->type = type;
                    item->actor_value = util::helper::get_actor_value_effect_from_potion(a_form);
                    if (item->actor_value == RE::ActorValue::kNone) {
                        item->form = a_form;
                    }
                    data_.push_back(item);
                    break;
                case handle::slot_setting::slot_type::weapon:
                case handle::slot_setting::slot_type::magic:
                case handle::slot_setting::slot_type::shield:
                case handle::slot_setting::slot_type::light:
                    //not handled here
                    break;
            }

            if (type == handle::slot_setting::slot_type::consumable) {
                const auto obj = a_form->As<RE::AlchemyItem>();
                RE::PlayerCharacter::GetSingleton()->AddObjectToContainer(obj, nullptr, 1, nullptr);
            }


            for (const auto data_item : data_) {
                util::helper::write_notification(fmt::format("Name {}, Type {}, Action {}, Left {}",
                    data_item->form ? data_item->form->GetName() : "null",
                    static_cast<uint32_t>(data_item->type),
                    static_cast<uint32_t>(data_item->action_type),
                    data_item->left));
            }
            util::helper::write_notification(
                fmt::format("Got {} Setting for Position {}. Is valid until next Change."sv,
                    data_.size(),
                    static_cast<uint32_t>(edit_handle->get_position())));
            edit_handle->set_hold_data(data_);
            data_.clear();
        }
    }

    void equip_event::work_elden_demon_souls(RE::TESForm*& a_form, const bool a_equipped) {
        const auto edit_handle = handle::edit_handle::get_singleton();
        const auto check_duplicates = config::mcm_setting::get_check_duplicate_items();
        if (const auto position = edit_handle->get_position();
            position != handle::position_setting::position_type::total && a_equipped) {
            data_ = edit_handle->get_hold_data();
            const auto item = util::helper::is_suitable_for_position(a_form, position);
            if (item->form || (a_form && item->actor_value != RE::ActorValue::kNone)) {
                if (check_duplicates && util::helper::already_used(a_form, position, data_)) {
                    util::helper::write_notification(
                        fmt::format("Item {} already used in that position", a_form ? a_form->GetName() : "null"));
                } else {
                    util::helper::write_notification(fmt::format("Added Item {}", a_form ? a_form->GetName() : "null"));
                    data_.push_back(item);
                }
            } else {
                if (a_form && !a_form->Is(RE::FormType::Enchantment)) {
                    util::helper::write_notification(
                        fmt::format("Ignored Item {}, because it did not fit the requirement",
                            a_form ? a_form->GetName() : "null"));
                }
            }

            if (a_form && a_form->Is(RE::FormType::AlchemyItem)) {
                const auto obj = a_form->As<RE::AlchemyItem>();
                RE::PlayerCharacter::GetSingleton()->AddObjectToContainer(obj, nullptr, 1, nullptr);
            }

            const auto pos_max = handle::page_handle::get_singleton()->get_highest_page_id_position(position);
            auto max = config::mcm_setting::get_max_page_count() - 1;  //we start at 0 so count -1
            logger::trace("Max for Position {} is {}, already set before edit {}"sv,
                static_cast<uint32_t>(position),
                max,
                pos_max);
            if (pos_max != -1) {
                max = config::mcm_setting::get_max_page_count() - pos_max;
            }

            if (data_.size() == max || max == 0) {
                edit_handle->set_hold_data(data_);
                util::helper::write_notification(fmt::format("Max Amount of {} Reached, rest will be Ignored", max));
            }
            if (data_.size() > max) {
                util::helper::write_notification(fmt::format("Ignored Item {}", a_form ? a_form->GetName() : "null"));
            }
            edit_handle->set_hold_data(data_);
            logger::trace("Size is {}"sv, data_.size());
            data_.clear();
        }
    }

    void equip_event::look_for_ammo(const bool a_crossbow) {
        bool only_favorite = config::mcm_setting::get_only_favorite_ammo();
        const auto max_items = config::mcm_setting::get_max_ammunition_type();
        auto player = RE::PlayerCharacter::GetSingleton();
        const auto inv = equip::item::get_inventory(player, RE::FormType::Ammo);
        std::multimap<uint32_t, handle::ammo_data*, std::greater<>> ammo_list;
        for (const auto& [item, inv_data] : inv) {
            const auto& [num_items, entry] = inv_data;
            const auto ammo = item->As<RE::TESAmmo>();
            if (!ammo->GetPlayable() || ammo->GetRuntimeData().data.flags.any(RE::AMMO_DATA::Flag::kNonPlayable)) {
                continue;
            }

            if (only_favorite && !entry->IsFavorited()) {
                continue;
            }

            if (a_crossbow && ammo->GetRuntimeData().data.flags.none(RE::AMMO_DATA::Flag::kNonBolt) && num_items != 0) {
                logger::trace("found bolt {}, damage {}, count {}"sv,
                    ammo->GetName(),
                    ammo->GetRuntimeData().data.damage,
                    num_items);
                auto* ammo_data = new handle::ammo_data();
                ammo_data->form = ammo;
                ammo_data->item_count = num_items;
                ammo_list.insert({ static_cast<uint32_t>(ammo->GetRuntimeData().data.damage), ammo_data });
            } else if (!a_crossbow && num_items != 0 &&
                       ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt)) {
                logger::trace("found arrow {}, damage {}, count {}"sv,
                    ammo->GetName(),
                    ammo->GetRuntimeData().data.damage,
                    num_items);
                auto* ammo_data = new handle::ammo_data();
                ammo_data->form = ammo;
                ammo_data->item_count = num_items;
                ammo_list.insert({ static_cast<uint32_t>(ammo->GetRuntimeData().data.damage), ammo_data });
            }
            if (ammo_list.size() == max_items) {
                break;
            }
        }
        std::vector<handle::ammo_data*> sorted_ammo;
        const auto ammo_handle = handle::ammo_handle::get_singleton();
        for (auto [dmg, data] : ammo_list) {
            sorted_ammo.push_back(data);
            logger::trace("got {} count {}"sv, data->form->GetName(), data->item_count);
        }
        ammo_list.clear();
        ammo_handle->init_ammo(sorted_ammo);
    }

    void equip_event::check_if_location_needs_block(RE::TESForm*& a_form, const bool a_equipped) {
        //is two-handed, if equipped
        //hardcode left for now, because we just need it there
        auto left_reequip_called = false;
        const auto key_handle = handle::key_position_handle::get_singleton();
        key_handle->set_position_lock(handle::position_setting::position_type::left, a_equipped ? 1 : 0);
        const auto page_handle = handle::page_handle::get_singleton();
        auto page = page_handle->get_active_page_id_position(handle::position_setting::position_type::left);
        auto setting = page_handle->get_page_setting(page, handle::position_setting::position_type::left);
        //use settings here
        if (setting && setting->draw_setting && setting->draw_setting->icon_transparency) {
            util::helper::block_location(setting, a_equipped);
        }
        //check if bow or crossbow, now we look for ammo that is in the favor list
        if (a_equipped && a_form->Is(RE::FormType::Weapon)) {
            if (const auto weapon = a_form->As<RE::TESObjectWEAP>(); weapon->IsBow() || weapon->IsCrossbow()) {
                look_for_ammo(weapon->IsCrossbow());
                if (const auto next_ammo = handle::ammo_handle::get_singleton()->get_next_ammo()) {
                    handle::setting_execute::execute_ammo(next_ammo);
                }
            }
        } else {
            handle::ammo_handle::get_singleton()->clear_ammo();
            //un equip armor here
            if (config::mcm_setting::get_un_equip_ammo()) {
                equip::item::un_equip_ammo();
            }
        }

        if (!a_equipped) {
            if (!util::helper::is_two_handed(a_form)) {
                reequip_left_hand_if_needed(setting);
                left_reequip_called = true;
            }
        }

        if (!left_reequip_called) {
            auto obj_right =
                RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
            if (obj_right && !util::helper::is_two_handed(obj_right)) {
                reequip_left_hand_if_needed(setting);
            }
        }
    }

    void equip_event::reequip_left_hand_if_needed(handle::position_setting* a_setting) {
        auto left_slot = equip::equip_slot::get_left_hand_slot();
        auto equip_manager = RE::ActorEquipManager::GetSingleton();
        auto player = RE::PlayerCharacter::GetSingleton();
        equip::equip_slot::un_equip_object_ft_dummy_dagger(left_slot, player, equip_manager);
        if (a_setting && !a_setting->slot_settings.empty()) {
            handle::setting_execute::execute_settings(a_setting->slot_settings);
        }
    }
}
