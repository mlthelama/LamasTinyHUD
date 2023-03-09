#pragma once

namespace event {
    class menu_manager final : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
    public:
        using event_result = RE::BSEventNotifyControl;

        static menu_manager* get_singleton();
        static void sink();

        menu_manager(const menu_manager&) = delete;
        menu_manager(menu_manager&&) = delete;

        menu_manager& operator=(const menu_manager&) = delete;
        menu_manager& operator=(menu_manager&&) = delete;

    protected:
        event_result ProcessEvent(const RE::MenuOpenCloseEvent* a_event,
            RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_event_source) override;

    private:
        menu_manager() = default;
        ~menu_manager() override = default;
    };
}  // event
