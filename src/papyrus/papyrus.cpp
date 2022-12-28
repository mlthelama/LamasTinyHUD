#include "papyrus.h"

#include "handle/slot_setting_handle.h"
#include "item/inventory.h"
#include "magic/shout.h"
#include "magic/spell.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/string_util.h"

namespace papyrus {
    static const char* mcm_name = "LamasTinyHUD_MCM";

    //handle startup so we fill the data as well
    auto inventory_data_list = new std::vector<RE::InventoryEntryData>;
    auto shout_data_list = new std::vector<RE::TESShout*>;
    auto spell_data_list = new std::vector<RE::SpellItem*>;
    util::selection_type index = util::selection_type::unset;

    void hud_mcm::on_config_close(RE::TESQuest*) {
        logger::trace("Reading mcm config ..."sv);
        config::mcm_setting::read_setting();

        logger::trace("Setting handlers ..."sv);
        if (config::mcm_setting::get_selected_top_item_form() > 0) {
            handle::slot_setting_handle::get_singleton()->init_top_setting(
                RE::TESForm::LookupByID(config::mcm_setting::get_selected_top_item_form()),
                static_cast<util::selection_type>(config::mcm_setting::get_top_type()));
        }
        index = util::selection_type::unset;
        clear_list();
    }

    std::vector<RE::BSFixedString> hud_mcm::get_selected_options(RE::TESQuest*, uint32_t a_id) {
        logger::info("Got refresh for id {}"sv, a_id);
        std::vector<RE::BSFixedString> empty_string_vec = { util::empty_enum_string };
        const auto display_string_list = new std::vector<RE::BSFixedString>;
        clear_list();

        index = static_cast<util::selection_type>(a_id);

        if (index == util::selection_type::item) {
            //maybe add a check if it is a potion
            for (auto potential_items = item::inventory::get_inventory_magic_items(); const auto& [item, inv_data] :
                 potential_items) {
                //just consider favored items
                const auto& [num_items, entry] = inv_data;
                if (inv_data.second->IsFavorited()) {
                    logger::trace("Item name {}, count {}"sv, entry->GetDisplayName(), num_items);
                    inventory_data_list->push_back(*inv_data.second);
                    display_string_list->push_back(
                        RE::BSFixedString{
                            format(FMT_STRING("{} ({})"), entry->GetDisplayName(), num_items) });
                }
            }
        } else if (index == util::selection_type::magic) {
            //add filter for casting
            for (const auto spell_list = magic::spell::get_spells(); const auto spell : spell_list) {
                display_string_list->push_back(spell->GetName());
                spell_data_list->push_back(spell);
            }
        } else if (index == util::selection_type::shout) {
            for (const auto shout_list = magic::shout::get_shouts(); const auto shout : shout_list) {
                display_string_list->push_back(shout->GetName());
                shout_data_list->push_back(shout);
            }
        } else if (index == util::selection_type::power) {
            /*if (const auto selected_power = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().selectedPower;
                selected_power != nullptr) {
                logger::trace("current selected power is"sv, selected_power->GetName());
            }*/
        } else {
            logger::warn("Selected type {} not supported"sv, a_id);
        }

        if (display_string_list->empty()) {
            return empty_string_vec;
        }

        return *display_string_list;
    }

    uint32_t hud_mcm::get_form_id_for_selection(RE::TESQuest*, uint32_t a_index) {
        logger::trace("Got Index {}, Search for Item for type {}"sv, a_index, static_cast<uint32_t>(index));

        RE::FormID form_id = 0;

        if (index == util::selection_type::item && !inventory_data_list->empty()) {
            logger::trace("Vector got a size of {}"sv, inventory_data_list->size());
            if (is_size_ok(a_index, inventory_data_list->size())) {
                const auto item = inventory_data_list->at(a_index);
                const RE::TESBoundObject* item_obj = item.GetObject();
                form_id = item_obj->GetFormID();
            }
        } else if (index == util::selection_type::magic && !spell_data_list->empty()) {
            logger::trace("Vector got a size of {}"sv, spell_data_list->size());
            if (is_size_ok(a_index, spell_data_list->size())) {
                const auto spell = spell_data_list->at(a_index);
                form_id = spell->GetFormID();
            }
        } else if (index == util::selection_type::shout && !shout_data_list->empty()) {
            logger::trace("Vector got a size of {}"sv, shout_data_list->size());
            if (is_size_ok(a_index, shout_data_list->size())) {
                const auto shout = shout_data_list->at(a_index);
                form_id = shout->GetFormID();
            }
        } else if (index == util::selection_type::power) { } else if (index == util::selection_type::weapon) { }

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
        inventory_data_list->clear();
        shout_data_list->clear();
        spell_data_list->clear();
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
