#include "helper.h"
#include "constant.h"
#include "string_util.h"
#include "handle/data/page/position_setting.h"
#include "setting/custom_setting.h"
#include "setting/file_setting.h"
#include "setting/mcm_setting.h"

namespace util {
    std::string helper::get_mod_and_form(const RE::FormID& a_form_id) {
        std::string form_string;
        if (!a_form_id) {
            return form_string;
        }

        const auto form = RE::TESForm::LookupByID(a_form_id);
        logger::trace("Item is {}, formid {}, formid not translated {}. return."sv,
            form->GetName(),
            string_util::int_to_hex(form->GetFormID()),
            form->GetFormID());


        if (form->IsDynamicForm()) {
            form_string = fmt::format("{}{}{}",
                dynamic_name,
                delimiter,
                string_util::int_to_hex(form->GetFormID()));
        } else {
            //it is not, search for the file it is from
            auto source_file = form->sourceFiles.array->front()->fileName;
            auto local_form = form->GetLocalFormID();

            logger::trace("form is from {}, local id is {}, translated {}"sv,
                source_file,
                local_form,
                string_util::int_to_hex(local_form));

            form_string = fmt::format("{}{}{}",
                source_file,
                delimiter,
                string_util::int_to_hex(local_form));
        }

        return form_string;
    }

    void helper::write_setting_helper(const uint32_t a_page,
        const uint32_t a_position,
        const std::vector<data_helper*>& a_data,
        const uint32_t a_hand) {
        const auto section = get_section_name_for_page_position(a_page, a_position);
        auto type = static_cast<uint32_t>(handle::slot_setting::slot_type::empty);
        std::string form_string;
        uint32_t action = 0;

        auto type_left = static_cast<uint32_t>(handle::slot_setting::slot_type::empty);
        std::string form_string_left;
        uint32_t action_left = 0;

        if (a_data.empty()) {
            return;
        }
        if (!a_data.empty()) {
            type = static_cast<uint32_t>(a_data[0]->type);
            if (a_data[0]->form) {
                form_string = get_mod_and_form(a_data[0]->form->formID);
            } else {
                form_string = "";
            }
            action = static_cast<uint32_t>(a_data[0]->action_type);
        }


        if (a_data.size() == 2) {
            type_left = static_cast<uint32_t>(a_data[1]->type);
            if (a_data[1]->form) {
                form_string_left = get_mod_and_form(a_data[1]->form->formID);
            } else {
                form_string_left = "";
            }
            action_left = static_cast<uint32_t>(a_data[1]->action_type);
        }

        config::mcm_setting::read_setting();

        config::custom_setting::write_section_setting(section,
            a_page,
            a_position,
            type,
            form_string,
            action,
            a_hand,
            type_left,
            form_string_left,
            action_left);
        config::custom_setting::read_setting();
    }

    void helper::read_configs() {
        config::file_setting::load_setting();
        config::mcm_setting::read_setting();
        config::custom_setting::read_setting();
    }

    std::vector<std::string> helper::get_configured_section_page_names() {
        std::vector<std::string> names;
        for (const auto entries = config::custom_setting::get_sections(); const auto& entry : entries) {
            names.emplace_back(entry.pItem);
        }
        logger::trace("got {} sections"sv, names.size());
        return names;
    }

    RE::TESForm* helper::get_form_from_mod_id_string(const std::string& a_str) {
        if (!a_str.find(delimiter)) {
            return nullptr;
        }
        RE::TESForm* form;

        std::istringstream string_stream{ a_str };
        std::string plugin, id;

        std::getline(string_stream, plugin, *delimiter);
        std::getline(string_stream, id);
        RE::FormID form_id;
        std::istringstream(id) >> std::hex >> form_id;

        if (plugin.empty()) {
            return nullptr;
        }

        if (plugin == dynamic_name) {
            form = RE::TESForm::LookupByID(form_id);
        } else {
            logger::trace("checking mod {} for form {}"sv, plugin, form_id);

            const auto data_handler = RE::TESDataHandler::GetSingleton();
            form = data_handler->LookupForm(form_id, plugin);
        }

        if (form != nullptr) {
            logger::trace("got form id {}, name {}", string_util::int_to_hex(form->GetFormID()), form->GetName());
        }

        return form;
    }

    bool helper::is_two_handed(RE::TESForm*& a_form) {
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

    handle::slot_setting::slot_type helper::get_type(RE::TESForm*& a_form) {
        if (!a_form) {
            return handle::slot_setting::slot_type::empty;
        }

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

    std::vector<data_helper*> helper::get_hand_assignment(RE::TESForm*& a_form) {
        bool two_handed = false;
        if (a_form) {
            two_handed = is_two_handed(a_form);
        }

        logger::trace("Item {} is two handed {}"sv, a_form ? a_form->GetName() : "null", two_handed);
        return get_hand_assignment(two_handed);
    }

    std::vector<data_helper*> helper::get_hand_assignment(bool a_two_handed) {
        std::vector<data_helper*> data;
        const auto player = RE::PlayerCharacter::GetSingleton();
        auto right_obj = player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        auto left_obj = player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();

        const auto empty_handle = config::mcm_setting::get_empty_hand_setting();

        const auto item = new data_helper();
        item->form = nullptr;
        item->left = false;
        item->type = handle::slot_setting::slot_type::empty;
        item->action_type = empty_handle ?
                                handle::slot_setting::acton_type::un_equip :
                                handle::slot_setting::acton_type::default_action;
        data.push_back(item);

        const auto item2 = new data_helper();
        item2->form = nullptr;
        item2->left = true;
        item2->type = handle::slot_setting::slot_type::empty;
        item2->action_type = empty_handle ?
                                 handle::slot_setting::acton_type::un_equip :
                                 handle::slot_setting::acton_type::default_action;
        data.push_back(item2);

        if (!a_two_handed) {
            a_two_handed = right_obj ? is_two_handed(right_obj) : false;
        }

        logger::trace("got form {}, name {} on both/right hand"sv,
            right_obj ? string_util::int_to_hex(right_obj->GetFormID()) : "null",
            right_obj ? right_obj->GetName() : "null");

        logger::trace("got form {}, name {} on left hand"sv,
            left_obj ? string_util::int_to_hex(left_obj->GetFormID()) : "null",
            left_obj ? left_obj->GetName() : "null");

        if (a_two_handed && right_obj && left_obj && right_obj->formID == left_obj->formID) {
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

    void helper::write_notification(const std::string& a_string) {
        RE::DebugNotification(a_string.c_str());
    }

    bool helper::can_instant_cast(const RE::TESForm* a_form, const handle::slot_setting::slot_type a_type) {
        if (a_type == handle::slot_setting::slot_type::magic) {
            if (const auto spell = a_form->As<RE::SpellItem>();
                spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell || spell->GetSpellType() ==
                RE::MagicSystem::SpellType::kLeveledSpell) {
                if (spell->GetCastingType() != RE::MagicSystem::CastingType::kConcentration) {
                    return true;
                }
            }
            return false;
        }
        if (a_type == handle::slot_setting::slot_type::power) {
            if (const auto power = a_form->As<RE::SpellItem>();
                power->GetSpellType() == RE::MagicSystem::SpellType::kPower || power->GetSpellType() ==
                RE::MagicSystem::SpellType::kLesserPower) {
                if (power->GetCastingType() != RE::MagicSystem::CastingType::kConcentration) {
                    return true;
                }
            }
            return false;
        }
        if (a_type == handle::slot_setting::slot_type::shout) {
            return false;
        }

        return false;
    }

    std::string helper::get_section_name_for_page_position(const uint32_t a_page, const uint32_t a_position) {
        //for now i will just generate it
        return fmt::format("Page{}Position{}", a_page, a_position);
    }
}
