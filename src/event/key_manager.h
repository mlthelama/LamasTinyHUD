#pragma once
#include "handle/data/page/position_setting.h"
#include "ui/image_path.h"

namespace event {
    class key_manager final : public RE::BSTEventSink<RE::InputEvent*> {
        using event_result = RE::BSEventNotifyControl;

    public:
        static key_manager* get_singleton();
        static void sink();
        void set_config_edit(bool a_edit);

        enum : uint32_t {
            k_invalid = static_cast<uint32_t>(-1),
            k_keyboard_offset = 0,
            k_mouse_offset = 256,
            k_gamepad_offset = 266
        };

        key_manager(const key_manager&) = delete;
        key_manager(key_manager&&) = delete;

        key_manager& operator=(const key_manager&) = delete;
        key_manager& operator=(key_manager&&) = delete;

    protected:
        event_result ProcessEvent(RE::InputEvent* const* a_event,
            [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_event_source) override;

    private:
        key_manager() = default;
        ~key_manager() override = default;

        static uint32_t get_gamepad_index(RE::BSWin32GamepadDevice::Key a_key);

        static bool is_key_valid(uint32_t a_key);

        uint32_t key_ = k_invalid;
        uint32_t key_top_action_ = k_invalid;
        uint32_t key_right_action_ = k_invalid;
        uint32_t key_bottom_action_ = k_invalid;
        uint32_t key_left_action_ = k_invalid;
        uint32_t key_bottom_execute_or_toggle_ = k_invalid;
        uint32_t key_top_execute_ = k_invalid;
        uint32_t key_hide_show_ = k_invalid;
        uint32_t key_edit_key_ = k_invalid;

        uint32_t button_press_modify_ = ui::draw_full;

        bool is_toggle_down_ = false;
        bool is_edit_active_ = false;
        bool is_toggle_down_menu_ = false;

        void do_button_press(uint32_t a_key);
        [[nodiscard]] bool is_position_button(uint32_t a_key) const;
        [[nodiscard]] bool scroll_position(uint32_t a_key) const;
        void do_button_down(handle::position_setting*& a_position_setting) const;
        static uint32_t get_selected_form(RE::UI*& a_ui);
        static bool is_need_menu_open(RE::UI*& a_ui);
    };
}
