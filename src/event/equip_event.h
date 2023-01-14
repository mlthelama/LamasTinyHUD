#pragma once
#include "handle/data/data_helper.h"

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

        std::vector<data_helper*> data_;

        static handle::slot_setting::slot_type get_type(RE::TESForm*& a_form);

        static bool is_two_handed(RE::TESForm*& a_form);

        static std::vector<data_helper*> get_hand_assignment(RE::TESForm*& a_form);
    };
}
