#include "set_data.h"
#include "key_position.h"
#include "page_handle.h"
#include "data/data_helper.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/helper.h"
#include "util/string_util.h"

namespace handle {
    using mcm = config::mcm_setting;

    void set_data::set_slot_data() {
        logger::trace("Setting handlers ..."sv);

        auto key_pos = key_position::get_singleton();
        key_pos->init_key_position_map();

        //set empty for each position, it will be overwritten if it is configured
        for (auto i = 0; i < util::page_count; ++i) {
            for (auto j = 0; j < static_cast<int>(page_setting::position::total); ++j) {
                set_empty_slot(i, j, key_pos);
            }
        }

        logger::trace("continue with overwriting data from configuration ..."sv);

        set_slot(0,
            page_setting::position::top,
            mcm::get_top_selected_item_form(),
            mcm::get_top_type(),
            mcm::get_top_hand_selection(),
            mcm::get_top_slot_action(),
            mcm::get_top_selected_item_form_left(),
            mcm::get_top_type_left(),
            mcm::get_top_slot_action_left(),
            key_pos);

        set_slot(0,
            page_setting::position::right,
            mcm::get_right_selected_item_form(),
            mcm::get_right_type(),
            mcm::get_right_hand_selection(),
            mcm::get_right_slot_action(),
            mcm::get_right_selected_item_form_left(),
            mcm::get_right_type_left(),
            mcm::get_right_slot_action_left(),
            key_pos);

        set_slot(0,
            page_setting::position::bottom,
            mcm::get_bottom_selected_item_form(),
            mcm::get_bottom_type(),
            mcm::get_bottom_hand_selection(),
            mcm::get_bottom_slot_action(),
            mcm::get_bottom_selected_item_form_left(),
            mcm::get_bottom_type_left(),
            mcm::get_bottom_slot_action_left(),
            key_pos);

        set_slot(0,
            page_setting::position::left,
            mcm::get_left_selected_item_form(),
            mcm::get_left_type(),
            mcm::get_left_hand_selection(),
            mcm::get_left_slot_action(),
            mcm::get_left_selected_item_form_left(),
            mcm::get_left_type_left(),
            mcm::get_left_slot_action_left(),
            key_pos);

        set_slot(1,
            page_setting::position::top,
            mcm::get_top_selected_item_form_page_one(),
            mcm::get_top_type_page_one(),
            mcm::get_top_hand_selection_page_one(),
            mcm::get_top_slot_action_page_one(),
            mcm::get_top_selected_item_form_left_page_one(),
            mcm::get_top_type_left_page_one(),
            mcm::get_top_slot_action_left_page_one(),
            key_pos);

        set_slot(1,
            page_setting::position::right,
            mcm::get_right_selected_item_form_page_one(),
            mcm::get_right_type_page_one(),
            mcm::get_right_hand_selection_page_one(),
            mcm::get_right_slot_action_page_one(),
            mcm::get_right_selected_item_form_left_page_one(),
            mcm::get_right_type_left_page_one(),
            mcm::get_right_slot_action_left_page_one(),
            key_pos);

        set_slot(1,
            page_setting::position::bottom,
            mcm::get_bottom_selected_item_form_page_one(),
            mcm::get_bottom_type_page_one(),
            mcm::get_bottom_hand_selection_page_one(),
            mcm::get_bottom_slot_action_page_one(),
            mcm::get_bottom_selected_item_form_left_page_one(),
            mcm::get_bottom_type_left_page_one(),
            mcm::get_bottom_slot_action_left_page_one(),
            key_pos);

        set_slot(1,
            page_setting::position::left,
            mcm::get_left_selected_item_form_page_one(),
            mcm::get_left_type_page_one(),
            mcm::get_left_hand_selection_page_one(),
            mcm::get_left_slot_action_page_one(),
            mcm::get_left_selected_item_form_left_page_one(),
            mcm::get_left_type_left_page_one(),
            mcm::get_left_slot_action_left_page_one(),
            key_pos);

        logger::trace("done setting. return."sv);
    }

    void set_data::set_new_item_count_if_needed(const RE::TESBoundObject* a_obj, const int32_t a_count) {
        set_new_item_count(a_obj->GetFormID(), a_obj->GetName(), a_count);
    }

    void set_data::set_single_slot(const uint32_t a_page,
        const page_setting::position a_pos,
        const std::vector<data_helper*>& a_data) {
        //well for now we have to match
        auto key_pos = key_position::get_singleton();
        auto hand_equip = slot_setting::hand_equip::total;
        if (const auto hand = a_data.size(); hand == 1) {
            hand_equip = slot_setting::hand_equip::both;
        } else if (hand == 2) {
            hand_equip = slot_setting::hand_equip::single;
        }
        logger::trace("calling init page for page {}, position {} ..."sv, a_page, static_cast<uint32_t>(a_pos));
        page_handle::get_singleton()->init_page(a_page,
            a_pos,
            a_data,
            config::mcm_setting::get_hud_slot_position_offset(),
            config::mcm_setting::get_hud_key_position_offset(),
            hand_equip,
            config::mcm_setting::get_icon_opacity(),
            key_pos);

        logger::debug("calling helper to write to file"sv);
        util::helper::write_setting_helper(a_page,
            static_cast<uint32_t>(a_pos),
            a_data,
            static_cast<uint32_t>(hand_equip));
    }

    void set_data::set_empty_slot(const int a_page, int a_pos, key_position*& a_key_pos) {
        logger::trace("setting empty config for page {}, position {}"sv, a_page, a_pos);
        std::vector<data_helper*> data;
        const auto item = new data_helper();
        item->form = nullptr;
        item->action_type = slot_setting::acton_type::default_action;
        item->type = slot_setting::slot_type::unset;
        data.push_back(item);

        page_handle::get_singleton()->init_page(a_page,
            static_cast<page_setting::position>(a_pos),
            data,
            mcm::get_hud_slot_position_offset(),
            mcm::get_hud_key_position_offset(),
            slot_setting::hand_equip::total,
            config::mcm_setting::get_icon_opacity(),
            a_key_pos);
    }

    void set_data::set_slot(const uint32_t a_page,
        page_setting::position a_pos,
        const std::string& a_form,
        uint32_t a_type,
        uint32_t a_hand,
        uint32_t a_action,
        const std::string& a_form_left,
        uint32_t a_type_left,
        const uint32_t a_action_left,
        key_position*& a_key_pos) {
        const auto form = get_form_from_mod_id_string(a_form);
        const auto form_left = get_form_from_mod_id_string(a_form_left);

        if (form == nullptr && form_left == nullptr) return;

        auto hand = static_cast<slot_setting::hand_equip>(a_hand);
        std::vector<data_helper*> data;

        auto action_check = config::mcm_setting::get_action_check();
        logger::trace("page {}, pos {}, start working data hands {}, action_check {} ..."sv,
            a_page,
            static_cast<uint32_t>(a_pos),
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


        if (form != nullptr) {
            const auto type = static_cast<slot_setting::slot_type>(a_type);

            if (type != slot_setting::slot_type::magic && type != slot_setting::slot_type::weapon && type !=
                slot_setting::slot_type::shield) {
                hand = slot_setting::hand_equip::total;
            }

            if (type == slot_setting::slot_type::shield) {
                logger::warn("Equipping shield on the Right hand might fail, or hand will be empty"sv);
            }

            logger::trace("start building data pos {}, form {}, type {}, action {}, hand {}"sv,
                static_cast<uint32_t>(a_pos),
                util::string_util::int_to_hex(form->GetFormID()),
                static_cast<int>(type),
                static_cast<uint32_t>(action),
                static_cast<uint32_t>(hand));

            const auto item = new data_helper();
            item->form = form;
            item->type = type;
            item->action_type = action;
            item->left = false;
            data.push_back(item);
        }


        logger::trace("checking if we need to build a second data set, already got {}"sv, data.size());

        if (form_left != nullptr) {
            if (const auto type_left = static_cast<slot_setting::slot_type>(a_type_left);
                (type_left == slot_setting::slot_type::magic || type_left ==
                 slot_setting::slot_type::weapon || type_left ==
                 slot_setting::slot_type::shield) && hand == slot_setting::hand_equip::single) {
                logger::trace("start building data pos {}, form {}, type {}, action {}, hand {}"sv,
                    static_cast<uint32_t>(a_pos),
                    util::string_util::int_to_hex(form_left->GetFormID()),
                    static_cast<int>(type_left),
                    static_cast<uint32_t>(action),
                    static_cast<uint32_t>(hand));

                const auto item_left = new data_helper();
                item_left->form = form_left;
                item_left->type = type_left;
                item_left->action_type = action;
                item_left->left = true;
                data.push_back(item_left);
            }
        }
        logger::trace("build data, calling handler, data size {}"sv, data.size());

        if (!data.empty()) {
            page_handle::get_singleton()->init_page(a_page,
                a_pos,
                data,
                config::mcm_setting::get_hud_slot_position_offset(),
                config::mcm_setting::get_hud_key_position_offset(),
                hand,
                config::mcm_setting::get_icon_opacity(),
                a_key_pos);
        }
    }

    void set_data::set_new_item_count(const RE::FormID a_form_id, const char* a_name, int32_t a_count) {
        //just consider magic items for now, that includes 
        const auto page_handle = page_handle::get_singleton();
        for (auto pages = page_handle->get_pages(); auto [page, page_settings] : pages) {
            for (auto [position, page_setting] : page_settings) {
                for (const auto setting : page_setting->slot_settings) {
                    if (setting->type == slot_setting::slot_type::consumable && setting->form->formID ==
                        a_form_id) {
                        setting->item_count = setting->item_count + a_count;
                        logger::trace("Name {}, new count {}, change count {}"sv, a_name, setting->item_count, a_count);

                        //TODO maybe add indicator to ui that the items are gone
                    }
                }
            }
        }
    }

    RE::TESForm* set_data::get_form_from_mod_id_string(const std::string& a_str) {
        if (!a_str.find(util::delimiter)) {
            return nullptr;
        }
        RE::TESForm* form;

        std::istringstream string_stream{ a_str };
        std::string plugin, id;

        std::getline(string_stream, plugin, *util::delimiter);
        std::getline(string_stream, id);
        RE::FormID form_id;
        std::istringstream(id) >> std::hex >> form_id;

        if (plugin.empty()) {
            return nullptr;
        }

        if (plugin == util::dynamic_name) {
            form = RE::TESForm::LookupByID(form_id);
        } else {
            logger::trace("checking mod {} for form {}"sv, plugin, form_id);

            const auto data_handler = RE::TESDataHandler::GetSingleton();
            form = data_handler->LookupForm(form_id, plugin);
        }

        if (form != nullptr) {
            logger::trace("got form id {}, name {}", util::string_util::int_to_hex(form->GetFormID()), form->GetName());
        }

        return form;
    }
}
