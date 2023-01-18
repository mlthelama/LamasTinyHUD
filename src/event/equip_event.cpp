#include "equip_event.h"
#include "handle/edit_handle.h"
#include "handle/name_handle.h"
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

        if (handle::edit_handle::get_singleton()->get_position() == handle::page_setting::position::total) {
            return event_result::kContinue;
        }

        auto form = RE::TESForm::LookupByID(a_event->baseObject);

        if (!form) {
            return event_result::kContinue;
        }


        if (const auto edit_handle = handle::edit_handle::get_singleton();
            edit_handle->get_position() != handle::page_setting::position::total &&
            config::mcm_setting::get_elder_demon_souls() && a_event->equipped) {
            const auto item = util::helper::is_suitable_for_position(form, edit_handle->get_position());
            if (item->form) {
                data_.push_back(item);
                util::helper::write_notification(fmt::format("Added Item {}", form ? form->GetName() : "null"));
            }

            //check how many we already have
            if (data_.size() == config::mcm_setting::get_max_page_count()) {
                edit_handle->set_hold_data(data_);
                util::helper::write_notification(fmt::format("Max Amount of {} Reached, rest will be Ignored",
                    config::mcm_setting::get_max_page_count()));
            }
            if (data_.size() > config::mcm_setting::get_max_page_count()) {
                util::helper::write_notification(fmt::format("Ignored Item {}", form ? form->GetName() : "null"));
            }
            edit_handle->set_hold_data(data_);
            logger::trace("Size is {}"sv, data_.size());
        }

        //edit for elder demon souls
        //right and left just weapons, left only one handed, right both
        //buttom consumables, scrolls, and such
        //top shouts, powers
        if (const auto edit_handle = handle::edit_handle::get_singleton();
            edit_handle->get_position() != handle::page_setting::position::total && !
            config::mcm_setting::get_elder_demon_souls()) {
            data_.clear();
            logger::trace("Player {} {}"sv, a_event->equipped ? "equipped" : "unequipped", form->GetName());
            //always
            const auto type = util::helper::get_type(form);
            if (type == handle::slot_setting::slot_type::unset || type ==
                handle::slot_setting::slot_type::weapon || type ==
                handle::slot_setting::slot_type::magic || type == handle::slot_setting::slot_type::shield) {
                data_ = util::helper::get_hand_assignment(form);
            }
            //just if equipped
            if (a_event->equipped) {
                const auto item = new data_helper();
                //magic, weapon, shield handled outside
                switch (type) {
                    case handle::slot_setting::slot_type::unset:
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
