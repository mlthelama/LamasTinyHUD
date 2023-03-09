#pragma once
#include "handle/data/data_helper.h"
#include "handle/data/page/position_setting.h"

namespace event {
    class equip_event final : public RE::BSTEventSink<RE::TESEquipEvent> {
    public:
        using event_result = RE::BSEventNotifyControl;

        static equip_event* get_singleton();
        static void sink();

        equip_event(const equip_event&) = delete;
        equip_event(equip_event&&) = delete;

        equip_event& operator=(const equip_event&) = delete;
        equip_event& operator=(equip_event&&) = delete;

    protected:
        event_result ProcessEvent(const RE::TESEquipEvent* a_event,
            [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>* a_event_source) override;

    private:
        equip_event() = default;
        ~equip_event() override = default;
    };
}
