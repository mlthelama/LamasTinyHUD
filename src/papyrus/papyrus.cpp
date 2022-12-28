#include "papyrus.h"

#include "handle/read_data.h"
#include "item/inventory.h"
#include "magic/power.h"
#include "magic/shout.h"
#include "magic/spell.h"
#include "util/constant.h"
#include "util/string_util.h"

namespace papyrus {
    static const char* mcm_name = "LamasTinyHUD_MCM";


    void hud_mcm::on_config_close(RE::TESQuest*) {
        logger::info("on config close"sv);
        handle::read_data::read_config_and_set_data();

        index_ = util::selection_type::unset;
        clear_list();
    }

    std::vector<RE::BSFixedString> hud_mcm::get_selected_options(RE::TESQuest*, uint32_t a_id) {
        logger::info("Got refresh for id {}"sv, a_id);
        std::vector<RE::BSFixedString> empty_string_vec = { util::empty_enum_string };
        const auto display_string_list = new std::vector<RE::BSFixedString>;
        clear_list();

        index_ = static_cast<util::selection_type>(a_id);

        if (index_ == util::selection_type::item) {
            //maybe add a check if it is a potion
            for (auto potential_items = item::inventory::get_inventory_magic_items(); const auto& [item, inv_data] :
                 potential_items) {
                //just consider favored items
                const auto& [num_items, entry] = inv_data;
                if (inv_data.second->IsFavorited()) {
                    logger::trace("Item name {}, count {}"sv, entry->GetDisplayName(), num_items);
                    inventory_data_list_->push_back(*inv_data.second);
                    display_string_list->push_back(
                        RE::BSFixedString{
                            format(FMT_STRING("{} ({})"), entry->GetDisplayName(), num_items) });
                }
            }
            logger::trace("potion list is size {}"sv, inventory_data_list_->size());
        } else if (index_ == util::selection_type::magic) {
            //add filter for casting
            for (const auto spell_list = magic::spell::get_spells(); const auto spell : spell_list) {
                display_string_list->push_back(spell->GetName());
                spell_data_list_->push_back(spell);
            }
        } else if (index_ == util::selection_type::shout) {
            for (const auto shout_list = magic::shout::get_shouts(); const auto shout : shout_list) {
                display_string_list->push_back(shout->GetName());
                shout_data_list_->push_back(shout);
            }
        } else if (index_ == util::selection_type::power) {
            /*if (const auto selected_power = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().selectedPower;
                selected_power != nullptr) {
                logger::trace("current selected power is"sv, selected_power->GetName());
            }*/
            //power_data_list
            for (const auto power_list = magic::power::get_powers(); const auto power : power_list) {
                display_string_list->push_back(power->GetName());
                power_data_list_->push_back(power);
            }
        } else if (index_ == util::selection_type::weapon) {
            for (auto potential_weapons = item::inventory::get_inventory_weapon_items(); const auto& [item, inv_data] :
                 potential_weapons) {
                //just consider favored items
                const auto& [num_items, entry] = inv_data;
                if (inv_data.second->IsFavorited()) {
                    logger::trace("Weapon name {}, count {}"sv, entry->GetDisplayName(), num_items);
                    weapon_data_list_->push_back(*inv_data.second);
                    display_string_list->push_back(
                        RE::BSFixedString{
                            format(FMT_STRING("{} ({})"), entry->GetDisplayName(), num_items) });
                }
            }
            logger::trace("weapon list is size {}"sv, weapon_data_list_->size());
        } else {
            logger::warn("Selected type {} not supported"sv, a_id);
        }

        if (display_string_list->empty()) {
            return empty_string_vec;
        }

        return *display_string_list;
    }

    uint32_t hud_mcm::get_form_id_for_selection(RE::TESQuest*, uint32_t a_index) {
        logger::trace("Got Index {}, Search for Item for type {}"sv, a_index, static_cast<uint32_t>(index_));

        RE::FormID form_id = 0;

        if (index_ == util::selection_type::item && !inventory_data_list_->empty()) {
            logger::trace("Vector got a size of {}"sv, inventory_data_list_->size());
            if (is_size_ok(a_index, inventory_data_list_->size())) {
                const auto item = inventory_data_list_->at(a_index);
                const RE::TESBoundObject* item_obj = item.GetObject();
                form_id = item_obj->GetFormID();
            }
        } else if (index_ == util::selection_type::magic && !spell_data_list_->empty()) {
            logger::trace("Vector got a size of {}"sv, spell_data_list_->size());
            if (is_size_ok(a_index, spell_data_list_->size())) {
                const auto spell = spell_data_list_->at(a_index);
                form_id = spell->GetFormID();
            }
        } else if (index_ == util::selection_type::shout && !shout_data_list_->empty()) {
            logger::trace("Vector got a size of {}"sv, shout_data_list_->size());
            if (is_size_ok(a_index, shout_data_list_->size())) {
                const auto shout = shout_data_list_->at(a_index);
                form_id = shout->GetFormID();
            }
        } else if (index_ == util::selection_type::power && !power_data_list_->empty()) {
            logger::trace("Vector got a size of {}"sv, power_data_list_->size());
            if (is_size_ok(a_index, power_data_list_->size())) {
                const auto power = power_data_list_->at(a_index);
                form_id = power->GetFormID();
            }
        } else if (index_ == util::selection_type::weapon && !weapon_data_list_->empty()) {
            logger::trace("Vector got a size of {}"sv, weapon_data_list_->size());
            if (is_size_ok(a_index, weapon_data_list_->size())) {
                const auto item = weapon_data_list_->at(a_index);
                const RE::TESBoundObject* item_obj = item.GetObject();
                form_id = item_obj->GetFormID();
            }
        }

        if (form_id == 0) return 0;

        const auto form = RE::TESForm::LookupByID(form_id);
        logger::trace("Item is {}, formid {}, formid not translated {}"sv,
            form->GetName(),
            util::string_util::int_to_hex(form->GetFormID()),
            form->GetFormID());

        return form_id;
    }

    bool hud_mcm::Register(RE::BSScript::IVirtualMachine* a_vm) {
        a_vm->RegisterFunction("OnConfigClose", mcm_name, on_config_close);
        a_vm->RegisterFunction("GetSelectedOptions", mcm_name, get_selected_options);
        a_vm->RegisterFunction("GetFormIdForSelection", mcm_name, get_form_id_for_selection);

        logger::info("Registered {} class"sv, mcm_name);
        return true;
    }

    void hud_mcm::clear_list() {
        inventory_data_list_->clear();
        shout_data_list_->clear();
        spell_data_list_->clear();
        power_data_list_->clear();
        weapon_data_list_->clear();
    }

    bool hud_mcm::is_size_ok(uint32_t a_idx, uint64_t a_size) {
        if (a_idx > a_size) {
            logger::warn("Index is {} but size is just {}, does not fit"sv, a_idx, a_size);
            return false;
        }
        return true;
    }

    void Register() {
        const auto papyrus = SKSE::GetPapyrusInterface();
        papyrus->Register(hud_mcm::Register);
        logger::info("Registered papyrus functions"sv);
    }
}
