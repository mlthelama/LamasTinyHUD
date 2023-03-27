#include "key_manager.h"
#include "handle/ammo_handle.h"
#include "handle/extra_data_holder.h"
#include "handle/page_handle.h"
#include "processing/game_menu_setting.h"
#include "processing/setting_execute.h"
#include "setting/mcm_setting.h"
#include "ui/ui_renderer.h"

namespace event {
    using event_result = RE::BSEventNotifyControl;
    using position_type = handle::position_setting::position_type;
    using common = control::common;
    using mcm = config::mcm_setting;
    using setting_execute = processing::setting_execute;

    key_manager* key_manager::get_singleton() {
        static key_manager singleton;
        return std::addressof(singleton);
    }

    void key_manager::sink() {
        RE::BSInputDeviceManager::GetSingleton()->AddEventSink(get_singleton());
        logger::info("start listening for input events."sv);
    }

    event_result key_manager::ProcessEvent(RE::InputEvent* const* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_event_source) {
        button_press_modify_ = mcm::get_slot_button_feedback();
        auto key_binding = control::binding::get_singleton();

        //top execute btn is bound to the shout key, no need to check here
        if (!key_binding->are_main_key_valid()) {
            return event_result::kContinue;
        }

        if (!a_event) {
            return event_result::kContinue;
        }

        auto ui = RE::UI::GetSingleton();
        if (!ui) {
            return event_result::kContinue;
        }

        const auto interface_strings = RE::InterfaceStrings::GetSingleton();
        if (ui->IsMenuOpen(interface_strings->console)) {
            return event_result::kContinue;
        }

        if (processing::game_menu_setting::is_need_menu_open(ui)) {
            return event_result::kContinue;
        }

        handle::extra_data_holder::get_singleton()->reset_data();

        for (auto event = *a_event; event; event = event->next) {
            if (event->eventType != RE::INPUT_EVENT_TYPE::kButton) {
                continue;
            }

            //this stays static_cast
            const auto button =
                static_cast<RE::ButtonEvent*>(event);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            key_ = button->idCode;
            if (key_ == common::k_invalid) {
                continue;
            }

            common::get_key_id(button, key_);

            if (const auto control_map = RE::ControlMap::GetSingleton(); !control_map->IsMovementControlsEnabled()) {
                continue;
            }

            /*if the game is not paused with the menu, it triggers the menu always in the background*/
            if (ui->GameIsPaused() || !ui->IsCursorHiddenWhenTopmost() || !ui->IsShowingMenus() ||
                !ui->GetMenu<RE::HUDMenu>()) {
                continue;
            }

            if (RE::UI::GetSingleton()->IsMenuOpen("LootMenu") && mcm::get_disable_input_quick_loot()) {
                continue;
            }

            const auto control_map = RE::ControlMap::GetSingleton();
            if (!control_map || !control_map->IsMovementControlsEnabled() ||
                control_map->contextPriorityStack.back() != RE::UserEvents::INPUT_CONTEXT_ID::kGameplay) {
                continue;
            }

            //get shout key
            auto elden = mcm::get_elden_demon_souls();
            if (elden) {
                RE::UserEvents* user_events = RE::UserEvents::GetSingleton();
                key_binding->set_top_execute(control_map->GetMappedKey(user_events->shout, button->device.get()));
            }

            if (mcm::get_hide_outside_combat() && !ui::ui_renderer::get_fade()) {
                if ((key_binding->is_position_button(key_) ||
                        key_ == key_binding->get_bottom_execute_or_toggle_action() ||
                        (elden && key_ == key_binding->get_top_execute())) &&
                    (button->IsDown() || button->IsPressed())) {
                    ui::ui_renderer::set_fade(true, 1.f);
                }
            }

            if (button->IsDown() && key_binding->is_position_button(key_)) {
                logger::debug("configured key ({}) is down"sv, key_);
                auto position_setting = setting_execute::get_position_setting_for_key(key_);
                if (!position_setting) {
                    logger::warn("setting for key {} is null. break."sv, key_);
                    break;
                }
                do_button_down(position_setting);
            }

            if (button->IsUp() && key_binding->is_position_button(key_)) {
                logger::debug("configured Key ({}) is up"sv, key_);
                //set slot back to normal color
                const auto position_setting = setting_execute::get_position_setting_for_key(key_);
                if (!position_setting) {
                    logger::warn("setting for key {} is null. break."sv, key_);
                    break;
                }
                position_setting->button_press_modify = ui::draw_full;
                if (position_setting->position == position_type::left) {
                    if (const auto current_ammo = handle::ammo_handle::get_singleton()->get_current()) {
                        current_ammo->button_press_modify = ui::draw_full;
                    }
                }
            }

            if (elden && mcm::get_bottom_execute_key_combo_only() &&
                key_ == key_binding->get_bottom_execute_or_toggle_action() && button->IsUp() && is_toggle_down_) {
                is_toggle_down_ = false;
            }

            if (elden && mcm::get_bottom_execute_key_combo_only() &&
                key_ == key_binding->get_bottom_execute_or_toggle_action() && button->IsDown()) {
                is_toggle_down_ = true;
            }

            if (!button->IsDown()) {
                continue;
            }

            if (button->IsPressed() && common::is_key_valid_and_matches(key_, key_binding->get_hide_show())) {
                ui::ui_renderer::toggle_show_ui();
            }

            if (button->IsPressed() && !elden &&
                common::is_key_valid_and_matches(key_, key_binding->get_bottom_execute_or_toggle_action())) {
                logger::debug("configured toggle key ({}) is pressed"sv, key_);

                const auto handler = handle::page_handle::get_singleton();
                handler->set_active_page(handler->get_next_page_id());
            }

            if (elden && button->IsPressed()) {
                if ((!mcm::get_bottom_execute_key_combo_only() &&
                        common::is_key_valid_and_matches(key_, key_binding->get_bottom_execute_or_toggle_action())) ||
                    (mcm::get_bottom_execute_key_combo_only() && is_toggle_down_ &&
                        key_ == key_binding->get_bottom_action())) {
                    auto page_setting = setting_execute::get_position_setting_for_key(key_binding->get_bottom_action());
                    if (!page_setting) {
                        logger::warn("setting for key {} is null. break."sv, key_);
                        break;
                    }
                    setting_execute::execute_settings(page_setting->slot_settings);
                }
                if (common::is_key_valid_and_matches(key_, key_binding->get_top_execute())) {
                    auto page_setting = setting_execute::get_position_setting_for_key(key_binding->get_top_action());
                    if (!page_setting) {
                        logger::warn("setting for key {} is null. break."sv, key_);
                        break;
                    }
                    //only instant should need work, the default shout will be handled by the game
                    setting_execute::execute_settings(page_setting->slot_settings, false, true);
                }
            }

            if (key_binding->is_position_button(key_)) {
                if (button->IsPressed()) {
                    do_button_press(key_, key_binding);
                }
            }
        }
        return event_result::kContinue;
    }

    void key_manager::do_button_press(uint32_t a_key, control::binding*& a_binding) const {
        logger::debug("configured Key ({}) pressed"sv, a_key);
        auto position_setting = setting_execute::get_position_setting_for_key(a_key);

        if (mcm::get_elden_demon_souls()) {
            if (mcm::get_bottom_execute_key_combo_only() && is_toggle_down_ &&
                a_key == a_binding->get_bottom_action()) {
                return;
            }
            const auto key_handler = handle::key_position_handle::get_singleton();
            const auto handler = handle::page_handle::get_singleton();
            if (!key_handler->is_position_locked(position_setting->position)) {
                handler->set_active_page_position(
                    handler->get_next_non_empty_setting_for_position(position_setting->position),
                    position_setting->position);
                position_setting = setting_execute::get_position_setting_for_key(a_key);
                if (!position_setting) {
                    logger::warn("setting for key {} is null. break."sv, key_);
                    return;
                }
                position_setting->highlight_slot = true;
                if (!scroll_position(a_key, a_binding)) {
                    setting_execute::execute_settings(position_setting->slot_settings);
                } else if (position_setting->position == position_type::top) {
                    setting_execute::execute_settings(position_setting->slot_settings, true);
                }
            } else {
                logger::trace("position {} is locked, skip"sv, static_cast<uint32_t>(position_setting->position));
                //check ammo is set, might be a bow or crossbow present
                const auto ammo_handle = handle::ammo_handle::get_singleton();
                if (const auto next_ammo = ammo_handle->get_next_ammo()) {
                    setting_execute::execute_ammo(next_ammo);
                    handle::ammo_handle::get_singleton()->get_current()->highlight_slot = true;
                }
            }
        } else {
            setting_execute::execute_settings(position_setting->slot_settings);
        }
    }

    bool key_manager::scroll_position(const uint32_t a_key, control::binding*& a_binding) {
        if (a_key == a_binding->get_bottom_action() || a_key == a_binding->get_top_action()) {
            return true;
        }
        return false;
    }

    void key_manager::do_button_down(handle::position_setting*& a_position_setting) const {
        if (!a_position_setting) {
            return;
        }
        if (!handle::key_position_handle::get_singleton()->is_position_locked(a_position_setting->position)) {
            a_position_setting->button_press_modify = button_press_modify_;
        } else {
            if (a_position_setting->position == position_type::left) {
                if (const auto current_ammo = handle::ammo_handle::get_singleton()->get_current()) {
                    current_ammo->button_press_modify = button_press_modify_;
                }
            }
        }
    }
}
