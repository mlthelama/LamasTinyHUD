#include "potion.h"

#include "inventory.h"

#include <xbyak/xbyak.h>

namespace item {
    void potion::consume_potion(const RE::TESForm* a_form) {
        logger::trace("try to consume {}"sv, a_form->GetName());
        //if not invalidate, config setting 
        /*if (!a_form->IsMagicItem() || !a_form->IsInventoryObject()) return;
        const auto potion_inv_data = a_form->As<RE::InventoryEntryData>();

        logger::trace("potion {}, got {} left"sv, potion_inv_data->object->GetName(), potion_inv_data->countDelta);
        //if empty invalidate, config setting 
        if (potion_inv_data->extraLists->empty()) return;

        const auto alchemy_potion = potion_inv_data->object->As<RE::AlchemyItem>();
        logger::trace("calling drink potion {}"sv, alchemy_potion->GetName());
        */
        RE::TESBoundObject* obj = nullptr;
        RE::InventoryEntryData inv_data;
        for (auto potential_items = item::inventory::get_inventory_magic_items(); const auto& [item, invData] :
             potential_items) {
            if (invData.second->object->formID == a_form->formID) {
                obj = item;
                inv_data = *invData.second;
                break;
            }
        }

        
        if (obj == nullptr) {
            logger::warn("could not find selected potion"sv);
            return;
        }
        //update count and
        const auto alchemy_potion = obj->As<RE::AlchemyItem>();
        logger::trace("calling drink potion {}"sv, alchemy_potion->GetName());
        //build a "cache" with formid and count, validate after consumption 
        RE::PlayerCharacter::GetSingleton()->DrinkPotion(alchemy_potion,
            inv_data.extraLists->front());
    }

}
