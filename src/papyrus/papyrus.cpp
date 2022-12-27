#include "papyrus.h"

#include "handle/slot_setting_handle.h"
#include "item/inventory.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/string_util.h"

namespace papyrus {
    static const char* mcm_name = "LamasTinyHUD_MCM";

    //handle startup so we fill the data as well
    
    auto inventory_data_list = new std::vector<RE::InventoryEntryData>;

    void hud_mcm::on_config_close(RE::TESQuest*) {
        logger::trace("Reading mcm config ..."sv);
        config::mcm_setting::read_setting();

        logger::trace("Setting handlers ..."sv);
        if (config::mcm_setting::get_selected_top_item_form() > 0) {
            handle::slot_setting_handle::get_singleton()->init_top_setting(
                RE::TESForm::LookupByID(config::mcm_setting::get_selected_top_item_form()),
                static_cast<util::selection_type>(config::mcm_setting::get_top_type()));
        }
    }

    std::vector<RE::BSFixedString> hud_mcm::get_selected_options(RE::TESQuest*, uint32_t a_id) {
        logger::info("Got refresh for id {}"sv, a_id);
        std::vector<RE::BSFixedString> empty_string_vec = { util::empty_enum_string };
        const auto display_string_list = new std::vector<RE::BSFixedString>;
        inventory_data_list->clear();


        if (a_id == static_cast<uint32_t>(util::selection_type::item)) {
            for (auto potential_items = item::inventory::get_inventory_magic_items(); const auto& [item, invData] :
                 potential_items) {
                //just consider favored items
                if (invData.second->IsFavorited()) {
                    logger::trace("Item name {}, count {}"sv, invData.second->GetDisplayName(), invData.first);
                    inventory_data_list->push_back(*invData.second);
                    display_string_list->push_back(
                        RE::BSFixedString{
                            format(FMT_STRING("{} ({})"), invData.second->GetDisplayName(), invData.first) });
                }
            }
        } else {
            logger::warn("Selected type {} not supported"sv, a_id);
        }


        if (display_string_list->empty()) {
            return empty_string_vec;
        }

        return *display_string_list;
    }

    uint32_t hud_mcm::get_form_id_for_selection(RE::TESQuest*, uint32_t a_index) {
        logger::trace("Got Index {}, Search for Item"sv, a_index);

        if (inventory_data_list->empty()) return 0;
        logger::trace("Vector got a size of {}", inventory_data_list->size());
        const auto item = inventory_data_list->at(a_index);

        const auto item_obj = item.GetObject();
        logger::trace("Item is {}, formid {}, formid not translated {}"sv,
            item.object->GetName(),
            util::string_util::int_to_hex(item_obj->GetFormID()),
            item_obj->GetFormID());

        //return util::string_util::int_to_hex(item_obj->GetFormID());
        return item_obj->GetFormID();
    }

    bool hud_mcm::Register(RE::BSScript::IVirtualMachine* a_vm) {
        a_vm->RegisterFunction("OnConfigClose", mcm_name, on_config_close);
        a_vm->RegisterFunction("GetSelectedOptions", mcm_name, get_selected_options);
        a_vm->RegisterFunction("GetFormIdForSelection", mcm_name, get_form_id_for_selection);

        logger::info("Registered {} class"sv, mcm_name);
        return true;
    }

    void Register() {
        const auto papyrus = SKSE::GetPapyrusInterface();
        papyrus->Register(hud_mcm::Register);
        logger::info("Registered papyrus functions"sv);
    }
}
