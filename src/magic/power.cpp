#include "power.h"


namespace magic {
    std::vector<RE::TESForm*> power::get_powers() {
        //easier just to use items that have been favourited, just filter them
        std::vector<RE::TESForm*> power_list;

        for (auto magic_favorites = RE::MagicFavorites::GetSingleton()->spells; auto form : magic_favorites) {
            if (form->Is(RE::FormType::Spell)) {
                if (const auto spell = form->As<RE::SpellItem>();
                    spell->GetSpellType() == RE::MagicSystem::SpellType::kPower ||
                    spell->GetSpellType() == RE::MagicSystem::SpellType::kLesserPower) {
                    logger::trace("spell is {}, casting {}, is two_handed {}, spelltype {}"sv,
                        spell->GetName(),
                        static_cast<uint32_t>(spell->GetCastingType()),
                        spell->IsTwoHanded(),
                        static_cast<uint32_t>(spell->GetSpellType()));

                    power_list.push_back(form);
                } else {
                    logger::trace("{} is not a power, not needed here form type {}"sv,
                        form->GetName(),
                        form->GetFormType());
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
        //and make a setting to instant cast, maybe in the key manager already
        RE::ActorEquipManager::GetSingleton()->EquipSpell(RE::PlayerCharacter::GetSingleton(), spell);
        logger::trace("equipped power {}. return."sv, a_form->GetName());
    }
}
