#include "common.h"

namespace control {
    void common::get_key_id(const RE::ButtonEvent* a_button, uint32_t& a_key) {
        switch (a_button->device.get()) {
            case RE::INPUT_DEVICE::kMouse:
                a_key += k_mouse_offset;
                break;
            case RE::INPUT_DEVICE::kKeyboard:
                a_key += k_keyboard_offset;
                break;
            case RE::INPUT_DEVICE::kGamepad:
                a_key = get_gamepad_index(static_cast<RE::BSWin32GamepadDevice::Key>(a_key));
                break;
            case RE::INPUT_DEVICE::kNone:
            case RE::INPUT_DEVICE::kVirtualKeyboard:
            //case RE::INPUT_DEVICE::kVRRight:
            //case RE::INPUT_DEVICE::kVRLeft:
            case RE::INPUT_DEVICE::kTotal:
                break;
        }
    }

    bool common::is_key_valid(uint32_t a_key) {
        if (a_key == k_invalid) {
            return false;
        }
        return true;
    }

    bool common::is_key_valid_and_matches(uint32_t a_key, uint32_t a_key_to_check) {
        if (is_key_valid(a_key_to_check) && a_key == a_key_to_check) {
            return true;
        }
        return false;
    }

    uint32_t common::get_gamepad_index(const RE::BSWin32GamepadDevice::Key a_key) {
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
            default:  // NOLINT(clang-diagnostic-covered-switch-default)
                index = k_invalid;
                break;
        }

        return index != k_invalid ? index + k_gamepad_offset : k_invalid;
    }
}  // control
