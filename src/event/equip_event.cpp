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

        const auto ui = RE::UI::GetSingleton();
        if (!ui || (ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) || ui->IsMenuOpen(RE::MagicMenu::MENU_NAME) ||
                       ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME))) {
            return event_result::kContinue;
        }

        auto form = RE::TESForm::LookupByID(a_event->baseObject);

        if (!form) {
            return event_result::kContinue;
        }

        if (config::mcm_setting::get_draw_current_items_text() &&
            (form->IsWeapon() || form->Is(RE::FormType::Spell) || form->IsAmmo() || form->Is(RE::FormType::Light))) {
            handle::name_handle::get_singleton()->init_names(util::player::get_hand_assignment());
        }

        //add check if we need to block left
        if (!ui->GameIsPaused() && config::mcm_setting::get_elden_demon_souls() && util::helper::is_two_handed(form)) {
            processing::set_setting_data::check_if_location_needs_block(form, a_event->equipped);
        }

        return event_result::kContinue;
    }
}
