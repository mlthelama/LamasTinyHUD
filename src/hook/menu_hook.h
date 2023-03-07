#pragma once
#include "control/binding.h"
#include "control/common.h"

namespace hook {
    class menu_hook : public RE::MenuControls {
    public:
        static void install();

    private:
        RE::BSEventNotifyControl process_event(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_source);

        using process_event_type =
            decltype(static_cast<RE::BSEventNotifyControl (RE::MenuControls::*)(RE::InputEvent* const*,
                    RE::BSTEventSource<RE::InputEvent*>*)>(&RE::MenuControls::ProcessEvent));
        static inline REL::Relocation<process_event_type> process_event_;

        uint32_t key_ = control::common::k_invalid;
        
        bool need_to_overwrite(RE::ButtonEvent*& a_button, RE::UserEvents*& a_user_event, RE::ControlMap*& a_control_map) const;
    };
}
