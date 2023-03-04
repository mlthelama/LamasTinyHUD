#include "magic.h"
#include "equip_slot.h"
#include "setting/mcm_setting.h"
#include "util/offset.h"
#include "util/player/player.h"

namespace equip {
    //add toggle mcm if equip or cast
    void magic::cast_magic(RE::TESForm* a_form,
        action_type a_action,
        const RE::BGSEquipSlot* a_slot,
        RE::PlayerCharacter*& a_player) {
        auto left = a_slot == equip_slot::get_left_hand_slot();
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
            if (config::mcm_setting::get_elden_demon_souls()) {
                //normally in elden just top uses instant for spells
                equip::equip_slot::un_equip_shout_slot(a_player);
            }

            //might cost nothing if nothing has been equipped into tha hands after start, so it seems
            auto cost = spell->CalculateMagickaCost(a_player);
            logger::trace("spell cost for {} is {}"sv, spell->GetName(), fmt::format(FMT_STRING("{:.2f}"), cost));

            float current_magicka = a_player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
            logger::trace("got temp magicka {}, cost {}"sv, current_magicka, cost);

            if (current_magicka < cost) {
                if (const auto ui = RE::UI::GetSingleton(); !ui->GetMenu<RE::HUDMenu>()) {
                    logger::warn("Will not flash HUD Menu, because I could not find it.");
                } else {
                    offset::flash_hud_menu_meter(RE::ActorValue::kMagicka);
                }
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
            actor->GetMagicCaster(get_casting_source(a_slot))
                ->CastSpellImmediate(spell, false, actor, 1.0f, false, 0.0f, nullptr);
        } else {
            const auto obj_right = a_player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
            const auto obj_left = a_player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
            if (left && obj_left && obj_left->formID == spell->formID) {
                logger::debug("Object Left {} is already where it should be already equipped. return."sv,
                    spell->GetName());
                return;
            }
            if (!left && obj_right && obj_right->formID == spell->formID) {
                logger::debug("Object Right {} is already where it should be already equipped. return."sv,
                    spell->GetName());
                return;
            }

            //other slot options like I thought did not work, so I get it like this now
            const auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->EquipSpell(a_player, spell, a_slot);
        }

        logger::trace("worked spell {}, action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }

    void magic::cast_scroll(const RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player) {
        logger::trace("try to work scroll {}, action {}"sv, a_form->GetName(), static_cast<uint32_t>(a_action));

        if (!a_form->Is(RE::FormType::Scroll)) {
            logger::warn("object {} is not a scroll. return."sv, a_form->GetName());
            return;
        }

        RE::TESBoundObject* obj = nullptr;
        uint32_t left = 0;
        for (auto potential_items = util::player::get_inventory(a_player, RE::FormType::Scroll);
             const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                left = inv_data.first;
                break;
            }
        }

        if (!obj || left == 0) {
            logger::warn("could not find selected scroll, maybe it all have been consumed"sv);
            return;
        }

        if (a_action == action_type::instant) {
            const auto actor = a_player->As<RE::Actor>();
            const auto scroll = obj->As<RE::ScrollItem>();
            actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                ->CastSpellImmediate(scroll, false, actor, 1.0f, false, 0.0f, nullptr);
            actor->RemoveItem(scroll, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
        } else {
            const auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->EquipObject(a_player, obj);
        }

        logger::trace("worked scroll {}, action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }

    void magic::equip_or_cast_power(RE::TESForm* a_form, action_type a_action, RE::PlayerCharacter*& a_player) {
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
            //try that
            if (config::mcm_setting::get_elden_demon_souls()) {
                //normally in elden just top uses instant for spells
                equip::equip_slot::un_equip_shout_slot(a_player);
            }
            //might not consider daily cool downs
            const auto actor = a_player->As<RE::Actor>();
            actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                ->CastSpellImmediate(spell, false, actor, 1.0f, false, 0.0f, nullptr);
        } else {
            RE::ActorEquipManager::GetSingleton()->EquipSpell(a_player, spell);
        }
        logger::trace("worked power {} action {}. return."sv, a_form->GetName(), static_cast<uint32_t>(a_action));
    }

    void magic::equip_shout(RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to equip shout {}"sv, a_form->GetName());

        if (!a_form->Is(RE::FormType::Shout)) {
            logger::warn("object {} is not a shout. return."sv, a_form->GetName());
            return;
        }

        if (const auto selected_power = a_player->GetActorRuntimeData().selectedPower; selected_power) {
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
        if (!util::player::has_shout(a_player, shout)) {
            logger::warn("player does not have spell {}. return."sv, shout->GetName());
            return;
        }

        RE::ActorEquipManager::GetSingleton()->EquipShout(a_player, shout);
        logger::trace("equipped shout {}. return."sv, a_form->GetName());
    }

    RE::MagicSystem::CastingSource magic::get_casting_source(const RE::BGSEquipSlot* a_slot) {
        if (a_slot == equip_slot::get_right_hand_slot()) {
            return RE::MagicSystem::CastingSource::kRightHand;
        }
        if (a_slot == equip_slot::get_left_hand_slot()) {
            return RE::MagicSystem::CastingSource::kLeftHand;
        }
        return RE::MagicSystem::CastingSource::kOther;
    }
}
