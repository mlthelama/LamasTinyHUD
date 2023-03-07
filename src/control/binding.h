#pragma once
#include "control/common.h"

namespace control {
    class binding {
    public:
        [[nodiscard]] static binding* get_singleton();

        [[nodiscard]] uint32_t get_top_action() const;
        [[nodiscard]] uint32_t get_right_action() const;
        [[nodiscard]] uint32_t get_bottom_action() const;
        [[nodiscard]] uint32_t get_left_action() const;
        [[nodiscard]] uint32_t get_bottom_execute_or_toggle_action() const;
        [[nodiscard]] uint32_t get_top_execute() const;
        [[nodiscard]] uint32_t get_hide_show() const;
        [[nodiscard]] uint32_t get_edit_key() const;
        [[nodiscard]] uint32_t get_edit_key_left_or_overwrite() const;

        void set_all_keys();  //except top_execute
        void set_top_execute(uint32_t a_key);

        [[nodiscard]] bool is_position_button(uint32_t a_key) const;
        [[nodiscard]] bool are_main_key_valid() const;

        [[nodiscard]] bool get_is_edit_down() const;
        void set_is_edit_down(bool a_down);

        [[nodiscard]] bool get_is_edit_left_down() const;
        void set_is_edit_left_down(bool a_down);


        binding(const binding&) = delete;
        binding(binding&&) = delete;
        binding& operator=(const binding&) = delete;
        binding& operator=(binding&&) = delete;

    private:
        binding();
        ~binding() = default;

        uint32_t key_top_action_ = control::common::k_invalid;
        uint32_t key_right_action_ = control::common::k_invalid;
        uint32_t key_bottom_action_ = control::common::k_invalid;
        uint32_t key_left_action_ = control::common::k_invalid;
        uint32_t key_bottom_execute_or_toggle_ = control::common::k_invalid;
        uint32_t key_top_execute_ = control::common::k_invalid;
        uint32_t key_hide_show_ = control::common::k_invalid;
        uint32_t key_edit_key_ = control::common::k_invalid;
        uint32_t key_edit_left_or_overwrite_ = control::common::k_invalid;

        bool is_edit_down_ = false;
        bool is_edit_left_down_ = false;
    };
}  // control
