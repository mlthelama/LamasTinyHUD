#include "set_setting_data.h"
#include "equip/equip_slot.h"
#include "handle/handle/ammo_handle.h"
#include "handle/handle/name_handle.h"
#include "handle/handle/page_handle.h"
#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"
#include "setting_execute.h"
#include "util/helper.h"
#include "util/string_util.h"
#include <handle/handle/edit_handle.h>

namespace handle {
    using mcm = config::mcm_setting;
    using custom = config::custom_setting;

    void set_setting_data::read_and_set_data() {
        logger::trace("Setting handlers, elden demon souls {} ..."sv, mcm::get_elden_demon_souls());

        auto key_position = key_position_handle::get_singleton();
        key_position->init_key_position_map();

        name_handle::get_singleton()->init_names(util::helper::get_hand_assignment());

        write_empty_config_and_init_active(key_position);

        logger::trace("continue with overwriting data from configuration ..."sv);

        process_config_data(key_position);

        logger::trace("done executing. return."sv);
    }

    void set_setting_data::set_new_item_count_if_needed(const RE::TESBoundObject* a_object, const int32_t a_count) {
        set_new_item_count(const_cast<RE::TESBoundObject*>(a_object), a_count);
    }

    void set_setting_data::set_single_slot(const uint32_t a_page,
        const position_setting::position_type a_position,
        const std::vector<data_helper*>& a_data) {
        //well for now we have to match
        auto key_pos = key_position_handle::get_singleton();
        auto hand_equip = slot_setting::hand_equip::total;
        if (const auto hand = a_data.size(); hand == 1) {
            hand_equip = slot_setting::hand_equip::both;
        } else if (hand == 2) {
            hand_equip = slot_setting::hand_equip::single;
        }

        //used for unarmed "handling"
        if (mcm::get_elden_demon_souls()) {
            hand_equip = slot_setting::hand_equip::single;
        }
        logger::trace("calling init page for page {}, position {} ..."sv, a_page, static_cast<uint32_t>(a_position));

        std::vector<data_helper*> data;
        if (a_data.empty()) {
            const auto item = new data_helper();
            item->form = nullptr;
            item->action_type = slot_setting::acton_type::default_action;
            item->type = slot_setting::slot_type::empty;
            data.push_back(item);

            logger::warn("Got no settings in List, create empty."sv);
        }

        page_handle::get_singleton()->init_page(a_page,
            a_position,
            a_data.empty() ? data : a_data,
            hand_equip,
            key_pos);

        logger::debug("calling helper to write to file"sv);
        util::helper::write_setting_helper(a_page,
            static_cast<uint32_t>(a_position),
            a_data,
            static_cast<uint32_t>(hand_equip));
    }


    void set_setting_data::set_queue_slot(position_setting::position_type a_pos,
        const std::vector<data_helper*>& a_data) {
        //each data item will be a new page with this position
        logger::trace("Got {} items to process"sv, a_data.size());
        if (a_data.empty()) {
            return;
        }

        auto pos = static_cast<uint32_t>(a_pos);
        auto key_pos = key_position_handle::get_singleton();
        const auto page_handle = page_handle::get_singleton();
        //so we get the next we need, or we can use
        auto page = page_handle->get_highest_page_id_position(a_pos) + 1;
        for (auto item : a_data) {
            auto hand = item->two_handed ? slot_setting::hand_equip::both : slot_setting::hand_equip::single;
            logger::trace("working page {}, pos {}"sv, page, pos);
            //for now make a vector with one item...
            std::vector<data_helper*> data;
            data.push_back(item);
            page_handle->init_page(page, a_pos, data, hand, key_pos);

            logger::debug("calling helper to write to file, page {}, pos {}"sv, page, pos);
            util::helper::write_setting_helper(page, pos, data, static_cast<uint32_t>(hand));

            ++page;
        }
        logger::trace("done with data items"sv);
    }

    void set_setting_data::set_empty_slot(const int a_page, int a_pos, key_position_handle*& a_key_pos) {
        logger::trace("setting empty config for page {}, position {}"sv, a_page, a_pos);
        std::vector<data_helper*> data;
        const auto item = new data_helper();
        item->form = nullptr;
        item->action_type = slot_setting::acton_type::default_action;
        item->type = slot_setting::slot_type::empty;
        data.push_back(item);

        page_handle::get_singleton()->init_page(a_page,
            static_cast<position_setting::position_type>(a_pos),
            data,
            slot_setting::hand_equip::total,
            a_key_pos);
    }

    void set_setting_data::set_slot(const uint32_t a_page,
        position_setting::position_type a_position,
        const std::string& a_form,
        uint32_t a_type,
        uint32_t a_hand,
        uint32_t a_action,
        const std::string& a_form_left,
        uint32_t a_type_left,
        const uint32_t a_action_left,
        RE::ActorValue a_actor_value,
        key_position_handle*& a_key_pos) {
        const auto form = util::helper::get_form_from_mod_id_string(a_form);
        const auto form_left = util::helper::get_form_from_mod_id_string(a_form_left);

        if (form == nullptr && form_left == nullptr && a_actor_value == RE::ActorValue::kNone) {
            return;
        }

        auto hand = static_cast<slot_setting::hand_equip>(a_hand);
        std::vector<data_helper*> data;

        auto action_check = config::mcm_setting::get_action_check();
        logger::trace("page {}, pos {}, start working data hands {}, action_check {} ..."sv,
            a_page,
            static_cast<uint32_t>(a_position),
            a_hand,
            action_check);


        slot_setting::acton_type action;
        if (action_check) {
            if (a_action == a_action_left) {
                action = static_cast<slot_setting::acton_type>(a_action);
            } else {
                action = slot_setting::acton_type::default_action;
                logger::warn("action type {} differ from action type left {}, setting both to {}"sv,
                    a_action,
                    a_action_left,
                    static_cast<uint32_t>(action));
            }
        } else {
            action = static_cast<slot_setting::acton_type>(a_action);
        }


        const auto type = static_cast<slot_setting::slot_type>(a_type);

        if (type != slot_setting::slot_type::magic && type != slot_setting::slot_type::weapon &&
            type != slot_setting::slot_type::shield && type != slot_setting::slot_type::empty) {
            hand = slot_setting::hand_equip::total;
        }

        if (type == slot_setting::slot_type::shield) {
            logger::warn("Equipping shield on the Right hand might fail, or hand will be empty"sv);
        }

        logger::trace("start building data pos {}, form {}, type {}, action {}, hand {}"sv,
            static_cast<uint32_t>(a_position),
            form ? util::string_util::int_to_hex(form->GetFormID()) : "null",
            static_cast<int>(type),
            static_cast<uint32_t>(action),
            static_cast<uint32_t>(hand));

        if (form && action == slot_setting::acton_type::un_equip) {
            action = slot_setting::acton_type::default_action;
            logger::warn("set action to default, because form was not null but un equip was set");
        }

        if (action == slot_setting::acton_type::instant && form) {
            if (!util::helper::can_instant_cast(form, type)) {
                logger::warn("form {} cannot be instant cast, set to default"sv,
                    util::string_util::int_to_hex(form->GetFormID()));
                action = slot_setting::acton_type::default_action;
            }
        }

        const auto item = new data_helper();
        item->form = form ? form : nullptr;
        item->type = type;
        item->action_type = action;
        item->left = false;
        item->actor_value = a_actor_value;
        data.push_back(item);

        logger::trace("checking if we need to build a second data set, already got {}"sv, data.size());

        if (hand == slot_setting::hand_equip::single) {
            const auto type_left = static_cast<slot_setting::slot_type>(a_type_left);
            action = static_cast<slot_setting::acton_type>(a_action_left);
            logger::trace("start building second set data pos {}, form {}, type {}, action {}, hand {}"sv,
                static_cast<uint32_t>(a_position),
                form_left ? util::string_util::int_to_hex(form_left->GetFormID()) : "null",
                static_cast<int>(type_left),
                static_cast<uint32_t>(action),
                static_cast<uint32_t>(hand));

            if (form_left && action == slot_setting::acton_type::un_equip) {
                action = slot_setting::acton_type::default_action;
                logger::warn("set left action to default, because form was not null but un equip was set");
            }

            if (action == slot_setting::acton_type::instant && form_left) {
                if (!util::helper::can_instant_cast(form_left, type)) {
                    logger::warn("form {} cannot be instant cast, set to default"sv,
                        util::string_util::int_to_hex(form_left->GetFormID()));
                    action = slot_setting::acton_type::default_action;
                }
            }

            const auto item_left = new data_helper();
            item_left->form = form_left ? form_left : nullptr;
            item_left->type = type_left;
            item_left->action_type = action;
            item_left->left = true;
            data.push_back(item_left);
        }

        logger::trace("build data, calling handler, data size {}"sv, data.size());

        if (!data.empty()) {
            page_handle::get_singleton()->init_page(a_page, a_position, data, hand, a_key_pos);
        }
    }

    void set_setting_data::set_new_item_count(RE::TESBoundObject* a_object, int32_t a_count) {
        //just consider magic items for now, that includes
        const auto page_handle = page_handle::get_singleton();
        for (auto pages = page_handle->get_pages(); auto& [page, page_settings] : pages) {
            for (auto [position, page_setting] : page_settings) {
                for (const auto setting : page_setting->slot_settings) {
                    if ((setting->type == slot_setting::slot_type::consumable ||
                            setting->type == slot_setting::slot_type::scroll) &&
                        ((setting->form && setting->form->formID == a_object->formID) ||
                            (setting->actor_value != RE::ActorValue::kNone &&
                                util::helper::get_actor_value_effect_from_potion(a_object) != RE::ActorValue::kNone))) {
                        setting->item_count = setting->item_count + a_count;
                        logger::trace("FormId {}, new count {}, change count {}"sv,
                            util::string_util::int_to_hex(a_object->formID),
                            setting->item_count,
                            a_count);
                        util::helper::block_location(page_setting, setting->item_count == 0);
                    }
                }
            }
        }

        if (mcm::get_elden_demon_souls()) {
            //check if we have ammo to update
            const auto ammo_handle = ammo_handle::get_singleton();
            if (const auto ammo_list = ammo_handle->get_all(); !ammo_list.empty()) {
                for (const auto ammo : ammo_list) {
                    if (a_object->formID == ammo->form->formID) {
                        ammo->item_count = ammo->item_count + a_count;
                    }
                }
            }
        }
    }


    void set_setting_data::set_active_and_equip(handle::page_handle*& a_page_handle) {
        for (auto i = 0; i < static_cast<int>(position_setting::position_type::total); ++i) {
            //will do for now, items could have been removed whatsoever
            auto position = static_cast<position_setting::position_type>(i);
            auto page = a_page_handle->get_active_page_id_position(position);
            a_page_handle->init_actives(page, position);
        }

        get_actives_and_equip();
        logger::trace("processed actives and equip"sv);
    }

    void set_setting_data::process_config_data(key_position_handle*& a_key_position) {
        custom::read_setting();

        auto handler = page_handle::get_singleton();
        if (mcm::get_elden_demon_souls()) {
            for (auto i = 0; i < static_cast<int>(position_setting::position_type::total); ++i) {
                //will do for now, items could have been removed whatsoever
                handler->set_highest_page_position(-1, static_cast<position_setting::position_type>(i));
            }
        }

        for (const auto sections = util::helper::get_configured_section_page_names(); const auto& section : sections) {
            set_slot(custom::get_page_by_section(section),
                static_cast<position_setting::position_type>(custom::get_position_by_section(section)),
                custom::get_item_form_by_section(section),
                custom::get_type_by_section(section),
                custom::get_hand_selection_by_section(section),
                custom::get_slot_action_by_section(section),
                custom::get_item_form_left_by_section(section),
                custom::get_type_left_by_section(section),
                custom::get_slot_action_left_by_section(section),
                static_cast<RE::ActorValue>(custom::get_effect_actor_value(section)),
                a_key_position);
        }

        //do not trigger reequip if config a config is set
        if (mcm::get_elden_demon_souls() &&
            handle::edit_handle::get_singleton()->get_position() == handle::position_setting::position_type::total) {
            set_active_and_equip(handler);
        }
        logger::trace("processed config data"sv);
    }
    void set_setting_data::write_empty_config_and_init_active(key_position_handle*& a_key_position) {
        //we start at 0, so it is max count -1
        if (!mcm::get_elden_demon_souls()) {
            if (const auto page_handle = page_handle::get_singleton();
                mcm::get_max_page_count() - 1 < page_handle->get_active_page_id()) {
                logger::warn("active page count is smaller than max count, set active to 0");
                page_handle->set_active_page(0);
            }
        }

        //set empty for each position, it will be overwritten if it is configured
        const auto max = static_cast<int>(config::mcm_setting::get_max_page_count());
        for (auto i = 0; i < max; ++i) {
            for (auto j = 0; j < static_cast<int>(position_setting::position_type::total); ++j) {
                set_empty_slot(i, j, a_key_position);
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

        auto page_handle = handle::page_handle::get_singleton();
        auto is_right_two_handed = false;

        auto right_position_setting = page_handle->get_page_setting(
            page_handle->get_active_page_id_position(position_setting::position_type::right),
            position_setting::position_type::right);
        if (right_position_setting && !right_position_setting->slot_settings.empty() &&
            right_position_setting->slot_settings.front()->form) {
            is_right_two_handed = util::helper::is_two_handed(right_position_setting->slot_settings.front()->form);
        }

        position_setting* position_setting;
        if (!is_right_two_handed) {
            position_setting = page_handle->get_page_setting(
                page_handle->get_active_page_id_position(position_setting::position_type::left),
                position_setting::position_type::left);
            setting_execute::execute_settings(position_setting->slot_settings);
        }

        setting_execute::execute_settings(right_position_setting->slot_settings);

        position_setting = page_handle->get_page_setting(
            page_handle->get_active_page_id_position(position_setting::position_type::top),
            position_setting::position_type::top);
        setting_execute::execute_settings(position_setting->slot_settings, true);

        logger::trace("done equip for first set"sv);
    }

    void set_setting_data::clear_hands() {
        logger::trace("clear hands"sv);
        auto player = RE::PlayerCharacter::GetSingleton();
        auto equip_manager = RE::ActorEquipManager::GetSingleton();
        auto right = equip::equip_slot::get_right_hand_slot();
        auto left = equip::equip_slot::get_left_hand_slot();

        //execute first setting for left, then right
        equip::equip_slot::un_equip_object_ft_dummy_dagger(right, player, equip_manager);
        equip::equip_slot::un_equip_object_ft_dummy_dagger(left, player, equip_manager);
        logger::trace("clear hands done."sv);
    }
}
