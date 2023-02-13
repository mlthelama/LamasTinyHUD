#include "helper.h"
#include "constant.h"
#include "data/config_writer_helper.h"
#include "equip/item.h"
#include "handle/data/page/position_setting.h"
#include "handle/handle/page_handle.h"
#include "setting/custom_setting.h"
#include "setting/file_setting.h"
#include "setting/mcm_setting.h"
#include "string_util.h"

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
            form_string = fmt::format("{}{}{}", dynamic_name, delimiter, string_util::int_to_hex(form->GetFormID()));
        } else {
            //it is not, search for the file it is from
            auto source_file = form->sourceFiles.array->front()->fileName;
            auto local_form = form->GetLocalFormID();

            logger::trace("form is from {}, local id is {}, translated {}"sv,
                source_file,
                local_form,
                string_util::int_to_hex(local_form));

            form_string = fmt::format("{}{}{}", source_file, delimiter, string_util::int_to_hex(local_form));
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
        RE::ActorValue actor_value = RE::ActorValue::kNone;

        auto type_left = static_cast<uint32_t>(handle::slot_setting::slot_type::empty);
        std::string form_string_left;
        uint32_t action_left = 0;

        if (a_data.empty()) {
            return;
        }

        if (config::mcm_setting::get_elden_demon_souls()) {
            if (!a_data.empty()) {
                if (a_data[0]->left) {
                    type_left = static_cast<uint32_t>(a_data[0]->type);
                    if (a_data[0]->form) {
                        form_string_left = get_mod_and_form(a_data[0]->form->formID);
                    } else {
                        form_string_left = "";
                    }
                    action_left = static_cast<uint32_t>(a_data[0]->action_type);
                } else {
                    type = static_cast<uint32_t>(a_data[0]->type);
                    if (a_data[0]->form) {
                        form_string = get_mod_and_form(a_data[0]->form->formID);
                    } else {
                        form_string = "";
                    }
                    action = static_cast<uint32_t>(a_data[0]->action_type);
                }
                actor_value = a_data[0]->actor_value;
            }
        } else {
            if (!a_data.empty()) {
                type = static_cast<uint32_t>(a_data[0]->type);
                if (a_data[0]->form) {
                    form_string = get_mod_and_form(a_data[0]->form->formID);
                } else {
                    form_string = "";
                }
                action = static_cast<uint32_t>(a_data[0]->action_type);
                actor_value = a_data[0]->actor_value;
            }


            if (a_data.size() == 2) {
                type_left = static_cast<uint32_t>(a_data[1]->type);
                if (a_data[1]->form) {
                    form_string_left = get_mod_and_form(a_data[1]->form->formID);
                } else {
                    form_string_left = "";
                }
                action_left = static_cast<uint32_t>(a_data[1]->action_type);
                actor_value = a_data[1]->actor_value;
            }
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
            action_left,
            static_cast<int>(actor_value));
        config::custom_setting::read_setting();
    }

    void helper::read_configs() {
        config::file_setting::load_setting();
        config::mcm_setting::read_setting();
        config::custom_setting::read_setting();
    }

    std::vector<std::string> helper::get_configured_section_page_names(uint32_t a_position) {
        //4 is all
        std::vector<std::string> names;
        for (const auto entries = config::custom_setting::get_sections(); const auto& entry : entries) {
            if (a_position == static_cast<uint32_t>(handle::position_setting::position_type::total)) {
                names.emplace_back(entry.pItem);
            } else {
                auto section_position = config::custom_setting::get_position_by_section(entry.pItem);
                if (section_position == a_position) {
                    names.emplace_back(entry.pItem);
                }
            }
        }
        logger::trace("got {} sections, for position {}"sv, names.size(), a_position);
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
        //check if two-handed
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
            const auto armor = a_form->As<RE::TESObjectARMO>();
            //GetSlotMask 49
            if (armor->IsShield()) {
                return handle::slot_setting::slot_type::shield;
            } else if (armor->IsClothing() &&
                       (armor->HasKeywordString("_WL_Lantern") &&
                               armor->HasPartOf(RE::BIPED_MODEL::BipedObjectSlot::kNone) &&
                               !armor->HasPartOf(RE::BIPED_MODEL::BipedObjectSlot::kModFaceJewelry) ||
                           armor->HasPartOf(RE::BIPED_MODEL::BipedObjectSlot::kModPelvisPrimary))) {
                //Wearable Lanterns got keyword _WL_Lantern
                //Simple Wearable Lanterns do not have a keyword, but will be equipped on 49 (30+19)
                return handle::slot_setting::slot_type::lantern;
            } else if (armor->IsClothing() && armor->HasKeywordString("BOS_DisplayMaskKeyword")) {
                return handle::slot_setting::slot_type::mask;
            }
            return handle::slot_setting::slot_type::armor;
        }
        if (a_form->Is(RE::FormType::Spell)) {
            const auto spell_type = a_form->As<RE::SpellItem>()->GetSpellType();
            if (spell_type == RE::MagicSystem::SpellType::kSpell ||
                spell_type == RE::MagicSystem::SpellType::kLeveledSpell) {
                return handle::slot_setting::slot_type::magic;
            }
            if (spell_type == RE::MagicSystem::SpellType::kLesserPower ||
                spell_type == RE::MagicSystem::SpellType::kPower) {
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
        if (a_form->Is(RE::FormType::Ammo)) {
            return handle::slot_setting::slot_type::misc;
        }
        if (a_form->Is(RE::FormType::Light)) {
            return handle::slot_setting::slot_type::light;
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
        item->action_type = empty_handle ? handle::slot_setting::acton_type::un_equip :
                                           handle::slot_setting::acton_type::default_action;
        data.push_back(item);

        const auto item2 = new data_helper();
        item2->form = nullptr;
        item2->left = true;
        item2->type = handle::slot_setting::slot_type::empty;
        item2->action_type = empty_handle ? handle::slot_setting::acton_type::un_equip :
                                            handle::slot_setting::acton_type::default_action;
        data.push_back(item2);

        if (!a_two_handed) {
            a_two_handed = right_obj && is_two_handed(right_obj);
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

    void helper::write_notification(const std::string& a_string) { RE::DebugNotification(a_string.c_str()); }

    data_helper* helper::is_suitable_for_position(RE::TESForm*& a_form,
        const handle::position_setting::position_type a_position) {
        //all kind of weapons and magic/spells
        const auto item = new data_helper();
        const auto type = get_type(a_form);
        const auto two_handed = is_two_handed(a_form);
        logger::trace("Item {}, is Type {}, TwoHanded {}"sv,
            a_form ? string_util::int_to_hex(a_form->formID) : "null",
            static_cast<uint32_t>(type),
            two_handed);

        switch (a_position) {
            case handle::position_setting::position_type::top:
                switch (type) {
                    case handle::slot_setting::slot_type::power:
                    case handle::slot_setting::slot_type::shout:
                        //case handle::slot_setting::slot_type::misc:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        item->action_type = can_instant_cast(a_form, type) ?
                                                handle::slot_setting::acton_type::instant :
                                                handle::slot_setting::acton_type::default_action;
                        break;
                    case handle::slot_setting::slot_type::magic:
                        if (can_instant_cast(a_form, type)) {
                            item->form = a_form;
                            item->type = type;
                            item->two_handed = two_handed;
                            item->left = false;
                            item->action_type = handle::slot_setting::acton_type::instant;
                        }
                        break;
                }
                break;
            case handle::position_setting::position_type::right:
                switch (type) {
                    case handle::slot_setting::slot_type::weapon:
                    case handle::slot_setting::slot_type::magic:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        break;
                }
                break;
            case handle::position_setting::position_type::bottom:
                switch (type) {
                    case handle::slot_setting::slot_type::consumable:
                        item->form = nullptr;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        item->actor_value = get_actor_value_effect_from_potion(a_form);
                        if (item->actor_value == RE::ActorValue::kNone) {
                            item->form = a_form;
                        }
                        break;
                    case handle::slot_setting::slot_type::lantern:  //not sure if best here
                    case handle::slot_setting::slot_type::mask:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        break;
                    case handle::slot_setting::slot_type::scroll:
                        item->form = a_form;
                        item->type = type;
                        item->two_handed = two_handed;
                        item->left = false;
                        item->action_type = handle::slot_setting::acton_type::instant;
                        break;
                }
                break;
            case handle::position_setting::position_type::left:
                switch (type) {
                    case handle::slot_setting::slot_type::weapon:
                    case handle::slot_setting::slot_type::magic:
                    case handle::slot_setting::slot_type::shield:
                    case handle::slot_setting::slot_type::light:
                        if (!two_handed) {
                            item->form = a_form;
                            item->type = type;
                            item->two_handed = two_handed;
                            item->left = true;
                            break;
                        }
                        break;
                }
                break;
        }

        return item;
    }

    void helper::rewrite_settings() {
        logger::trace("rewriting config ..."sv);
        std::map<uint32_t, uint32_t> next_page_for_position;

        for (auto i = 0; i < static_cast<int>(handle::position_setting::position_type::total); ++i) {
            next_page_for_position[i] = 0;
        }
        std::vector<config_writer_helper*> configs;
        const auto sections = get_configured_section_page_names();
        logger::trace("got {} sections, rewrite that they are in consecutive pages"sv, sections.size());
        for (const auto& section : sections) {
            auto position = config::custom_setting::get_position_by_section(section);
            const auto next_page = next_page_for_position[position];

            auto* config = new config_writer_helper();
            config->section = section;
            config->page = next_page;
            config->position = position;
            config->form = config::custom_setting::get_item_form_by_section(section);
            config->type = config::custom_setting::get_type_by_section(section);
            config->hand = config::custom_setting::get_hand_selection_by_section(section);
            config->action = config::custom_setting::get_slot_action_by_section(section);
            config->form_left = config::custom_setting::get_item_form_left_by_section(section);
            config->type_left = config::custom_setting::get_type_left_by_section(section);
            config->action_left = config::custom_setting::get_slot_action_left_by_section(section);
            config->actor_value = config::custom_setting::get_effect_actor_value(section);

            configs.push_back(config);
            next_page_for_position[position] = next_page + 1;
        }

        logger::trace("start writing config, got {} items"sv, configs.size());

        for (const auto config : configs) {
            config::custom_setting::reset_section(config->section);
            const auto section = get_section_name_for_page_position(config->page, config->position);

            config::custom_setting::write_section_setting(section,
                config->page,
                config->position,
                config->type,
                config->form,
                config->action,
                config->hand,
                config->type_left,
                config->form_left,
                config->action_left,
                config->actor_value);
        }

        next_page_for_position.clear();
        configs.clear();
        logger::trace("done rewriting."sv);
    }

    bool helper::can_instant_cast(RE::TESForm* a_form, const handle::slot_setting::slot_type a_type) {
        if (a_type == handle::slot_setting::slot_type::magic) {
            const auto spell = a_form->As<RE::SpellItem>();
            if (spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell ||
                spell->GetSpellType() == RE::MagicSystem::SpellType::kLeveledSpell) {
                if (spell->GetCastingType() != RE::MagicSystem::CastingType::kConcentration) {
                    return true;
                }
            }
            return false;
        }
        if (a_type == handle::slot_setting::slot_type::power) {
            return false;
        }
        if (a_type == handle::slot_setting::slot_type::scroll) {
            return true;
        }
        if (a_type == handle::slot_setting::slot_type::shout) {
            return false;
        }

        return false;
    }

    bool helper::already_used(const RE::TESForm* a_form,
        const handle::position_setting::position_type a_position,
        const std::vector<data_helper*>& a_config_data) {
        if (!a_form) {
            return false;
        }
        //get pages and check for the form id in the position we are editing
        const auto page_handle = handle::page_handle::get_singleton();

        uint32_t max_count = 1;
        uint32_t count = 0;
        if (a_form->IsWeapon() || a_form->Is(RE::FormType::Armor)) {
            //check item count in inventory
            max_count = equip::item::get_inventory_count(a_form);
        }

        auto actor_value = RE::ActorValue::kNone;
        if (a_form->Is(RE::FormType::AlchemyItem)) {
            actor_value = get_actor_value_effect_from_potion(const_cast<RE::TESForm*>(a_form));
        }

        auto pages = page_handle->get_pages();
        if (!pages.empty()) {
            for (auto& [page, page_settings] : pages) {
                for (auto [position, page_setting] : page_settings) {
                    if (position == a_position) {
                        for (const auto setting : page_setting->slot_settings) {
                            if (setting &&
                                ((setting->form && setting->form->formID == a_form->formID) ||
                                    (setting->actor_value == actor_value && actor_value != RE::ActorValue::kNone))) {
                                count++;
                                if (max_count == count) {
                                    logger::trace("Item already {} time(s) used. return."sv, count);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!a_config_data.empty()) {
            for (const auto data_item : a_config_data) {
                if ((data_item->form && data_item->form->formID == a_form->formID) ||
                    (data_item->actor_value == actor_value && actor_value != RE::ActorValue::kNone)) {
                    count++;
                    if (max_count == count) {
                        logger::trace("Item already {} time(s) used. return."sv, count);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::string helper::get_section_name_for_page_position(const uint32_t a_page, const uint32_t a_position) {
        //for now, I will just generate it
        return fmt::format("Page{}Position{}", a_page, a_position);
    }

    std::vector<std::string> helper::search_for_config_files(bool a_elden) {
        std::vector<std::string> file_list;
        auto file_name = ini_default_name;
        if (a_elden) {
            file_name = ini_elden_name;
        }

        logger::trace("Will start looking in Path {}"sv, ini_path);
        if (std::filesystem::is_directory(ini_path)) {
            for (const auto& entry : std::filesystem::directory_iterator(ini_path)) {
                if (is_regular_file(entry) && entry.path().extension() == util::ini_ending &&
                    entry.path().filename().string().starts_with(file_name)) {
                    logger::trace("found file {}, path {}"sv, entry.path().filename().string(), entry.path().string());
                    if (!a_elden && entry.path().filename().string().starts_with(ini_elden_name)) {
                        logger::warn("Skipping File {}, because it would also match for Elden"sv,
                            entry.path().filename().string());
                        continue;
                    }
                    file_list.push_back(entry.path().filename().string());
                }
            }
        }
        logger::trace("Got {} Files to return in Path"sv, file_list.size());
        return file_list;
    }

    void helper::block_location(handle::position_setting* a_position_setting, bool a_condition) {
        //if true block
        if (!a_position_setting || !a_position_setting->draw_setting) {
            return;
        }
        if (a_condition) {
            a_position_setting->draw_setting->icon_transparency = config::mcm_setting::get_icon_transparency_blocked();
        } else {
            a_position_setting->draw_setting->icon_transparency = config::mcm_setting::get_icon_transparency();
        }
    }

    RE::ActorValue helper::get_actor_value_effect_from_potion(RE::TESForm* a_form) {
        if (!a_form->Is(RE::FormType::AlchemyItem) || !config::mcm_setting::get_group_potions()) {
            return RE::ActorValue::kNone;
        }
        auto alchemy_potion = a_form->As<RE::AlchemyItem>();

        if (alchemy_potion->IsFood() || alchemy_potion->IsPoison()) {
            return RE::ActorValue::kNone;
        }

        const auto effect = alchemy_potion->GetCostliestEffectItem()->baseEffect;
        auto actor_value = effect->GetMagickSkill();

        if (actor_value == RE::ActorValue::kNone) {
            actor_value = effect->data.primaryAV;
        }

        if ((actor_value == RE::ActorValue::kHealth || actor_value == RE::ActorValue::kStamina ||
                actor_value == RE::ActorValue::kMagicka) &&
            effect->data.flags.none(RE::EffectSetting::EffectSettingData::Flag::kRecover)) {
            return actor_value;
        }

        return RE::ActorValue::kNone;
    }

    std::string helper::get_form_name_string_for_section(const std::string& a_str) {
        std::string display_string;
        auto form_string = config::custom_setting::get_item_form_by_section(a_str);
        auto form_string_left = config::custom_setting::get_item_form_left_by_section(a_str);

        RE::TESForm* form = nullptr;
        if (!form_string.empty()) {
            form = util::helper::get_form_from_mod_id_string(form_string);
        }
        RE::TESForm* form_left = nullptr;
        if (!form_string_left.empty()) {
            form_left = util::helper::get_form_from_mod_id_string(form_string_left);
        }

        display_string = form ? form->GetName() : "";
        if (form_left) {
            if (!display_string.empty()) {
                display_string = display_string + util::delimiter;
            }
            display_string = display_string + form_left->GetName();
        }

        return display_string.empty() ? a_str : display_string;
    }
}
