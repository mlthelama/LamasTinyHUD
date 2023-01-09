#include "potion.h"
#include "inventory.h"

namespace item {
    void potion::consume_potion(handle::slot_setting*& a_slot, RE::PlayerCharacter*& a_player) {
        logger::trace("try to consume {}"sv, a_slot->form->GetName());

        RE::TESBoundObject* obj = nullptr;
        RE::InventoryEntryData inv_data;
        uint32_t left;
        for (auto potential_items = inventory::get_inventory_magic_items(a_player);
             const auto& [item, invData] : potential_items) {
            if (invData.second->object->formID == a_slot->form->formID) {
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


        if (obj->As<RE::MagicItem>()->IsFood()) {
            logger::trace("trying to equip/eat a food item {}, count left {}"sv, obj->GetName(), left);
            const auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->EquipObject(a_player, obj);
            a_slot->item_count = left - 1;
            logger::trace("equipped/ate a food item {}"sv, obj->GetName());
            return;
        }


        //update count and
        const auto alchemy_potion = obj->As<RE::AlchemyItem>();
        logger::trace("calling drink potion {}, count left {}"sv, alchemy_potion->GetName(), left);


        //build a "cache" with formid and count, validate after consumption
        a_player->DrinkPotion(alchemy_potion, inv_data.extraLists->front());
        a_slot->item_count = left - 1;
        logger::trace("drank potion {}. return."sv, alchemy_potion->GetName());
    }
}
