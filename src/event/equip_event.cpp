#include "equip_event.h"
#include "handle/handle/edit_handle.h"
#include "handle/handle/name_handle.h"
#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"
#include "util/helper.h"
#include "util/string_util.h"

namespace event {
    equip_event* equip_event::get_singleton() {
        static equip_event singleton;
        return std::addressof(singleton);
    }

    void equip_event::sink() {
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(get_singleton());
    }

    equip_event::event_result equip_event::ProcessEvent(const RE::TESEquipEvent* a_event,
        [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>* a_event_source) {
        if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef()) {
            return event_result::kContinue;
        }

        if (config::mcm_setting::get_draw_current_items_text()) {
            handle::name_handle::get_singleton()->init_names(util::helper::get_hand_assignment());
        }

        if (handle::edit_handle::get_singleton()->get_position() == handle::position_setting::position_type::total) {
            return event_result::kContinue;
        }

        auto form = RE::TESForm::LookupByID(a_event->baseObject);

        if (!form) {
            return event_result::kContinue;
        }

        if (const auto edit_handle = handle::edit_handle::get_singleton();
            edit_handle->get_position() != handle::position_setting::position_type::total) {
            data_.clear();
            logger::trace("Player {} {}"sv, a_event->equipped ? "equipped" : "unequipped", form->GetName());
            //always
            const auto type = util::helper::get_type(form);
            if (type == handle::slot_setting::slot_type::empty || type ==
                handle::slot_setting::slot_type::weapon || type ==
                handle::slot_setting::slot_type::magic || type == handle::slot_setting::slot_type::shield) {
                data_ = util::helper::get_hand_assignment(form);
            }
            //just if equipped
            if (a_event->equipped) {
                const auto item = new data_helper();
                //magic, weapon, shield handled outside
                switch (type) {
                    case handle::slot_setting::slot_type::empty:
                        item->form = nullptr;
                        item->type = type;
                        data_.push_back(item);
                        break;
                    case handle::slot_setting::slot_type::shout:
                    case handle::slot_setting::slot_type::power:
                    case handle::slot_setting::slot_type::consumable:
                    case handle::slot_setting::slot_type::armor:
                    case handle::slot_setting::slot_type::scroll:
                    case handle::slot_setting::slot_type::misc:
                        item->form = form;
                        item->type = type;
                        data_.push_back(item);
                        break;
                }

                if (type == handle::slot_setting::slot_type::consumable) {
                    const auto obj = form->As<RE::AlchemyItem>();
                    RE::PlayerCharacter::GetSingleton()->AddObjectToContainer(obj, nullptr, 1, nullptr);
                }
            }
            edit_handle->set_hold_data(data_);
            data_.clear();
        }

        return event_result::kContinue;
    }
}
