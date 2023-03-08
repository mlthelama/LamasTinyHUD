#include "game_menu_setting.h"
#include "handle/page_handle.h"
#include "set_setting_data.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/helper.h"
#include "util/string_util.h"

namespace processing {
    void game_menu_setting::elden_souls_config(RE::TESForm* a_form, position_type a_position, bool a_overwrite) {
        std::vector<data_helper*> data;

        write_notification(fmt::format("Elden Souls Config, Position {}, overwrite {}"sv,
            static_cast<uint32_t>(a_position),
            static_cast<uint32_t>(a_overwrite)));
        const auto pos_max = handle::page_handle::get_singleton()->get_highest_page_id_position(a_position);
        auto max = config::mcm_setting::get_max_page_count() - 1;  //we start at 0 so count -1
        logger::trace("Max for Position {} is {}, already set before edit {}"sv,
            static_cast<uint32_t>(a_position),
            max,
            pos_max);
        if (pos_max != -1) {
            max = config::mcm_setting::get_max_page_count() - pos_max;
        }

        if (!a_overwrite && (data.size() == max || max == 0)) {
            write_notification(fmt::format("Can not add more Items to Position", max));
            logger::trace("Max is 0, can not add anymore, return.");
            return;
        }

        const auto check_duplicates = config::mcm_setting::get_check_duplicate_items();

        const auto item = is_suitable_for_position(a_form, a_position);
        if (item->form || (a_form && item->actor_value != RE::ActorValue::kNone)) {
            if (check_duplicates && util::helper::already_used(a_form, a_position, data)) {
                auto log_string =
                    fmt::format("Item {} already used in that position", a_form ? a_form->GetName() : "null");
                write_notification(log_string);
                logger::trace("{}. return."sv, log_string);  //well
                return;
            } else {
                write_notification(fmt::format("Added Item {}", a_form ? a_form->GetName() : "null"));
                data.push_back(item);
            }
        } else {
            if (a_form && !a_form->Is(RE::FormType::Enchantment)) {
                write_notification(
                    fmt::format("Ignored Item {}, because it did not fit the requirement", a_form->GetName()));
            }
        }

        logger::trace("Size is {}. calling to set data now, overwrite is {}."sv,
            data.size(),
            static_cast<uint32_t>(a_overwrite));

        //use set_single_slot for replacement handling
        if (a_overwrite) {
            auto page = handle::page_handle::get_singleton()->get_active_page_id_position(a_position);
            processing::set_setting_data::set_single_slot(page, a_position, data);
        } else {
            processing::set_setting_data::set_queue_slot(a_position, data);
        }

        logger::trace("Setting done. return.");
    }
    void game_menu_setting::default_config(RE::TESForm*& a_form, position_type a_position_type, bool a_left) {
        const auto two_handed = util::helper::is_two_handed(a_form);
        if (two_handed && a_left) {
            auto log_string = fmt::format("Going to Ignore {}, because Two Handed {} and Left {}",
                a_form ? a_form->GetName() : "null",
                two_handed,
                a_left);
            write_notification(log_string);
            logger::trace("{}. return."sv, log_string);  //well
            return;
        }

        std::vector<data_helper*> data;
        const auto type = util::helper::get_type(a_form);
        const auto item = new data_helper();
        switch (type) {
            case slot_type::empty:
                item->form = nullptr;
                item->type = type;
                data.push_back(item);
                break;
            case slot_type::shout:
            case slot_type::power:
            case slot_type::armor:
            case slot_type::scroll:
            case slot_type::misc:
            case slot_type::lantern:
            case slot_type::mask:
                item->form = a_form;
                item->type = type;
                data.push_back(item);
                break;
            case slot_type::consumable:
                item->form = nullptr;
                item->type = type;
                item->actor_value = util::helper::get_actor_value_effect_from_potion(a_form);
                if (item->actor_value == RE::ActorValue::kNone) {
                    item->form = a_form;
                }
                data.push_back(item);
                break;
            case slot_type::weapon:
            case slot_type::magic:
            case slot_type::shield:
            case slot_type::light:
                item->form = a_form;
                item->left = a_left;
                item->type = type;
                item->action_type = handle::slot_setting::action_type::default_action;
                item->two_handed = two_handed;
                data.push_back(item);
                break;
        }

        for (const auto data_item : data) {
            write_notification(fmt::format("Name {}, Type {}, Action {}, Left {}",
                data_item->form ? data_item->form->GetName() : "null",
                static_cast<uint32_t>(data_item->type),
                static_cast<uint32_t>(data_item->action_type),
                data_item->left));
        }

        auto page_handle = handle::page_handle::get_singleton();
        auto page = page_handle->get_active_page_id();
        //for some types we need to check if there is something on the other hand
        if (type == slot_type::weapon || type == slot_type::magic || type == slot_type::shield ||
            type == slot_type::light) {
            if (!two_handed) {
                auto slot_settings = page_handle->get_page_setting(page, a_position_type)->slot_settings;

                std::vector<data_helper*> current_data;
                const auto item_current = new data_helper();
                item_current->form = nullptr;
                item_current->left = false;
                item_current->type = slot_type::empty;
                item_current->action_type = handle::slot_setting::action_type::default_action;
                current_data.push_back(item_current);

                const auto item2_current = new data_helper();
                item2_current->form = nullptr;
                item2_current->left = true;
                item2_current->type = slot_type::empty;
                item2_current->action_type = handle::slot_setting::action_type::default_action;
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
                    item2->type = slot_type::weapon;
                    item2->action_type = handle::slot_setting::action_type::default_action;
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
        processing::set_setting_data::set_single_slot(page, a_position_type, data);
    }

    uint32_t game_menu_setting::get_selected_form(RE::UI*& a_ui) {
        uint32_t menu_form = 0;
        if (a_ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME)) {
            auto inventory_menu = static_cast<RE::InventoryMenu*>(a_ui->GetMenu(RE::InventoryMenu::MENU_NAME).get());
            if (inventory_menu) {
                RE::GFxValue result;
                //inventory_menu->uiMovie->SetPause(true);
                inventory_menu->uiMovie->GetVariable(&result,
                    "_root.Menu_mc.inventoryLists.itemList.selectedEntry.formId");
                if (result.GetType() == RE::GFxValue::ValueType::kNumber) {
                    menu_form = static_cast<std::uint32_t>(result.GetNumber());
                    logger::trace("formid {}"sv, util::string_util::int_to_hex(menu_form));
                }
            }
        }

        if (a_ui->IsMenuOpen(RE::MagicMenu::MENU_NAME)) {
            auto magic_menu = static_cast<RE::MagicMenu*>(a_ui->GetMenu(RE::MagicMenu::MENU_NAME).get());
            if (magic_menu) {
                RE::GFxValue result;
                magic_menu->uiMovie->GetVariable(&result, "_root.Menu_mc.inventoryLists.itemList.selectedEntry.formId");
                if (result.GetType() == RE::GFxValue::ValueType::kNumber) {
                    menu_form = static_cast<std::uint32_t>(result.GetNumber());
                    logger::trace("formid {}"sv, util::string_util::int_to_hex(menu_form));
                }
            }
        }

        if (a_ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME)) {
            auto favorite_menu = static_cast<RE::FavoritesMenu*>(a_ui->GetMenu(RE::FavoritesMenu::MENU_NAME).get());
            if (favorite_menu) {
                RE::GFxValue result;
                favorite_menu->uiMovie->GetVariable(&result, "_root.MenuHolder.Menu_mc.itemList.selectedEntry.formId");
                if (result.GetType() == RE::GFxValue::ValueType::kNumber) {
                    menu_form = static_cast<std::uint32_t>(result.GetNumber());
                    logger::trace("formid {}"sv, util::string_util::int_to_hex(menu_form));
                }
            }
        }

        return menu_form;
    }

    bool game_menu_setting::is_need_menu_open(RE::UI*& a_ui) {
        return a_ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) || a_ui->IsMenuOpen(RE::MagicMenu::MENU_NAME) ||
               a_ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME);
    }

    data_helper* game_menu_setting::is_suitable_for_position(RE::TESForm*& a_form,
        const handle::position_setting::position_type a_position) {
        //all kind of weapons and magic/spells
        const auto item = new data_helper();
        const auto type = util::helper::get_type(a_form);
        const auto two_handed = util::helper::is_two_handed(a_form);
        logger::trace("Item {}, is Type {}, TwoHanded {}"sv,
            a_form ? util::string_util::int_to_hex(a_form->formID) : "null",
            static_cast<uint32_t>(type),
            two_handed);

        switch (a_position) {
            case position_type::top:
                switch (type) {
                    case slot_type::power:
                    case slot_type::shout:
                        //case slot_type::misc:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        item->action_type = util::helper::can_instant_cast(a_form, type) ?
                                                handle::slot_setting::action_type::instant :
                                                handle::slot_setting::action_type::default_action;
                        break;
                    case slot_type::magic:
                        if (util::helper::can_instant_cast(a_form, type)) {
                            item->form = a_form;
                            item->type = type;
                            item->two_handed = two_handed;
                            item->left = false;
                            item->action_type = handle::slot_setting::action_type::instant;
                        }
                        break;
                }
                break;
            case position_type::right:
                switch (type) {
                    case slot_type::weapon:
                    case slot_type::magic:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        break;
                }
                break;
            case position_type::bottom:
                switch (type) {
                    case slot_type::consumable:
                        item->form = nullptr;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        item->actor_value = util::helper::get_actor_value_effect_from_potion(a_form);
                        if (item->actor_value == RE::ActorValue::kNone) {
                            item->form = a_form;
                        }
                        break;
                    case slot_type::lantern:  //not sure if best here
                    case slot_type::mask:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        break;
                    case slot_type::scroll:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        item->action_type = handle::slot_setting::action_type::instant;
                        break;
                }
                break;
            case position_type::left:
                switch (type) {
                    case slot_type::weapon:
                    case slot_type::magic:
                    case slot_type::shield:
                    case slot_type::light:
                        if (!two_handed) {
                            item->form = a_form;
                            item->type = type;
                            item->two_handed = two_handed;
                            item->left = true;
                            break;
                        }
                        break;
                }
                break;
            case position_type::total:
                break;
        }

        return item;
    }

    void game_menu_setting::write_notification(const std::string& a_string) { RE::DebugNotification(a_string.c_str()); }

}
