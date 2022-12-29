#include "power.h"


namespace magic {
    std::vector<RE::SpellItem*> power::get_powers() {
        const auto spells = RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSpellList();
        auto added_spells = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().addedSpells;
        //if races have powers too, we should check those as well

        std::vector<RE::SpellItem*> power_list;

        if (spells->numSpells == 0) return power_list;

        for (uint32_t i = 0; i < spells->numSpells; ++i) {
            if (const auto spell = spells->spells[i];
                spell->GetSpellType() == RE::MagicSystem::SpellType::kPower || spell->GetSpellType() ==
                RE::MagicSystem::SpellType::kLesserPower) {
                logger::trace("power is {}, casting {}, is two_handed {}, spelltype {}"sv,
                    spell->GetName(),
                    static_cast<uint32_t>(spell->GetCastingType()),
                    spell->IsTwoHanded(),
                    static_cast<uint32_t>(spell->GetSpellType()));
                power_list.push_back(spell);
            }
        }


        for (uint32_t i = 0; i < added_spells.size(); ++i) {
            if (const auto spell = added_spells.data()[i];
                spell->GetSpellType() == RE::MagicSystem::SpellType::kPower || spell->GetSpellType() ==
                RE::MagicSystem::SpellType::kLesserPower) {
                logger::trace("power is {}, casting {}, is two_handed {}, spelltype {}"sv,
                    spell->GetName(),
                    static_cast<uint32_t>(spell->GetCastingType()),
                    spell->IsTwoHanded(),
                    static_cast<uint32_t>(spell->GetSpellType()));
                if (std::ranges::find(power_list, spell) == power_list.end()) {
                    power_list.push_back(spell);
                }
            }
        }

        logger::trace("power list is size {}. return."sv, power_list.size());
        return power_list;
    }

    void power::equip_power(RE::TESForm* a_form) {
        logger::trace("try to equip power {}"sv, a_form->GetName());

        if (const auto selected_power = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().selectedPower;
            selected_power != nullptr) {
            logger::trace("current selected power is {}, is shout {}, is spell {}"sv,
                selected_power->GetName(),
                selected_power->Is(RE::FormType::Shout),
                selected_power->Is(RE::FormType::Spell));
            if (selected_power->formID == a_form->formID) {
                logger::debug("no need to equip power {}, it is already equipped. return."sv, a_form->GetName());
                return;
            }
        }

        const auto spell = a_form->As<RE::SpellItem>();
        //maybe check if it is already equipped
        //and make a setting to instant cast, maybe in the keymanager already
        RE::ActorEquipManager::GetSingleton()->EquipSpell(RE::PlayerCharacter::GetSingleton(), spell);
        logger::trace("equipped power {}. return."sv, a_form->GetName());
    }
}
