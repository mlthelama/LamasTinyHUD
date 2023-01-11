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
        
        logger::trace("calling drink/eat potion/food {}, count left {}"sv, obj->GetName(), left);
        
        const auto equip_manager = RE::ActorEquipManager::GetSingleton();
        equip_manager->EquipObject(a_player, obj);
        logger::trace("drank/ate potion/food {}. return."sv, obj->GetName());
    }
}
