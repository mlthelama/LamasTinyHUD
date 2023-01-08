#include "inventory_event.h"

#include "handle/page_handle.h"

namespace event {
    inventory_event* inventory_event::get_singleton() {
        static inventory_event singleton;
        return std::addressof(singleton);
    }

    void inventory_event::sink() {
        if (const auto event = RE::Inventory::GetEventSource(); event) {
            event->AddEventSink<RE::Inventory::Event>(get_singleton());
            logger::info("Registered {} handler"sv, typeid(RE::Inventory::Event).name());
        }
    }

    inventory_event::event_result inventory_event::ProcessEvent(const RE::Inventory::Event* a_event,
        RE::BSTEventSource<RE::Inventory::Event>*) {
        if (!a_event) {
            return event_result::kContinue;
        }

        if (!a_event->objRefr->IsPlayer() && !a_event->objRefr->IsPlayerRef()) {
            return event_result::kContinue;
        }

        //TODO i might use something different, as far as testing goes, removing it does not notify
        //TODO use something else
        //those should include food and potions
        if (a_event->entryData && a_event->entryData->object->IsMagicItem()) {
            const auto page_handle = handle::page_handle::get_singleton();
            for (auto pages = page_handle->get_page(); auto [position, page] : pages) {
                for (const auto setting : page->slot_settings) {
                    if (setting->type == util::selection_type::item && setting->form->formID == a_event->entryData->
                        object->GetFormID()) {
                        const auto diff = a_event->newCount - a_event->prevCount;
                        setting->item_count = setting->item_count + diff;
                        logger::trace("Name {}, old {}, new {}"sv,
                            a_event->entryData->GetDisplayName(),
                            a_event->prevCount,
                            a_event->newCount);
                    }
                }
            }
        }

        return event_result::kContinue;
    }
}
