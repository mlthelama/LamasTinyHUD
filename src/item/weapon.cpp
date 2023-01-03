#include "weapon.h"
#include "inventory.h"

namespace item {
    void weapon::equip_weapon(const RE::TESForm* a_form) {
        logger::trace("try to equip {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        //RE::InventoryEntryData inv_data;
        //uint32_t left;
        for (auto potential_items = inventory::get_inventory_weapon_items();
             const auto& [item, invData] : potential_items) {
            if (invData.second->object->formID == a_form->formID) {
                obj = item;
                //inv_data = *invData.second;
                //left = invData.first;
                break;
            }
        }

        if (obj == nullptr) {
            logger::warn("could not find selected weapon, maybe it is gone"sv);
            //update ui in this case
            return;
        }

        logger::trace("try to equip weapon {}"sv, a_form->GetName());

        const auto weapon = obj->As<RE::TESObjectWEAP>();
        //maybe check if it is already equipped
        //RE::ActorEquipManager::GetSingleton()->EquipShout(RE::PlayerCharacter::GetSingleton(), shout);
        //		void EquipObject(Actor* a_actor
        //		, TESBoundObject* a_object
        //      , ExtraDataList* a_extraData = nullptr
        //      , std::uint32_t a_count = 1
        //      , const BGSEquipSlot* a_slot = nullptr
        //      , bool a_queueEquip = true
        //      , bool a_forceEquip = false
        //      , bool a_playSounds = true
        //      , bool a_applyNow = false);
        RE::ActorEquipManager::GetSingleton()->EquipObject(RE::PlayerCharacter::GetSingleton(), weapon);

        logger::trace("equipped weapon {}. return."sv, a_form->GetName());
    }
}
