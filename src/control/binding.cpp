#include "binding.h"
#include "control/common.h"
#include "setting/mcm_setting.h"

namespace control {

    binding* binding::get_singleton() {
        static binding singleton;
        return std::addressof(singleton);
    }

    binding::binding() = default;

    uint32_t binding::get_top_action() const { return key_top_action_; }
    uint32_t binding::get_right_action() const { return key_right_action_; }
    uint32_t binding::get_bottom_action() const { return key_bottom_action_; }
    uint32_t binding::get_left_action() const { return key_left_action_; }
    uint32_t binding::get_bottom_execute_or_toggle_action() const { return key_bottom_execute_or_toggle_; }
    uint32_t binding::get_top_execute() const { return key_top_execute_; }
    uint32_t binding::get_hide_show() const { return key_hide_show_; }
    uint32_t binding::get_edit_key() const { return key_edit_key_; }
    uint32_t binding::get_edit_key_left_or_overwrite() const { return key_edit_left_or_overwrite_; }
    uint32_t binding::get_remove_key() const { return key_remove_key_; }

    void binding::set_all_keys() {
        key_top_action_ = config::mcm_setting::get_top_action_key();
        key_right_action_ = config::mcm_setting::get_right_action_key();
        key_bottom_action_ = config::mcm_setting::get_bottom_action_key();
        key_left_action_ = config::mcm_setting::get_left_action_key();
        key_bottom_execute_or_toggle_ = config::mcm_setting::get_toggle_key();
        key_hide_show_ = config::mcm_setting::get_show_hide_key();
        key_edit_key_ = config::mcm_setting::get_edit_key();
        key_edit_left_or_overwrite_ = config::mcm_setting::get_left_or_overwrite_edit_key();
        key_remove_key_ = config::mcm_setting::get_remove_key();
    }

    void binding::set_top_execute(uint32_t a_key) { key_top_execute_ = a_key; }

    bool binding::is_position_button(uint32_t a_key) const {
        if (a_key == key_top_action_ || a_key == key_right_action_ || a_key == key_bottom_action_ ||
            a_key == key_left_action_) {
            return true;
        }
        return false;
    }

    bool binding::are_main_key_valid() const {
        if (!control::common::is_key_valid(key_top_action_) || !control::common::is_key_valid(key_right_action_) ||
            !control::common::is_key_valid(key_bottom_action_) || !control::common::is_key_valid(key_left_action_) ||
            !control::common::is_key_valid(key_bottom_execute_or_toggle_)) {
            return false;
        }
        return true;
    }

    bool binding::get_is_edit_down() const { return is_edit_down_; }
    void binding::set_is_edit_down(bool a_down) {
        logger::trace("setting toggle down to {}", a_down);
        is_edit_down_ = a_down;
    }

    bool binding::get_is_edit_left_down() const { return is_edit_left_down_; }
    void binding::set_is_edit_left_down(bool a_down) {
        logger::trace("setting left down to {}", a_down);
        is_edit_left_down_ = a_down;
    }

    bool binding::get_is_remove_down() const { return is_remove_down_; }
    void binding::set_is_remove_down(bool a_down) {
        logger::trace("setting remove down to {}", a_down);
        is_remove_down_ = a_down;
    }
}  // control
