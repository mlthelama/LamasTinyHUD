﻿#include "set_setting_data.h"
#include "equip/equip_slot.h"
#include "equip/item.h"
#include "handle/ammo_handle.h"
#include "handle/extra_data_holder.h"
#include "handle/name_handle.h"
#include "handle/page_handle.h"
#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"
#include "setting_execute.h"
#include "util/constant.h"
#include "util/helper.h"
#include "util/player/player.h"
#include "util/string_util.h"

namespace processing {
    using mcm = setting::mcm_setting;
    using custom = setting::custom_setting;

    void set_setting_data::read_and_set_data() {
        logger::trace("Setting handlers, elden demon souls {} ..."sv, mcm::get_elden_demon_souls());

        handle::key_position_handle::get_singleton()->init_key_position_map();

        auto hand_assignment = util::player::get_hand_assignment();
        handle::name_handle::get_singleton()->init_names(hand_assignment);

        write_empty_config_and_init_active();

        logger::trace("continue with overwriting data from configuration ..."sv);

        process_config_data();

        //check_block(hand_assignment);

        logger::trace("done executing. return."sv);
    }

    void set_setting_data::set_new_item_count_if_needed(RE::TESBoundObject* a_object, int32_t a_count) {
        set_new_item_count(a_object, a_count);
    }

    void set_setting_data::set_single_slot(const uint32_t a_page,
        const position_type a_position,
        const std::vector<data_helper*>& a_data) {
        //well for now we have to match
        auto* key_pos = handle::key_position_handle::get_singleton();
        auto hand_equip = handle::slot_setting::hand_equip::total;
        if (const auto hand = a_data.size(); hand == 1) {
            hand_equip = handle::slot_setting::hand_equip::both;
        } else if (hand == 2) {
            hand_equip = handle::slot_setting::hand_equip::single;
        }

        if (mcm::get_elden_demon_souls()) {
            if (!a_data.empty()) {
                hand_equip = a_data.front()->two_handed ? handle::slot_setting::hand_equip::both :
                                                          handle::slot_setting::hand_equip::single;
            }
        }
        logger::trace("calling init page for page {}, position {} ..."sv, a_page, static_cast<uint32_t>(a_position));

        std::vector<data_helper*> data;
        if (a_data.empty()) {
            auto* const item = new data_helper();
            item->form = nullptr;
            item->action_type = handle::slot_setting::action_type::default_action;
            item->type = slot_type::empty;
            data.push_back(item);

            logger::warn("Got no settings in List, create empty."sv);
        }

        handle::page_handle::get_singleton()->init_page(a_page,
            a_position,
            a_data.empty() ? data : a_data,
            hand_equip,
            key_pos);

        logger::debug("calling helper to write to file"sv);
        util::helper::write_setting_to_file(a_page,
            static_cast<uint32_t>(a_position),
            a_data,
            static_cast<uint32_t>(hand_equip));
    }

    void set_setting_data::set_queue_slot(position_type a_pos, const std::vector<data_helper*>& a_data) {
        //each data item will be a new page with this position
        logger::trace("Got {} items to process"sv, a_data.size());
        if (a_data.empty()) {
            return;
        }

        auto pos = static_cast<uint32_t>(a_pos);
        auto* key_pos = handle::key_position_handle::get_singleton();
        auto* page_handle = handle::page_handle::get_singleton();
        //so we get the next we need, or we can use
        auto page = page_handle->get_highest_page_id_position(a_pos) + 1;
        for (auto* item : a_data) {
            auto hand =
                item->two_handed ? handle::slot_setting::hand_equip::both : handle::slot_setting::hand_equip::single;
            logger::trace("working page {}, pos {}"sv, page, pos);
            //for now make a vector with one item...
            std::vector<data_helper*> data;
            data.push_back(item);
            page_handle->init_page(page, a_pos, data, hand, key_pos);

            logger::debug("calling helper to write to file, page {}, pos {}"sv, page, pos);
            util::helper::write_setting_to_file(page, pos, data, static_cast<uint32_t>(hand));

            ++page;
        }
        logger::trace("done with data items"sv);
    }

    void set_setting_data::set_empty_slot(const int a_page, int a_pos, handle::key_position_handle*& a_key_pos) {
        logger::trace("setting empty setting for page {}, position {}"sv, a_page, a_pos);
        std::vector<data_helper*> data;
        auto* const item = new data_helper();
        item->form = nullptr;
        item->action_type = handle::slot_setting::action_type::default_action;
        item->type = slot_type::empty;
        data.push_back(item);

        handle::page_handle::get_singleton()->init_page(a_page,
            static_cast<position_type>(a_pos),
            data,
            handle::slot_setting::hand_equip::total,
            a_key_pos);
    }

    void set_setting_data::set_slot(const uint32_t a_page,
        position_type a_position,
        const std::string& a_form,
        uint32_t a_type,
        uint32_t a_hand,
        uint32_t a_action,
        const std::string& a_form_left,
        uint32_t a_type_left,
        const uint32_t a_action_left,
        RE::ActorValue a_actor_value,
        handle::key_position_handle*& a_key_pos,
        const std::string& a_section) {
        auto* form = util::helper::get_form_from_mod_id_string(a_form);
        auto* form_left = util::helper::get_form_from_mod_id_string(a_form_left);

        if (form == nullptr && form_left == nullptr && a_actor_value == RE::ActorValue::kNone) {
            //reset section here if allowed
            logger::info(
                "section {}, form and form left are null as well as the actor value is non. resetting if allowed."sv,
                a_section);
            if (mcm::get_auto_cleanup()) {
                custom::reset_section(a_section);
            }
            return;
        }

        auto hand = static_cast<handle::slot_setting::hand_equip>(a_hand);
        std::vector<data_helper*> data;

        auto action_check = setting::mcm_setting::get_action_check();
        logger::trace("page {}, pos {}, start working data hands {}, action_check {} ..."sv,
            a_page,
            static_cast<uint32_t>(a_position),
            a_hand,
            action_check);

        handle::slot_setting::action_type action;
        if (action_check && !mcm::get_elden_demon_souls()) {
            if (a_action == a_action_left) {
                action = static_cast<handle::slot_setting::action_type>(a_action);
            } else {
                action = handle::slot_setting::action_type::default_action;
                logger::warn("action type {} differ from action type left {}, setting both to {}"sv,
                    a_action,
                    a_action_left,
                    static_cast<uint32_t>(action));
            }
        } else {
            action = static_cast<handle::slot_setting::action_type>(a_action);
        }

        const auto type = static_cast<slot_type>(a_type);

        if (type != slot_type::magic && type != slot_type::weapon && type != slot_type::shield &&
            type != slot_type::empty) {
            hand = handle::slot_setting::hand_equip::total;
        }

        if (type == slot_type::shield) {
            logger::warn("Equipping shield on the Right hand might fail, or hand will be empty"sv);
        }

        logger::trace("start building data pos {}, form {}, type {}, action {}, hand {}"sv,
            static_cast<uint32_t>(a_position),
            form ? util::string_util::int_to_hex(form->GetFormID()) : "null",
            static_cast<int>(type),
            static_cast<uint32_t>(action),
            static_cast<uint32_t>(hand));

        if (form && action == handle::slot_setting::action_type::un_equip) {
            action = handle::slot_setting::action_type::default_action;
            logger::warn("set action to default, because form was not null but un equip was set");
        }

        if (action == handle::slot_setting::action_type::instant && form) {
            if (!util::helper::can_instant_cast(form, type)) {
                logger::warn("form {} cannot be instant cast, set to default"sv,
                    util::string_util::int_to_hex(form->GetFormID()));
                action = handle::slot_setting::action_type::default_action;
            }
        }

        auto* const item = new data_helper();
        item->form = form ? form : nullptr;
        item->type = type;
        item->action_type = action;
        item->left = false;
        item->actor_value = a_actor_value;
        data.push_back(item);

        logger::trace("checking if we need to build a second data set, already got {}"sv, data.size());

        if (hand == handle::slot_setting::hand_equip::single) {
            const auto type_left = static_cast<slot_type>(a_type_left);
            action = static_cast<handle::slot_setting::action_type>(a_action_left);
            logger::trace("start building second set data pos {}, form {}, type {}, action {}, hand {}"sv,
                static_cast<uint32_t>(a_position),
                form_left ? util::string_util::int_to_hex(form_left->GetFormID()) : "null",
                static_cast<int>(type_left),
                static_cast<uint32_t>(action),
                static_cast<uint32_t>(hand));

            if (form_left && action == handle::slot_setting::action_type::un_equip) {
                action = handle::slot_setting::action_type::default_action;
                logger::warn("set left action to default, because form was not null but un equip was set");
            }

            if (action == handle::slot_setting::action_type::instant && form_left) {
                if (!util::helper::can_instant_cast(form_left, type)) {
                    logger::warn("form {} cannot be instant cast, set to default"sv,
                        util::string_util::int_to_hex(form_left->GetFormID()));
                    action = handle::slot_setting::action_type::default_action;
                }
            }

            auto* const item_left = new data_helper();
            item_left->form = form_left ? form_left : nullptr;
            item_left->type = type_left;
            item_left->action_type = action;
            item_left->left = true;
            data.push_back(item_left);
        }

        logger::trace("build data, calling handler, data size {}"sv, data.size());

        if (!data.empty()) {
            handle::page_handle::get_singleton()->init_page(a_page, a_position, data, hand, a_key_pos);
        }
    }

    void set_setting_data::set_new_item_count(RE::TESBoundObject* a_object, int32_t a_count) {
        //just consider magic items for now, that includes
        auto* page_handle = handle::page_handle::get_singleton();
        for (auto pages = page_handle->get_pages(); auto& [page, page_settings] : pages) {
            for (auto [position, page_setting] : page_settings) {
                for (auto* setting : page_setting->slot_settings) {
                    if ((setting->form && setting->form->formID == a_object->formID) ||
                        (setting->actor_value != RE::ActorValue::kNone &&
                            util::helper::get_actor_value_effect_from_potion(a_object) != RE::ActorValue::kNone)) {
                        setting->item_count = setting->item_count + a_count;
                        logger::trace("FormId {}, new count {}, change count {}"sv,
                            util::string_util::int_to_hex(a_object->formID),
                            setting->item_count,
                            a_count);
                        block_location(page_setting, setting->item_count == 0);
                        if (setting->item_count == 0 && clean_type_allowed(setting->type)) {
                            do_cleanup(page_setting, setting);
                            if (mcm::get_elden_demon_souls()) {
                                util::helper::rewrite_settings();
                            }
                            process_config_data();
                        }
                    }
                }
            }
        }

        if (mcm::get_elden_demon_souls()) {
            //check if we have ammo to update
            const auto* ammo_handle = handle::ammo_handle::get_singleton();
            if (const auto ammo_list = ammo_handle->get_all(); !ammo_list.empty()) {
                for (auto* ammo : ammo_list) {
                    if (a_object->formID == ammo->form->formID) {
                        ammo->item_count = ammo->item_count + a_count;
                    }
                }
            }
        }
    }


    void set_setting_data::set_active_and_equip(handle::page_handle*& a_page_handle) {
        for (auto i = 0; i < static_cast<int>(position_type::total); ++i) {
            //will do for now, items could have been removed whatsoever
            auto position = static_cast<position_type>(i);
            auto page = a_page_handle->get_active_page_id_position(position);
            a_page_handle->init_actives(page, position);
        }

        get_actives_and_equip();
        logger::trace("processed actives and equip"sv);
    }

    void set_setting_data::process_config_data() {
        custom::read_setting();
        auto* key_position = handle::key_position_handle::get_singleton();
        auto* handler = handle::page_handle::get_singleton();

        if (mcm::get_elden_demon_souls()) {
            for (auto i = 0; i < static_cast<int>(position_type::total); ++i) {
                //will do for now, items could have been removed whatsoever
                handler->set_highest_page_position(-1, static_cast<position_type>(i));
            }
        }

        for (const auto sections = util::helper::get_configured_section_page_names(); const auto& section : sections) {
            set_slot(custom::get_page_by_section(section),
                static_cast<position_type>(custom::get_position_by_section(section)),
                custom::get_item_form_by_section(section),
                custom::get_type_by_section(section),
                custom::get_hand_selection_by_section(section),
                custom::get_slot_action_by_section(section),
                custom::get_item_form_left_by_section(section),
                custom::get_type_left_by_section(section),
                custom::get_slot_action_left_by_section(section),
                static_cast<RE::ActorValue>(custom::get_effect_actor_value(section)),
                key_position,
                section);
        }

        //do not trigger reequip if setting a setting is set
        if (mcm::get_elden_demon_souls()) {
            set_active_and_equip(handler);
        }
        logger::trace("processed setting data"sv);
    }
    void set_setting_data::write_empty_config_and_init_active() {
        //we start at 0, so it is max count -1
        if (!mcm::get_elden_demon_souls()) {
            if (const auto* page_handle = handle::page_handle::get_singleton();
                mcm::get_max_page_count() - 1 < page_handle->get_active_page_id()) {
                logger::warn("active page count is smaller than max count, set active to 0");
                page_handle->set_active_page(0);
            }
        }

        auto* key_position = handle::key_position_handle::get_singleton();
        //set empty for each position, it will be overwritten if it is configured
        const auto max = static_cast<int>(setting::mcm_setting::get_max_page_count());
        for (auto i = 0; i < max; ++i) {
            for (auto j = 0; j < static_cast<int>(position_type::total); ++j) {
                set_empty_slot(i, j, key_position);
            }
        }
        logger::trace("processed empty data"sv);
    }
    void set_setting_data::get_actives_and_equip() {
        if (!mcm::get_elden_demon_souls() || mcm::get_disable_re_equip_of_actives()) {
            return;
        }

        clear_hands();

        logger::trace("execute first setting for left/right/top"sv);

        auto* page_handle = handle::page_handle::get_singleton();
        auto is_right_two_handed = false;

        auto* right_position_setting =
            page_handle->get_page_setting(page_handle->get_active_page_id_position(position_type::right),
                position_type::right);
        if (right_position_setting && !right_position_setting->slot_settings.empty() &&
            right_position_setting->slot_settings.front()->form) {
            is_right_two_handed = util::helper::is_two_handed(right_position_setting->slot_settings.front()->form);
        }

        handle::position_setting* position_setting;
        if (!is_right_two_handed) {
            position_setting =
                page_handle->get_page_setting(page_handle->get_active_page_id_position(position_type::left),
                    position_type::left);
            setting_execute::execute_settings(position_setting->slot_settings);
        }

        if (right_position_setting && !right_position_setting->slot_settings.empty()) {
            setting_execute::execute_settings(right_position_setting->slot_settings);
        }

        position_setting = page_handle->get_page_setting(page_handle->get_active_page_id_position(position_type::top),
            position_type::top);
        setting_execute::execute_settings(position_setting->slot_settings, true);

        handle::extra_data_holder::get_singleton()->reset_data();
        logger::trace("done equip for first set"sv);
    }

    void set_setting_data::clear_hands() {
        logger::trace("clear hands"sv);
        auto* player = RE::PlayerCharacter::GetSingleton();
        auto* equip_manager = RE::ActorEquipManager::GetSingleton();
        auto* right = equip::equip_slot::get_right_hand_slot();
        auto* left = equip::equip_slot::get_left_hand_slot();

        //execute first setting for left, then right
        equip::equip_slot::un_equip_object_ft_dummy_dagger(right, player, equip_manager);
        equip::equip_slot::un_equip_object_ft_dummy_dagger(left, player, equip_manager);
        logger::trace("clear hands done."sv);
    }

    void set_setting_data::check_if_location_needs_block(RE::TESForm*& a_form, const bool a_equipped) {
        logger::trace("checking if location needs block, form {}, equipped {}"sv,
            a_form ? util::string_util::int_to_hex(a_form->formID) : "null",
            a_equipped);
        //is two-handed, if equipped
        //hardcode left for now, because we just need it there
        auto left_reequip_called = false;
        auto* key_handle = handle::key_position_handle::get_singleton();
        key_handle->set_position_lock(position_type::left, a_equipped ? 1 : 0);
        auto* page_handle = handle::page_handle::get_singleton();
        auto page = page_handle->get_active_page_id_position(position_type::left);
        auto* setting = page_handle->get_page_setting(page, position_type::left);
        //use settings here
        if (setting && setting->draw_setting && setting->draw_setting->icon_transparency) {
            block_location(setting, a_equipped);
        }
        //check if bow or crossbow, now we look for ammo that is in the favor list
        if (a_equipped && a_form->Is(RE::FormType::Weapon)) {
            if (const auto* weapon = a_form->As<RE::TESObjectWEAP>(); weapon->IsBow() || weapon->IsCrossbow()) {
                look_for_ammo(weapon->IsCrossbow());
                if (const auto* next_ammo = handle::ammo_handle::get_singleton()->get_next_ammo()) {
                    processing::setting_execute::execute_ammo(next_ammo);
                }
            }
        } else {
            handle::ammo_handle::get_singleton()->clear_ammo();
            //un equip armor here
            if (setting::mcm_setting::get_un_equip_ammo()) {
                equip::item::un_equip_ammo();
            }
        }

        if (!a_equipped) {
            if (!util::helper::is_two_handed(a_form)) {
                processing::setting_execute::reequip_left_hand_if_needed(setting);
                left_reequip_called = true;
            }
        }

        if (!left_reequip_called) {
            auto* obj_right =
                RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
            if ((obj_right && !util::helper::is_two_handed(obj_right)) || !obj_right) {
                processing::setting_execute::reequip_left_hand_if_needed(setting);
            }
        }
        handle::extra_data_holder::get_singleton()->reset_data();
        logger::trace("checking for block done. return."sv);
    }

    void set_setting_data::block_location(handle::position_setting* a_position_setting, bool a_condition) {
        //if true block
        if (!a_position_setting->draw_setting) {
            return;
        }
        if (a_condition) {
            a_position_setting->draw_setting->icon_transparency = setting::mcm_setting::get_icon_transparency_blocked();
        } else {
            a_position_setting->draw_setting->icon_transparency = setting::mcm_setting::get_icon_transparency();
        }
    }

    void set_setting_data::look_for_ammo(const bool a_crossbow) {
        bool only_favorite = setting::mcm_setting::get_only_favorite_ammo();
        bool sort_by_quantity = setting::mcm_setting::get_sort_arrow_by_quantity();
        const auto max_items = setting::mcm_setting::get_max_ammunition_type();
        auto* player = RE::PlayerCharacter::GetSingleton();
        const auto inv = util::player::get_inventory(player, RE::FormType::Ammo);
        const auto data_handler = RE::TESDataHandler::GetSingleton();

        std::multimap<uint32_t, handle::ammo_data*, std::greater<>> ammo_list;
        for (const auto& [item, inv_data] : inv) {
            const auto& [num_items, entry] = inv_data;
            auto* ammo = item->As<RE::TESAmmo>();

            if (!ammo->GetPlayable() || ammo->GetRuntimeData().data.flags.any(RE::AMMO_DATA::Flag::kNonPlayable)) {
                continue;
            }

            //if (ammo->formID == util::bound_arrow) {
            if (ammo->formID == data_handler->LookupFormID(util::bound_arrow, util::skyrim_esm)) {
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
                if (sort_by_quantity) {
                    ammo_list.insert({ static_cast<uint32_t>(num_items), ammo_data });
                } else {
                    ammo_list.insert({ static_cast<uint32_t>(ammo->GetRuntimeData().data.damage), ammo_data });
                }
            } else if (!a_crossbow && num_items != 0 &&
                       ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt)) {
                logger::trace("found arrow {}, damage {}, count {}"sv,
                    ammo->GetName(),
                    ammo->GetRuntimeData().data.damage,
                    num_items);
                auto* ammo_data = new handle::ammo_data();
                ammo_data->form = ammo;
                ammo_data->item_count = num_items;
                if (sort_by_quantity) {
                    ammo_list.insert({ static_cast<uint32_t>(num_items), ammo_data });
                } else {
                    ammo_list.insert({ static_cast<uint32_t>(ammo->GetRuntimeData().data.damage), ammo_data });
                }
            }
        }
        std::vector<handle::ammo_data*> sorted_ammo;
        auto* ammo_handle = handle::ammo_handle::get_singleton();
        for (auto [dmg, data] : ammo_list) {
            sorted_ammo.push_back(data);
            logger::trace("got {} count {}"sv, data->form->GetName(), data->item_count);
            if (sorted_ammo.size() == max_items) {
                break;
            }
        }
        ammo_list.clear();
        ammo_handle->init_ammo(sorted_ammo);
    }

    void set_setting_data::do_cleanup(handle::position_setting*& a_position_setting,
        handle::slot_setting*& a_slot_setting) {
        if (a_slot_setting->form && a_slot_setting->form->formID == util::unarmed) {
            logger::trace("skipping cleanup for form {}, because of unarmed"sv,
                util::string_util::int_to_hex(a_slot_setting->form->formID));
            return;
        }

        auto log_string = fmt::format("doing cleanup at page {}, position {}, form {}"sv,
            a_position_setting->page,
            static_cast<uint32_t>(a_position_setting->position),
            a_slot_setting->form ? util::string_util::int_to_hex(a_slot_setting->form->formID) : "null");
        RE::DebugNotification(log_string.c_str());
        logger::trace("{}"sv, log_string);

        if (mcm::get_elden_demon_souls()) {
            setting::custom_setting::reset_section(
                util::helper::get_section_name_for_page_position(a_position_setting->page,
                    static_cast<uint32_t>(a_position_setting->position)));
        } else {
            a_slot_setting->form = nullptr;
            a_slot_setting->type = slot_type::empty;
            a_slot_setting->action = handle::slot_setting::action_type::default_action;

            std::vector<data_helper*> data;

            for (auto* setting : a_position_setting->slot_settings) {
                auto item = new data_helper();
                item->form = setting->form;
                item->left = setting->equip_slot == equip::equip_slot::get_left_hand_slot();
                item->type = setting->type;
                item->action_type = setting->action;
                item->actor_value = setting->actor_value;
                item->two_handed = util::helper::is_two_handed(setting->form);
                data.push_back(item);
            }

            auto hand_equip = handle::slot_setting::hand_equip::total;
            if (const auto hand = data.size(); hand == 1) {
                hand_equip = handle::slot_setting::hand_equip::both;
            } else if (hand == 2) {
                hand_equip = handle::slot_setting::hand_equip::single;
            }

            util::helper::write_setting_to_file(a_position_setting->page,
                static_cast<uint32_t>(static_cast<uint32_t>(a_position_setting->position)),
                data,
                static_cast<uint32_t>(hand_equip));
        }

        logger::trace("done with cleaning at page {}, position {}, form {}"sv,
            a_position_setting->page,
            static_cast<uint32_t>(a_position_setting->position),
            a_slot_setting->form ? util::string_util::int_to_hex(a_slot_setting->form->formID) : "null");
    }

    void set_setting_data::check_config_data() {
        if (!mcm::get_auto_cleanup()) {
            return;
        }
        auto* page_handle = handle::page_handle::get_singleton();
        auto need_reprocess = false;
        for (auto pages = page_handle->get_pages(); auto& [page, page_settings] : pages) {
            for (auto [position, page_setting] : page_settings) {
                logger::trace("checking page {}, position {}"sv,
                    page_setting->page,
                    static_cast<uint32_t>(page_setting->position));
                for (auto* setting : page_setting->slot_settings) {
                    if (setting->form || (!setting->form && setting->actor_value != RE::ActorValue::kNone)) {
                        if (clean_type_allowed(setting->type)) {
                            auto has_it = util::player::has_item_or_spell(setting->form);
                            if ((!setting->form && setting->actor_value != RE::ActorValue::kNone &&
                                    setting->item_count == 0) ||
                                !has_it) {
                                //clean
                                need_reprocess = true;
                                do_cleanup(page_setting, setting);
                            }
                        }
                    }
                }
            }
        }

        if (need_reprocess) {
            util::helper::rewrite_settings();
            write_empty_config_and_init_active();
            process_config_data();
            get_actives_and_equip();
        }
    }

    void set_setting_data::default_remove(RE::TESForm* a_form) {
        if (!a_form) {
            logger::warn("form is null. return."sv);
            return;
        }

        auto* page_handle = handle::page_handle::get_singleton();
        for (auto pages = page_handle->get_pages(); auto& [page, page_settings] : pages) {
            for (auto [position, page_setting] : page_settings) {
                for (auto* setting : page_setting->slot_settings) {
                    if ((setting->form && setting->form->formID == a_form->formID) ||
                        (setting->actor_value != RE::ActorValue::kNone &&
                            util::helper::get_actor_value_effect_from_potion(a_form) != RE::ActorValue::kNone)) {
                        do_cleanup(page_setting, setting);
                        if (setting::mcm_setting::get_elden_demon_souls()) {
                            util::helper::rewrite_settings();
                        }
                        write_empty_config_and_init_active();
                        process_config_data();
                        get_actives_and_equip();
                    }
                }
            }
        }
    }

    bool set_setting_data::clean_type_allowed(slot_type a_type) {
        if (!setting::mcm_setting::get_auto_cleanup()) {
            return false;
        }
        auto allowed = false;
        switch (a_type) {
            case slot_type::weapon:
                allowed = setting::mcm_setting::get_clean_weapon();
                break;
            case slot_type::magic:
            case slot_type::power:
                allowed = setting::mcm_setting::get_clean_spell();
                break;
            case slot_type::shield:
            case slot_type::armor:
            case slot_type::lantern:
            case slot_type::mask:
                allowed = setting::mcm_setting::get_clean_armor();
                break;
            case slot_type::shout:
                allowed = setting::mcm_setting::get_clean_shout();
                break;
            case slot_type::consumable:
                allowed = setting::mcm_setting::get_clean_alchemy_item();
                break;
            case slot_type::scroll:
                allowed = setting::mcm_setting::get_clean_scroll();
                break;
            case slot_type::light:
                allowed = setting::mcm_setting::get_clean_light();
                break;
            case slot_type::empty:
            case slot_type::misc:
                allowed = false;
                break;
        }
        return allowed;
    }

    [[maybe_unused]] void set_setting_data::check_block(const std::vector<data_helper*>& data_helpers) {
        if (mcm::get_elden_demon_souls() && !mcm::get_disable_re_equip_of_actives() &&
            !RE::UI::GetSingleton()->GameIsPaused()) {
            logger::trace("continue with if location needs block, because reequip is {} and elden is {}"sv,
                mcm::get_elden_demon_souls(),
                mcm::get_disable_re_equip_of_actives());
            RE::TESForm* two_handed_form = nullptr;
            if (!data_helpers.empty() && data_helpers.size() == 1 && data_helpers.front() &&
                data_helpers.front()->form && util::helper::is_two_handed(data_helpers.front()->form)) {
                two_handed_form = data_helpers.front()->form;
            }

            if (!two_handed_form) {
                return;
            }
            logger::trace("form is {} in right hand"sv, util::string_util::int_to_hex(two_handed_form));

            auto* page_handle = handle::page_handle::get_singleton();
            auto page = page_handle->get_active_page_id_position(position_type::right);
            auto* setting = page_handle->get_page_setting(page, position_type::right);
            if (!setting->slot_settings.empty() && setting->slot_settings.front() &&
                setting->slot_settings.front()->form) {
                if (setting->slot_settings.front()->form == two_handed_form) {
                    processing::set_setting_data::check_if_location_needs_block(two_handed_form, true);
                }
            }
            logger::trace("done checking if location needs block. return.");
        }
    }
}
