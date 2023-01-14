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


    void power::equip_or_cast_power(RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player) {
        logger::trace("try to work power {}, action {}"sv, a_form->GetName(), static_cast<uint32_t>(a_action));

        if (!a_form->Is(RE::FormType::Spell)) {
            logger::warn("object {} is not a spell. return."sv, a_form->GetName());
            return;
        }

        if (const auto selected_power = a_player->GetActorRuntimeData().selectedPower;
            selected_power && a_action != handle::slot_setting::acton_type::instant) {
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

        if (!a_player->HasSpell(spell)) {
            logger::warn("player does not have spell {}. return."sv, spell->GetName());
            return;
        }

        if (a_action == handle::slot_setting::acton_type::instant) {
            //might not consider daily cool downs
            const auto actor = a_player->As<RE::Actor>();
            actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                 ->CastSpellImmediate(spell, false, actor, 1.0f, false, 0.0f, nullptr);
        } else {
            RE::ActorEquipManager::GetSingleton()->EquipSpell(a_player, spell);
        }
        logger::trace("worked power {} action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }
}
