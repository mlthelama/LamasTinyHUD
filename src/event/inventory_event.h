#pragma once

namespace event {
    class inventory_event final : public RE::BSTEventSink<RE::Inventory::Event> {
    public:
        using event_result = RE::BSEventNotifyControl;

        [[nodiscard]] static inventory_event* get_singleton();

        static void sink();

        inventory_event(const inventory_event&) = delete;
        inventory_event(inventory_event&&) = delete;

        inventory_event& operator=(const inventory_event&) = delete;
        inventory_event& operator=(inventory_event&&) = delete;

    protected:
        event_result ProcessEvent(const RE::Inventory::Event* a_event,
            RE::BSTEventSource<RE::Inventory::Event>*) override;

    private:
        inventory_event() = default;
        ~inventory_event() override = default;
    };
}
