#include "spell.h"


namespace magic {
    std::vector<RE::TESForm*> spell::get_spells() {
        //easier just to use items that have been favourited, just filter them
        std::vector<RE::TESForm*> spell_list;

        for (auto magic_favorites = RE::MagicFavorites::GetSingleton()->spells; auto form : magic_favorites) {
            if (form->Is(RE::FormType::Spell)) {
                if (const auto spell = form->As<RE::SpellItem>();
                    spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
                    logger::trace("spell is {}, casting {}, is two_handed {}, spelltype {}"sv,
                        spell->GetName(),
                        static_cast<uint32_t>(spell->GetCastingType()),
                        spell->IsTwoHanded(),
                        static_cast<uint32_t>(spell->GetSpellType()));
                    spell_list.push_back(form);
                } else {
                    logger::trace(" {} is not a spell, not needed here form type {}"sv,
                        form->GetName(),
                        form->GetFormType());
                }
            }
        }


        logger::trace("spell list is size {}. return."sv, spell_list.size());
        return spell_list;
    }

    //add toggle mcm if equip or cast
    void spell::instant_cast(RE::TESForm* a_form) {
        logger::trace("try to instant cast spell {}"sv, a_form->GetName());
        const auto spell = a_form->As<RE::SpellItem>();

        //spell->avEffectSetting->data.dualCastScale
        //maybe add option to dual cast
        //and that it does consume mana
        //spell->avEffectSetting->data.baseCost
        /*logger::trace("dual cast scale {}, base cost {}"sv,
            spell->avEffectSetting->data.dualCastScale,
            spell->avEffectSetting->data.baseCost);*/
        const auto actor = RE::PlayerCharacter::GetSingleton()->As<RE::Actor>();
        actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
             ->CastSpellImmediate(spell, false, actor, 1.0f, false, 0.0f, nullptr);
        logger::trace("instant casted spell {}. return."sv, a_form->GetName());
    }
}
