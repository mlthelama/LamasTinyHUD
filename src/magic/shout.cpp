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

        logger::trace("shout list is size {}. return."sv, shout_list.size());
        return shout_list;
    }

    void shout::equip_shout(RE::TESForm* a_form) {
        logger::trace("try to equip shout {}"sv, a_form->GetName());

        if (const auto selected_power = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().selectedPower;
            selected_power != nullptr) {
            logger::trace("current selected power is {}, is shout {}, is spell {}"sv,
                selected_power->GetName(),
                selected_power->Is(RE::FormType::Shout),
                selected_power->Is(RE::FormType::Spell));
            if (selected_power->formID == a_form->formID) {
                logger::debug("no need to equip shout {}, it is already equipped. return."sv, a_form->GetName());
                return;
            }
        }

        const auto shout = a_form->As<RE::TESShout>();
        //maybe check if it is already equipped
        RE::ActorEquipManager::GetSingleton()->EquipShout(RE::PlayerCharacter::GetSingleton(), shout);
        logger::trace("equipped shout {}. return."sv, a_form->GetName());
    }
}
