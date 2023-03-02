#include "game_menu_setting.h"
#include "handle/handle/page_handle.h"
#include "set_setting_data.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/helper.h"
#include "util/string_util.h"

namespace handle {
    void handle::game_menu_setting::elden_souls_config(RE::TESForm* a_form,
        handle::position_setting::position_type a_position,
        bool a_overwrite) {
        std::vector<data_helper*> data;

        const auto pos_max = handle::page_handle::get_singleton()->get_highest_page_id_position(a_position);
        auto max = config::mcm_setting::get_max_page_count() - 1;  //we start at 0 so count -1
        logger::trace("Max for Position {} is {}, already set before edit {}"sv,
            static_cast<uint32_t>(a_position),
            max,
            pos_max);
        if (pos_max != -1) {
            max = config::mcm_setting::get_max_page_count() - pos_max;
        }

        //check for replace mode here
        if (data.size() == max || max == 0) {
            util::helper::write_notification(fmt::format("Can not add more Items to Position", max));
            logger::trace("Max is 0, can not add anymore, return.");
            return;
        }

        const auto check_duplicates = config::mcm_setting::get_check_duplicate_items();

        const auto item = util::helper::is_suitable_for_position(a_form, a_position);
        if (item->form || (a_form && item->actor_value != RE::ActorValue::kNone)) {
            if (check_duplicates && util::helper::already_used(a_form, a_position, data)) {
                auto log_string =
                    fmt::format("Item {} already used in that position", a_form ? a_form->GetName() : "null");
                util::helper::write_notification(log_string);
                logger::trace("{}. return."sv, log_string);  //well
                return;
            } else {
                util::helper::write_notification(fmt::format("Added Item {}", a_form ? a_form->GetName() : "null"));
                data.push_back(item);
            }
        } else {
            if (a_form && !a_form->Is(RE::FormType::Enchantment)) {
                util::helper::write_notification(fmt::format("Ignored Item {}, because it did not fit the requirement",
                    a_form ? a_form->GetName() : "null"));
            }
        }

        logger::trace("Size is {}. calling to set data now."sv, data.size());

        //use set_single_slot for replacement handling
        if (a_overwrite) {
            auto page = handle::page_handle::get_singleton()->get_active_page_id_position(a_position);
            handle::set_setting_data::set_single_slot(page, a_position, data);
        } else {
            handle::set_setting_data::set_queue_slot(a_position, data);
        }

        logger::trace("Setting done. return.");
    }
    void handle::game_menu_setting::default_config(RE::TESForm*& a_form,
        handle::position_setting::position_type a_position,
        bool a_left) {
        const auto two_handed = util::helper::is_two_handed(a_form);
        if (two_handed && a_left) {
            auto log_string = fmt::format("Going to Ignore {}, because Two Handed {} and Left {}",
                a_form ? a_form->GetName() : "null",
                two_handed,
                a_left);
            util::helper::write_notification(log_string);
            logger::trace("{}. return."sv, log_string);  //well
            return;
        }

        std::vector<data_helper*> data;
        const auto type = util::helper::get_type(a_form);
        const auto item = new data_helper();
        switch (type) {
            case handle::slot_setting::slot_type::empty:
                item->form = nullptr;
                item->type = type;
                data.push_back(item);
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
                data.push_back(item);
                break;
            case handle::slot_setting::slot_type::consumable:
                item->form = nullptr;
                item->type = type;
                item->actor_value = util::helper::get_actor_value_effect_from_potion(a_form);
                if (item->actor_value == RE::ActorValue::kNone) {
                    item->form = a_form;
                }
                data.push_back(item);
                break;
            case handle::slot_setting::slot_type::weapon:
            case handle::slot_setting::slot_type::magic:
            case handle::slot_setting::slot_type::shield:
            case handle::slot_setting::slot_type::light:
                item->form = a_form;
                item->left = a_left;
                item->type = type;
                item->action_type = handle::slot_setting::acton_type::default_action;
                data.push_back(item);
                break;
        }

        for (const auto data_item : data) {
            util::helper::write_notification(fmt::format("Name {}, Type {}, Action {}, Left {}",
                data_item->form ? data_item->form->GetName() : "null",
                static_cast<uint32_t>(data_item->type),
                static_cast<uint32_t>(data_item->action_type),
                data_item->left));
        }

        auto page_handle = handle::page_handle::get_singleton();
        auto page = page_handle->get_active_page_id();
        //for some types we need to check if there is something on the other hand
        if (type == handle::slot_setting::slot_type::weapon || type == handle::slot_setting::slot_type::magic ||
            type == handle::slot_setting::slot_type::shield || type == handle::slot_setting::slot_type::light) {
            if (!two_handed) {
                auto slot_settings = page_handle->get_page_setting(page, a_position)->slot_settings;

                std::vector<data_helper*> current_data;
                const auto item_current = new data_helper();
                item_current->form = nullptr;
                item_current->left = false;
                item_current->type = handle::slot_setting::slot_type::empty;
                item_current->action_type = handle::slot_setting::acton_type::default_action;
                current_data.push_back(item_current);

                const auto item2_current = new data_helper();
                item2_current->form = nullptr;
                item2_current->left = true;
                item2_current->type = handle::slot_setting::slot_type::empty;
                item2_current->action_type = handle::slot_setting::acton_type::default_action;
                current_data.push_back(item2_current);

                auto current_two_handed = false;
                RE::TESForm* current_right = nullptr;
                RE::TESForm* current_left = nullptr;
                if (!slot_settings.empty()) {
                    current_right = slot_settings.front()->form;
                    current_two_handed = current_right && util::helper::is_two_handed(current_right);
                }
                if (slot_settings.size() == 2) {
                    current_left = slot_settings.at(1)->form;
                }

                logger::trace("got form {}, name {} on both/right hand"sv,
                    current_right ? util::string_util::int_to_hex(current_right->GetFormID()) : "null",
                    current_right ? current_right->GetName() : "null");

                logger::trace("got form {}, name {} on left hand"sv,
                    current_left ? util::string_util::int_to_hex(current_left->GetFormID()) : "null",
                    current_left ? current_left->GetName() : "null");

                if (current_two_handed && current_right) {
                    current_data[0]->form = current_right;
                    current_data[0]->left = false;
                    current_data[0]->type = slot_settings.front()->type;
                    current_data[0]->action_type = slot_settings.front()->action;
                    current_data.erase(current_data.begin() + 1);
                }

                if (current_right) {
                    current_data[0]->form = current_right;
                    current_data[0]->left = false;
                    current_data[0]->type = slot_settings.front()->type;
                    current_data[0]->action_type = slot_settings.front()->action;
                    current_data[0]->actor_value = slot_settings.front()->actor_value;
                }

                if (current_left) {
                    current_data[1]->form = current_left;
                    current_data[1]->left = true;
                    current_data[1]->type = slot_settings.at(1)->type;
                    current_data[1]->action_type = slot_settings.at(1)->action;
                } else {
                    current_data.erase(current_data.begin() + 1);
                }

                //should be nothing we need here, overwrite everything
                if (current_data.size() == 1 && !current_two_handed) {
                    const auto item2 = new data_helper();
                    item2->form = RE::TESForm::LookupByID(util::unarmed);
                    item2->left = !a_left;  //need the opposite
                    item2->type = handle::slot_setting::slot_type::weapon;
                    item2->action_type = handle::slot_setting::acton_type::default_action;
                    data.push_back(item2);
                } else {
                    if (a_left) {
                        data.push_back(item_current);
                    } else {
                        data.push_back(item2_current);
                    }
                }
            }
        }

        logger::trace("Size is {}. calling to set data now."sv, data.size());
        for (const auto data_item : data) {
            logger::trace("Name {}, Type {}, Action {}, Left {}",
                data_item->form ? data_item->form->GetName() : "null",
                static_cast<uint32_t>(data_item->type),
                static_cast<uint32_t>(data_item->action_type),
                data_item->left);
        }
        //do things
        handle::set_setting_data::set_single_slot(page, a_position, data);
    }
}
