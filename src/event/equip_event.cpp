#include "equip_event.h"
#include "handle/name_handle.h"
#include "processing/set_setting_data.h"
#include "setting/mcm_setting.h"
#include "util/helper.h"
#include "util/player/player.h"

namespace event {
    equip_event* equip_event::get_singleton() {
        static equip_event singleton;
        return std::addressof(singleton);
    }

    void equip_event::sink() { RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(get_singleton()); }

    equip_event::event_result equip_event::ProcessEvent(const RE::TESEquipEvent* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>* a_event_source) {
        if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef()) {
            return event_result::kContinue;
        }

        auto* form = RE::TESForm::LookupByID(a_event->baseObject);
        if (!form) {
            return event_result::kContinue;
        }

        if (config::mcm_setting::get_draw_current_items_text() &&
            (form->IsWeapon() || form->Is(RE::FormType::Spell) || form->IsAmmo() || form->Is(RE::FormType::Light))) {
            handle::name_handle::get_singleton()->init_names(util::player::get_hand_assignment());
        }

        if (config::mcm_setting::get_draw_current_shout_text() && form->Is(RE::FormType::Shout) ||
            form->Is(RE::FormType::Spell)) {
            // call function there and check selected power, spell trigger and spells as well but that is ok for now
            handle::name_handle::get_singleton()->init_voice_name(
                RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().selectedPower);
        }

        //add check if we need to block left
        if (!RE::UI::GetSingleton()->GameIsPaused() && config::mcm_setting::get_elden_demon_souls() &&
            util::helper::is_two_handed(form)) {
            processing::set_setting_data::check_if_location_needs_block(form, a_event->equipped);
        }
        
        return event_result::kContinue;
    }
}
