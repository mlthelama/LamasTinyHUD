#include "potion.h"

#include "inventory.h"

#include <xbyak/xbyak.h>

namespace item {
    void potion::consume_potion(const RE::TESForm* a_form) {
        logger::trace("try to consume {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        RE::InventoryEntryData inv_data;
        uint32_t left;
        for (auto potential_items = inventory::get_inventory_magic_items(); const auto& [item, invData] :
             potential_items) {
            if (invData.second->object->formID == a_form->formID) {
                obj = item;
                inv_data = *invData.second;
                left = invData.first;
                break;
            }
        }


        if (obj == nullptr) {
            logger::warn("could not find selected potion, maybe it all have been consumed"sv);
            //update ui in this case
            return;
        }
        //update count and
        const auto alchemy_potion = obj->As<RE::AlchemyItem>();
        logger::trace("calling drink potion {}, count left {}"sv, alchemy_potion->GetName(), left);
        //build a "cache" with formid and count, validate after consumption 
        RE::PlayerCharacter::GetSingleton()->DrinkPotion(alchemy_potion,
            inv_data.extraLists->front());
    }
}
