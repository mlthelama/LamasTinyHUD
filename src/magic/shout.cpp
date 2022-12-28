#include "shout.h"

#include "item/inventory.h"

namespace magic {
    std::vector<RE::TESShout*> shout::get_shouts() {
        const auto spells = RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSpellList();
        
        std::vector<RE::TESShout*> shout_list;
        
        if (spells->numShouts == 0) return shout_list;

        //maybe check if the shout has been learned
        for (uint32_t i = 0; i < spells->numShouts; ++i) {
            const auto shout = spells->shouts[i];
            logger::trace("shout is {}"sv, shout->GetName());
            shout_list.push_back(shout);
        }

        return shout_list;
    }

    void shout::equip_shout(RE::TESForm* a_form) {
        logger::trace("try to equip shout {}"sv, a_form->GetName());

        const auto shout = a_form->As<RE::TESShout>();
        //maybe check if it is already equipped
        RE::ActorEquipManager::GetSingleton()->EquipShout(RE::PlayerCharacter::GetSingleton(), shout);
    }

}
