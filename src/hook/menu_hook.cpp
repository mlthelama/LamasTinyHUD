#include "menu_hook.h"
#include "handle/key_position_handle.h"
#include "processing/game_menu_setting.h"
#include "setting/mcm_setting.h"

namespace hook {

    void menu_hook::install() {
        logger::info("Hooking ..."sv);


        REL::Relocation<std::uintptr_t> menu_controls_vtbl{ RE::VTABLE_MenuControls[0] };
        process_event_ = menu_controls_vtbl.write_vfunc(0x1, &menu_hook::process_event);

        logger::info("Hooked."sv);
    }

    RE::BSEventNotifyControl menu_hook::process_event(RE::InputEvent** a_event,
        RE::BSTEventSource<RE::InputEvent*>* a_source) {
        auto ui = RE::UI::GetSingleton();
        auto binding = control::binding::get_singleton();
        auto edit_key = config::mcm_setting::get_key_press_to_enter_edit();
        auto user_event = RE::UserEvents::GetSingleton();
        auto control_map = RE::ControlMap::GetSingleton();

        if (a_event && *a_event && processing::game_menu_setting::is_need_menu_open(ui)) {
            for (auto event = *a_event; event; event = event->next) {
                if (event->HasIDCode()) {
                    auto button = static_cast<RE::ButtonEvent*>(event);

                    key_ = button->idCode;
                    if (key_ == control::common::k_invalid) {
                        continue;
                    }

                    control::common::get_key_id(button, key_);

                    if (button->IsUp()) {
                        if (binding->get_is_edit_down() &&
                            ((edit_key && control::common::is_key_valid_and_matches(key_, binding->get_edit_key())) ||
                                control::common::is_key_valid_and_matches(key_,
                                    binding->get_bottom_execute_or_toggle_action()))) {
                            binding->set_is_edit_down(false);
                        }

                        if (binding->get_is_edit_left_down() && control::common::is_key_valid_and_matches(key_,
                                                                    binding->get_edit_key_left_or_overwrite())) {
                            binding->set_is_edit_left_down(false);
                        }
                    }

                    if (button->IsDown()) {
                        if (control::common::is_key_valid_and_matches(key_,
                                binding->get_bottom_execute_or_toggle_action()) ||
                            (config::mcm_setting::get_key_press_to_enter_edit() &&
                                control::common::is_key_valid_and_matches(key_, binding->get_edit_key()))) {
                            binding->set_is_edit_down(true);
                        }
                        if (control::common::is_key_valid_and_matches(key_,
                                binding->get_edit_key_left_or_overwrite())) {
                            binding->set_is_edit_left_down(true);
                        }
                    }

                    if (need_to_overwrite(button, user_event, control_map) &&
                        (binding->get_is_edit_down() || binding->get_is_edit_left_down())) {
                        button->idCode = control::common::k_invalid;
                        button->userEvent = "";
                    }

                    if (!button->IsDown()) {
                        continue;
                    }

                    if (!binding->get_is_edit_down() && !binding->get_is_edit_left_down()) {
                        continue;
                    }

                    if (button->IsPressed() && binding->is_position_button(key_)) {
                        auto menu_form = processing::game_menu_setting::get_selected_form(ui);
                        if (menu_form) {
                            auto tes_form_menu = RE::TESForm::LookupByID(menu_form);
                            auto key_position =
                                handle::key_position_handle::get_singleton()->get_position_for_key(key_);
                            if (config::mcm_setting::get_elden_demon_souls()) {
                                processing::game_menu_setting::elden_souls_config(tes_form_menu,
                                    key_position,
                                    binding->get_is_edit_left_down());
                            } else {
                                processing::game_menu_setting::default_config(tes_form_menu,
                                    key_position,
                                    binding->get_is_edit_left_down());
                            }
                        }
                    }
                }
            }
        }
        return process_event_(this, a_event, a_source);
    }

    bool menu_hook::need_to_overwrite(RE::ButtonEvent*& a_button,
        RE::UserEvents*& a_user_event,
        RE::ControlMap*& a_control_map) const {
        auto button_event = a_button->userEvent;
        if (button_event == a_user_event->up || button_event == a_user_event->right ||
            button_event == a_user_event->down || button_event == a_user_event->left ||
            button_event == a_user_event->strafeRight || button_event == a_user_event->strafeLeft ||
            button_event == a_user_event->forward || button_event == a_user_event->back ||
            button_event == a_user_event->pageUp || button_event == a_user_event->nextPage ||
            button_event == a_user_event->pageDown || button_event == a_user_event->prevPage) {
            return true;
        }

        auto device = a_button->device.get();
        if (key_ == a_control_map->GetMappedKey(a_user_event->up, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->right, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->down, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->left, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->strafeRight, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->strafeLeft, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->forward, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->back, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->pageUp, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->nextPage, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->pageDown, device) ||
            key_ == a_control_map->GetMappedKey(a_user_event->prevPage, device)) {
            return true;
        }
        return false;
    }
}
