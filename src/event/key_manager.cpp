#include "key_manager.h"
#include "handle/setting_execute.h"
#include "setting/mcm_setting.h"

namespace event {
    using event_result = RE::BSEventNotifyControl;

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
        using event_type = RE::INPUT_EVENT_TYPE;
        using device_type = RE::INPUT_DEVICE;


        key_top_action_ = config::mcm_setting::get_top_action_key();
        key_right_action_ = config::mcm_setting::get_right_action_key();
        key_bottom_action_ = config::mcm_setting::get_bottom_action_key();
        key_left_action_ = config::mcm_setting::get_left_action_key();

        button_press_modify_ = config::mcm_setting::get_slot_button_feedback();


        if (!is_key_valid(key_top_action_) || !is_key_valid(key_right_action_) || !is_key_valid(key_bottom_action_) || !
            is_key_valid(key_left_action_)) {
            return event_result::kContinue;
        }

        if (!a_event) {
            return event_result::kContinue;
        }

        const auto ui = RE::UI::GetSingleton();
        const auto interface_strings = RE::InterfaceStrings::GetSingleton();

        if (ui->IsMenuOpen(interface_strings->console)) {
            return event_result::kContinue;
        }

        for (auto event = *a_event; event; event = event->next) {
            if (event->eventType != event_type::kButton) {
                continue;
            }

            //this stays static_cast
            const auto button = static_cast<RE::ButtonEvent*>(event);
            // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)


            key_ = button->idCode;
            if (key_ == k_invalid) {
                continue;
            }

            switch (button->device.get()) {
                // NOLINT(clang-diagnostic-switch-enum)
                case device_type::kMouse:
                    key_ += k_mouse_offset;
                    break;
                case device_type::kKeyboard:
                    key_ += k_keyboard_offset;
                    break;
                case device_type::kGamepad:
                    key_ = get_gamepad_index(static_cast<RE::BSWin32GamepadDevice::Key>(key_));
                    break;
                default:
                    continue;
            }

            if (const auto control_map = RE::ControlMap::GetSingleton(); !control_map->IsMovementControlsEnabled()) {
                continue;
            }

            /*if the game is not paused with the menu, it triggers the menu always in the background*/
            if (ui->GameIsPaused()) {
                continue;
            }

            //init fade setting here
            //currently not used, not sure this is te correct place
            /*if (button->IsDown() && (key_ == key_top_action_ || key_ == key_right_action_ || key_ == key_bottom_action_
                                     || key_ ==
                                     key_left_action_)) {
                logger::debug("configured Key ({}) is down"sv, key_);
                const auto page_setting = handle::setting_execute::get_page_setting_for_key(key_);
                if (page_setting == nullptr) {
                    logger::trace("setting for key {} is null. return."sv, key_);
                    break;
                }

                logger::trace("trying to set fade setting for position {} ... "sv,
                    static_cast<uint32_t>(page_setting->pos));
                page_setting->fade_setting->action = handle::fade_setting::action::out;
                page_setting->fade_setting->alpha = handle::fade_setting::alpha::min;
                page_setting->fade_setting->current_alpha = static_cast<uint32_t>(handle::fade_setting::alpha::max);
                logger::trace("done settinging fade for position {}"sv, static_cast<uint32_t>(page_setting->pos));
            }*/

            if (button->IsDown() && (key_ == key_top_action_ || key_ == key_right_action_ || key_ == key_bottom_action_
                                     || key_ == key_left_action_)) {
                logger::debug("configured Key ({}) is down"sv, key_);
                //set slot to a different color
                const auto page_setting = handle::setting_execute::get_page_setting_for_key(key_);
                if (page_setting == nullptr) {
                    logger::trace("setting for key {} is null. return."sv, key_);
                    break;
                }
                page_setting->button_press_modify = button_press_modify_;
            }

            if (button->IsUp() && (key_ == key_top_action_ || key_ == key_right_action_ || key_ == key_bottom_action_ ||
                                   key_ == key_left_action_)) {
                logger::debug("configured Key ({}) is up"sv, key_);
                //set slot back to normal color
                const auto page_setting = handle::setting_execute::get_page_setting_for_key(key_);
                if (page_setting == nullptr) {
                    logger::trace("setting for key {} is null. return."sv, key_);
                    break;
                }
                page_setting->button_press_modify = ui::draw_full;
            }


            if (!button->IsDown()) {
                continue;
            }

            if (key_ == key_top_action_ || key_ == key_right_action_ || key_ == key_bottom_action_ || key_ ==
                key_left_action_) {
                logger::debug("configured Key ({}) pressed"sv, key_);
                const auto page_setting = handle::setting_execute::get_page_setting_for_key(key_);
                if (page_setting == nullptr) {
                    logger::trace("setting for key {} is null. return."sv, key_);
                    break;
                }
                handle::setting_execute::execute_settings(page_setting->slot_settings);

                break;
            }
        }
        return event_result::kContinue;
    }

    uint32_t key_manager::get_gamepad_index(const RE::BSWin32GamepadDevice::Key a_key) {
        using key = RE::BSWin32GamepadDevice::Key;

        uint32_t index;
        switch (a_key) {
            case key::kUp:
                index = 0;
                break;
            case key::kDown:
                index = 1;
                break;
            case key::kLeft:
                index = 2;
                break;
            case key::kRight:
                index = 3;
                break;
            case key::kStart:
                index = 4;
                break;
            case key::kBack:
                index = 5;
                break;
            case key::kLeftThumb:
                index = 6;
                break;
            case key::kRightThumb:
                index = 7;
                break;
            case key::kLeftShoulder:
                index = 8;
                break;
            case key::kRightShoulder:
                index = 9;
                break;
            case key::kA:
                index = 10;
                break;
            case key::kB:
                index = 11;
                break;
            case key::kX:
                index = 12;
                break;
            case key::kY:
                index = 13;
                break;
            case key::kLeftTrigger:
                index = 14;
                break;
            case key::kRightTrigger:
                index = 15;
                break;
            default: // NOLINT(clang-diagnostic-covered-switch-default)
                index = k_invalid;
                break;
        }

        return index != k_invalid ? index + k_gamepad_offset : k_invalid;
    }

    bool key_manager::is_key_valid(const uint32_t a_key) {
        if (a_key == k_invalid) {
            return false;
        }
        return true;
    }
}
