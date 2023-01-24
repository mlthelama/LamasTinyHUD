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

        void work_default(RE::TESForm*& a_form, bool a_equipped);
        void work_elden_demon_souls(RE::TESForm*& a_form, bool a_equipped);
        static void look_for_ammo(bool a_crossbow);
        static void check_if_location_needs_block(RE::TESForm*& a_form, bool a_equipped);
    };
}
