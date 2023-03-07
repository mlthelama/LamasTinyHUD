#pragma once
#include "control/binding.h"
#include "control/common.h"
#include "handle/data/page/position_setting.h"
#include "ui/image_path.h"

namespace event {
    class key_manager final : public RE::BSTEventSink<RE::InputEvent*> {
        using event_result = RE::BSEventNotifyControl;

    public:
        static key_manager* get_singleton();
        static void sink();

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

        uint32_t key_ = control::common::k_invalid;

        uint32_t button_press_modify_ = ui::draw_full;

        bool is_toggle_down_ = false;

        void do_button_press(uint32_t a_key, control::binding*& a_binding) const;
        [[nodiscard]] static bool scroll_position(uint32_t a_key, control::binding*& a_binding);
        void do_button_down(handle::position_setting*& a_position_setting) const;
    };
}
