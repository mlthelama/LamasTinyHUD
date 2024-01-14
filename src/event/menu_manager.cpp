#include "menu_manager.h"
#include "control/binding.h"
#include "handle/extra_data_holder.h"

namespace event {
    menu_manager* menu_manager::get_singleton() {
        static menu_manager singleton;
        return std::addressof(singleton);
    }

    void menu_manager::sink() { RE::UI::GetSingleton()->AddEventSink(get_singleton()); }
    menu_manager::event_result menu_manager::ProcessEvent(const RE::MenuOpenCloseEvent* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_event_source) {
        if (!a_event) {
            return event_result::kContinue;
        }

        if (!a_event->opening &&
            (a_event->menuName == RE::InventoryMenu::MENU_NAME || a_event->menuName == RE::MagicMenu::MENU_NAME ||
                a_event->menuName == RE::FavoritesMenu::MENU_NAME)) {
            auto* binding = control::binding::get_singleton();
            if (binding->get_is_edit_down()) {
                binding->set_is_edit_down(false);
            }
            if (binding->get_is_edit_left_down()) {
                binding->set_is_edit_left_down(false);
            }
            if (binding->get_is_remove_down()) {
                binding->set_is_remove_down(false);
            }
        }
        
        if (a_event->menuName == RE::InventoryMenu::MENU_NAME || a_event->menuName == RE::MagicMenu::MENU_NAME ||
            a_event->menuName == RE::FavoritesMenu::MENU_NAME || a_event->menuName == RE::BarterMenu::MENU_NAME ||
            a_event->menuName == RE::GiftMenu::MENU_NAME || a_event->menuName == RE::BookMenu::MENU_NAME ||
            a_event->menuName == RE::FaderMenu::MENU_NAME || a_event->menuName == RE::CraftingMenu::MENU_NAME) {
            handle::extra_data_holder::get_singleton()->reset_data();
        }
        
        return event_result::kContinue;
    }
}  // event
