#include "spell.h"
#include "equip/equip_slot.h"
#include "equip/item/inventory.h"
#include "util/offset.h"

namespace magic {
    std::vector<RE::TESForm*> spell::get_spells(const bool a_instant, const bool a_single) {
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
                    if (spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration && a_instant) {
                        logger::debug("skipping spell {} because it does not work will with instant cast"sv,
                            spell->GetName());
                        continue;
                    }

                    if ((spell->IsTwoHanded() && !a_single) || (!spell->IsTwoHanded() && a_single)) {
                        spell_list.push_back(form);
                    }
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
        auto left = a_slot == item::equip_slot::get_left_hand_slot();
        logger::trace("try to work spell {}, action {}, left {}"sv,
            a_form->GetName(),
            static_cast<uint32_t>(a_action),
            left);

        if (!a_form->Is(RE::FormType::Spell)) {
            logger::warn("object {} is not a spell. return."sv, a_form->GetName());
            return;
        }

        const auto spell = a_form->As<RE::SpellItem>();

        //spell->avEffectSetting->data.dualCastScale
        //maybe add option to dual cast
        //and that it does consume mana
        //spell->avEffectSetting->data.baseCost
        /*logger::trace("dual cast scale {}, base cost {}"sv,
            spell->avEffectSetting->data.dualCastScale,
            spell->avEffectSetting->data.baseCost);*/

        if (!a_player->HasSpell(spell)) {
            logger::warn("player does not have spell {}. return."sv, spell->GetName());
            return;
        }

        //maybe check if the spell is already equipped
        const auto actor = a_player->As<RE::Actor>();
        auto casting_type = spell->GetCastingType();
        logger::trace("spell {} is type {}"sv, spell->GetName(), static_cast<uint32_t>(casting_type));
        if (a_action == action_type::instant && casting_type != RE::MagicSystem::CastingType::kConcentration) {
            //might cost nothing if nothing has been equipped into tha hands after start, so it seems
            auto cost = spell->CalculateMagickaCost(a_player);
            logger::trace("spell cost for {} is {}"sv, spell->GetName(), fmt::format(FMT_STRING("{:.2f}"), cost));

            float current_magicka = a_player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
            logger::trace("got temp magicka {}, cost {}"sv, current_magicka, cost);

            if (current_magicka < cost) {
                offset::flash_hud_menu_meter(RE::ActorValue::kMagicka);
                logger::warn("not enough magicka for spell {}, magicka {}, cost {} return."sv,
                    a_form->GetName(),
                    current_magicka,
                    cost);
                return;
            }

            a_player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                RE::ActorValue::kMagicka,
                -cost);

            //could trigger an animation here
            //might need to set some things
            //TODO make an animation to play here
            //a_player->NotifyAnimationGraph("RightCastSelf");
            actor->GetMagicCaster(get_casting_source(a_slot))->CastSpellImmediate(spell,
                false,
                actor,
                1.0f,
                false,
                0.0f,
                nullptr);
        } else {

            const auto obj_right = a_player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
            const auto obj_left = a_player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
            if(left && obj_left && obj_left->formID == spell->formID ) {
                logger::debug("Object Left {} is already where it should be already equipped. return."sv, spell->GetName());
                return;
            }
            if(!left && obj_right && obj_right->formID == spell->formID ) {
                logger::debug("Object Right {} is already where it should be already equipped. return."sv, spell->GetName());
                return;
            }

            //other slot options like i thought did not work, so i get it like this now
            const auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->EquipSpell(a_player, spell, a_slot);
        }

        logger::trace("worked spell {}, action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }

    void spell::cast_scroll(const RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player) {
        logger::trace("try to work scroll {}, action {}"sv,
            a_form->GetName(),
            static_cast<uint32_t>(a_action));

        if (!a_form->Is(RE::FormType::Scroll)) {
            logger::warn("object {} is not a scroll. return."sv, a_form->GetName());
            return;
        }

        RE::TESBoundObject* obj = nullptr;
        uint32_t left = 0;
        for (auto potential_items = item::inventory::get_inventory(a_player, RE::FormType::Scroll);
             const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                left = inv_data.first;
                break;
            }
        }

        if (obj == nullptr || left == 0) {
            logger::warn("could not find selected scroll, maybe it all have been consumed"sv);
        }

        if (a_action == action_type::instant) {
            const auto actor = a_player->As<RE::Actor>();
            const auto scroll = obj->As<RE::ScrollItem>();
            actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(scroll,
                false,
                actor,
                1.0f,
                false,
                0.0f,
                nullptr);
            actor->RemoveItem(scroll, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
        } else {
            const auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->EquipObject(a_player, obj);
        }

        logger::trace("worked scroll {}, action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }

    RE::MagicSystem::CastingSource spell::get_casting_source(const RE::BGSEquipSlot* a_slot) {
        if (a_slot == item::equip_slot::get_right_hand_slot()) {
            return RE::MagicSystem::CastingSource::kRightHand;
        }
        if (a_slot == item::equip_slot::get_left_hand_slot()) {
            return RE::MagicSystem::CastingSource::kLeftHand;
        }
        return RE::MagicSystem::CastingSource::kOther;
    }
}
