#include "equip_event.h"
#include "handle/edit_handle.h"
#include "setting/custom_setting.h"
#include "setting/mcm_setting.h"
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

        if (handle::edit_handle::get_singleton()->get_position() == handle::page_setting::position::total) {
            return event_result::kContinue;
        }

        auto form = RE::TESForm::LookupByID(a_event->baseObject);

        if (!form) {
            return event_result::kContinue;
        }

        //edit for elder demon souls
        //right and left just weapons, left only one handed, right both
        //buttom consumables, scrolls, and such
        //top shouts, powers
        if (const auto edit_handle = handle::edit_handle::get_singleton();
            edit_handle->get_position() != handle::page_setting::position::total) {
            data_.clear();
            logger::trace("Player {} {}"sv, a_event->equipped ? "equipped" : "unequipped", form->GetName());
            //always
            const auto type = get_type(form);
            if (type == handle::slot_setting::slot_type::unset || type ==
                handle::slot_setting::slot_type::weapon || type ==
                handle::slot_setting::slot_type::magic || type == handle::slot_setting::slot_type::shield) {
                data_ = get_hand_assignment(form);
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

    handle::slot_setting::slot_type equip_event::get_type(RE::TESForm*& a_form) {
        if (a_form->IsWeapon()) {
            if (const auto weapon = a_form->As<RE::TESObjectWEAP>(); !weapon->IsBound()) {
                return handle::slot_setting::slot_type::weapon;
            }
        }
        if (a_form->IsArmor()) {
            if (const auto armor = a_form->As<RE::TESObjectARMO>(); armor->IsShield()) {
                return handle::slot_setting::slot_type::shield;
            }
            return handle::slot_setting::slot_type::armor;
        }
        if (a_form->Is(RE::FormType::Spell)) {
            const auto spell_type = a_form->As<RE::SpellItem>()->GetSpellType();
            if (spell_type == RE::MagicSystem::SpellType::kSpell || spell_type ==
                RE::MagicSystem::SpellType::kLeveledSpell) {
                return handle::slot_setting::slot_type::magic;
            }
            if (spell_type == RE::MagicSystem::SpellType::kLesserPower || spell_type ==
                RE::MagicSystem::SpellType::kPower) {
                return handle::slot_setting::slot_type::power;
            }
        }
        if (a_form->Is(RE::FormType::Shout)) {
            return handle::slot_setting::slot_type::shout;
        }
        if (a_form->Is(RE::FormType::AlchemyItem)) {
            return handle::slot_setting::slot_type::consumable;
        }
        if (a_form->Is(RE::FormType::Scroll)) {
            return handle::slot_setting::slot_type::scroll;
        }

        return handle::slot_setting::slot_type::misc;
    }

    bool equip_event::is_two_handed(RE::TESForm*& a_form) {
        //check if two handed
        if (a_form->Is(RE::FormType::Spell)) {
            if (const auto spell = a_form->As<RE::SpellItem>(); spell->IsTwoHanded()) {
                return true;
            }
        }
        if (a_form->IsWeapon()) {
            if (const auto weapon = a_form->As<RE::TESObjectWEAP>();
                weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() || weapon->IsBow() || weapon->IsCrossbow()) {
                return true;
            }
        }

        return false;
    }

    std::vector<data_helper*> equip_event::get_hand_assignment(RE::TESForm*& a_form) {
        bool two_handed = false;
        if (a_form) {
            two_handed = is_two_handed(a_form);
        }
        const auto player = RE::PlayerCharacter::GetSingleton();
        logger::trace("Item {} is two handed {}"sv, a_form->GetName(), two_handed);
        std::vector<data_helper*> data;

        auto right_obj = player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        auto left_obj = player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();

        const auto empty_handle = config::mcm_setting::get_empty_hand_setting();

        const auto item = new data_helper();
        item->form = nullptr;
        item->left = false;
        item->type = handle::slot_setting::slot_type::empty;
        item->action_type = empty_handle ?
                                handle::slot_setting::acton_type::unequip :
                                handle::slot_setting::acton_type::default_action;
        data.push_back(item);

        const auto item2 = new data_helper();
        item2->form = nullptr;
        item2->left = true;
        item2->type = handle::slot_setting::slot_type::empty;
        item2->action_type = empty_handle ?
                                 handle::slot_setting::acton_type::unequip :
                                 handle::slot_setting::acton_type::default_action;
        data.push_back(item2);


        logger::trace("got form {}, name {} on both/right hand"sv,
            right_obj ? util::string_util::int_to_hex(right_obj->GetFormID()) : "null",
            right_obj ? right_obj->GetName() : "null");

        logger::trace("got form {}, name {} on left hand"sv,
            left_obj ? util::string_util::int_to_hex(left_obj->GetFormID()) : "null",
            left_obj ? left_obj->GetName() : "null");

        if (two_handed && right_obj && left_obj && right_obj->formID == left_obj->formID) {
            data[0]->form = right_obj;
            data[0]->left = false;
            data[0]->type = get_type(right_obj);
            data[0]->action_type = handle::slot_setting::acton_type::default_action;
            data.erase(data.begin() + 1);
        }

        if (right_obj) {
            data[0]->form = right_obj;
            data[0]->left = false;
            data[0]->type = get_type(right_obj);
            data[0]->action_type = handle::slot_setting::acton_type::default_action;
        }

        if (left_obj) {
            data[1]->form = left_obj;
            data[1]->left = true;
            data[1]->type = get_type(left_obj);
            data[1]->action_type = handle::slot_setting::acton_type::default_action;
        }

        logger::trace("got {} items in List now. return."sv, data.size());
        return data;
    }
}
