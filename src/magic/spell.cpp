#include "spell.h"

#include "equip/equip_slot.h"
#include "handle/setting_execute.h"

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
    void spell::cast_magic(RE::TESForm* a_form,
        action_type a_action,
        const RE::BGSEquipSlot* a_slot,
        RE::PlayerCharacter*& a_player) {
        auto left = a_slot == equip::equip_slot::get_left_hand_slot();
        logger::trace("try to work spell {}, action {}, left {}"sv,
            a_form->GetName(),
            static_cast<uint32_t>(a_action),
            left);
        const auto spell = a_form->As<RE::SpellItem>();

        //spell->avEffectSetting->data.dualCastScale
        //maybe add option to dual cast
        //and that it does consume mana
        //spell->avEffectSetting->data.baseCost
        /*logger::trace("dual cast scale {}, base cost {}"sv,
            spell->avEffectSetting->data.dualCastScale,
            spell->avEffectSetting->data.baseCost);*/

        //maybe check if the spell is already equipped
        const auto actor = a_player->As<RE::Actor>();
        if (a_action == action_type::instant) {
            //TODO add magicka consumption and check if xp gets added
            /*actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                 ->CastSpellImmediate(spell, false, actor, 1.0f, false, 0.0f, nullptr);*/
            actor->GetMagicCaster(get_casting_source(a_slot))->CastSpellImmediate(spell,
                false,
                actor,
                1.0f,
                false,
                0.0f,
                nullptr);
        } else {
            //other slot options like i thought, so i get it like this now
            auto equip_manager = RE::ActorEquipManager::GetSingleton();
            if (a_slot != nullptr) {
                handle::setting_execute::unequip_if_equipped(left, a_player, equip_manager);
            }
            equip_manager->EquipSpell(a_player, spell, a_slot);
        }

        logger::trace("worked spell {}, action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }

    RE::MagicSystem::CastingSource spell::get_casting_source(const RE::BGSEquipSlot* a_slot) {
        if (a_slot == equip::equip_slot::get_right_hand_slot()) {
            return RE::MagicSystem::CastingSource::kRightHand;
        }
        if (a_slot == equip::equip_slot::get_left_hand_slot()) {
            return RE::MagicSystem::CastingSource::kLeftHand;
        }
        return RE::MagicSystem::CastingSource::kOther;
    }
}
