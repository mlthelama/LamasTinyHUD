#include "spell.h"

namespace magic {
    std::vector<RE::SpellItem*> spell::get_spells() {
        const auto spells = RE::PlayerCharacter::GetSingleton()->GetActorBase()->GetSpellList();
        auto added_spells = RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().addedSpells;
        //if races have spells too, we should check those as well
        
        std::vector<RE::SpellItem*> spell_list;

        if (spells->numSpells == 0) return spell_list;

        for (uint32_t i = 0; i < spells->numSpells; ++i) {
            if (const auto spell = spells->spells[i]; spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
                logger::trace("spell is {}, casting {}, is two_handed {}, spelltype {}"sv,
                    spell->GetName(),
                    static_cast<uint32_t>(spell->GetCastingType()),
                    spell->IsTwoHanded(),
                    static_cast<uint32_t>(spell->GetSpellType()));
                spell_list.push_back(spell);
            }
        }


        for (uint32_t i = 0; i < added_spells.size(); ++i) {
            //just consider normal spells
            if (const auto spell = added_spells.data()[i];
                spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
                logger::trace("spell is {}, casting {}, is two_handed {}, spelltype {}"sv,
                    spell->GetName(),
                    static_cast<uint32_t>(spell->GetCastingType()),
                    spell->IsTwoHanded(),
                    static_cast<uint32_t>(spell->GetSpellType()));
                if (std::ranges::find(spell_list, spell) == spell_list.end()) {
                    spell_list.push_back(spell);
                }
            }
        }

        return spell_list;
    }

    //add toggle mcm if equip or cast
    void spell::instant_cast(RE::TESForm* a_form) {
        logger::trace("try to instant cast spell {}"sv, a_form->GetName());
        const auto spell = a_form->As<RE::SpellItem>();

        //spell->avEffectSetting->data.dualCastScale
        //maybe add option to dual cast
        const auto actor = RE::PlayerCharacter::GetSingleton()->As<RE::Actor>();
        actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, actor, 1.0f, false, 0.0f, nullptr);
    }
}
