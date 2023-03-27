#include "item.h"
#include "equip_slot.h"
#include "handle/extra_data_holder.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/helper.h"
#include "util/player/player.h"
#include "util/string_util.h"

namespace equip {
    void item::equip_item(const RE::TESForm* a_form,
        RE::BGSEquipSlot*& a_slot,
        RE::PlayerCharacter*& a_player,
        handle::slot_setting::slot_type a_type) {
        auto left = a_slot == equip_slot::get_left_hand_slot();
        logger::trace("try to equip {}, left {}, type {}"sv, a_form->GetName(), left, static_cast<uint32_t>(a_type));

        if (a_form->formID == util::unarmed) {
            logger::trace("Got unarmed, try to call un equip"sv);
            equip_slot::un_equip_hand(a_slot, a_player, handle::slot_setting::action_type::un_equip);
            return;
        }

        RE::TESBoundObject* obj = nullptr;
        RE::ExtraDataList* extra = nullptr;
        std::vector<RE::ExtraDataList*> extra_vector;
        std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>> potential_items;
        if (a_type == handle::slot_setting::slot_type::weapon) {
            if (!a_form->Is(RE::FormType::Weapon)) {
                logger::warn("object {} is not a weapon. return."sv, a_form->GetName());
                return;
            }
            potential_items = util::player::get_inventory(a_player, RE::FormType::Weapon);
        } else if (a_type == handle::slot_setting::slot_type::shield) {
            if (!a_form->Is(RE::FormType::Armor)) {
                logger::warn("object {} is not an armor. return."sv, a_form->GetName());
                return;
            }
            potential_items = util::player::get_inventory(a_player, RE::FormType::Armor);
        } else if (a_type == handle::slot_setting::slot_type::light) {
            if (!a_form->Is(RE::FormType::Light)) {
                logger::warn("object {} is not a light. return."sv, a_form->GetName());
                return;
            }
            potential_items = util::player::get_inventory(a_player, RE::FormType::Light);
        }

        auto item_count = 0;
        for (const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                item_count = num_items;
                auto simple_extra_data_list = entry->extraLists;
                if (simple_extra_data_list) {
                    for (auto* extra_data : *simple_extra_data_list) {
                        extra = extra_data;
                        auto worn_right = extra_data->HasType(RE::ExtraDataType::kWorn);
                        auto worn_left = extra_data->HasType(RE::ExtraDataType::kWornLeft);
                        logger::trace("extra data {}, worn right {}, worn left {}"sv,
                            extra_data->GetCount(),
                            worn_right,
                            worn_left);
                        if (!worn_right && !worn_left) {
                            extra_vector.push_back(extra_data);
                        }
                    }
                }
                break;
            }
        }

        if (!obj) {
            logger::warn("could not find selected weapon/shield, maybe it is gone"sv);
            //update ui in this case
            return;
        }

        auto* extra_handler = handle::extra_data_holder::get_singleton();
        if (extra_handler->is_form_set(a_form)) {
            auto extra_list = extra_handler->get_extra_list_for_form(a_form);
            if (!extra_list.empty()) {
                extra = extra_list.back();
                extra_list.pop_back();
                extra_handler->overwrite_extra_data_for_form(a_form, extra_list);
            }
        } else {
            if (!extra_vector.empty()) {
                extra = extra_vector.back();
                extra_vector.pop_back();  //remove last item, because we already use that
                extra_handler->init_extra_data(a_form, extra_vector);
                logger::trace("set {} extra data for form {}"sv, extra_vector.size(), a_form->GetName());
            }
        }

        //if (!extra_vector.empty()) {
        //extra = extra_vector.back();
        /*if (left) {
                REL::Relocation<std::uintptr_t> extra_worn_left_vtbl{ RE::VTABLE_ExtraWornLeft[0] };
                auto* worn_left =
                    (RE::ExtraWornLeft*)RE::BSExtraData::Create(sizeof(RE::ExtraWornLeft), extra_worn_left_vtbl.get());
                extra->Add(worn_left);
            } else {
                REL::Relocation<std::uintptr_t> extra_worn_vtbl{ RE::VTABLE_ExtraWorn[0] };
                auto* worn = (RE::ExtraWorn*)RE::BSExtraData::Create(sizeof(RE::ExtraWorn), extra_worn_vtbl.get());
                extra->Add(worn);
            }*/
        //}

        const auto obj_right = a_player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        const auto obj_left = a_player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        if (left && obj_left && obj_left->formID == obj->formID) {
            logger::debug("Object Left {} is already where it should be already equipped. return."sv, obj->GetName());
            return;
        }

        if (!left && obj_right && obj_right->formID == obj->formID) {
            logger::debug("Object Right {} is already where it should be already equipped. return."sv, obj->GetName());
            return;
        }

        auto equipped_count = 0;
        if (obj_right && obj_right->formID == obj->formID) {
            equipped_count++;
            logger::debug("Object {} already equipped."sv, obj->GetName());
        }

        if (obj_left && obj_left->formID == obj->formID) {
            equipped_count++;
            logger::debug("Object {} already equipped."sv, obj->GetName());
        }

        logger::trace("Got a count of {} in the Inventory {}, Equipped {}"sv,
            obj->GetName(),
            item_count,
            equipped_count);
        if (item_count == equipped_count) {
            //all we have are already equipped
            logger::warn("All Items we have of {} are equipped, return."sv, obj->GetName());
            //try to prevent the game to equip something else
            equip_slot::un_equip_hand(a_slot, a_player, handle::slot_setting::action_type::un_equip);
            return;
        }

        logger::trace("try to equip weapon/shield/light {}"sv, a_form->GetName());
        const auto equip_manager = RE::ActorEquipManager::GetSingleton();

        equip_manager->EquipObject(a_player, obj, extra, 1, a_slot);
        logger::trace("equipped weapon/shield/light {}, left {}. return."sv, a_form->GetName(), left);
    }

    void item::equip_armor(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to equip {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        auto item_count = 0;
        for (const auto& [item, inv_data] : util::player::get_inventory(a_player, RE::FormType::Armor)) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = inv_data.second->object;
                item_count = num_items;
                break;
            }
        }

        if (!obj || item_count == 0) {
            logger::warn("could not find selected armor, maybe it is gone"sv);
            //update ui in this case
            return;
        }
        logger::trace("try to equip armor/clothing {}"sv, a_form->GetName());

        if (auto equip_manager = RE::ActorEquipManager::GetSingleton();
            !equip_slot::un_equip_if_equipped(obj, a_player, equip_manager)) {
            equip_manager->EquipObject(a_player, obj);
            logger::trace("equipped armor {}. return."sv, a_form->GetName());
        }
    }

    void item::consume_potion(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to consume {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        uint32_t left = 0;
        for (auto potential_items = util::player::get_inventory(a_player, RE::FormType::AlchemyItem);
             const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                left = num_items;
                break;
            }
        }

        if (config::mcm_setting::get_prevent_consumption_of_last_dynamic_potion() && obj && obj->IsDynamicForm() &&
            left == 1) {
            logger::warn(
                "Somehow the game crashes on potions with dynamic id if the count is 0 (happens with or without the mod). So I am not consuming it. Form {}, Name {}"sv,
                util::string_util::int_to_hex(obj->formID),
                obj->GetName());
            return;
        }

        if (!obj || left == 0) {
            logger::warn("could not find selected potion, maybe all have been consumed"sv);
            return;
        }

        if (!obj->Is(RE::FormType::AlchemyItem)) {
            logger::warn("object {} is not an alchemy item. return."sv, obj->GetName());
            return;
        }

        auto alchemy_item = obj->As<RE::AlchemyItem>();
        if (alchemy_item->IsPoison()) {
            logger::trace("try to apply poison to weapon, count left {}"sv, left);
            //check if there is a weapon to apply it to
            //check count here as well, since we need 2
            auto equipped_object = a_player->GetEquippedEntryData(false);
            if (equipped_object && equipped_object->object->IsWeapon() && !equipped_object->IsPoisoned()) {
                logger::trace("try to add poison {} to right {}"sv,
                    alchemy_item->GetName(),
                    equipped_object->GetDisplayName());
                equipped_object->PoisonObject(alchemy_item, config::mcm_setting::get_apply_poison_charges());
                a_player->RemoveItem(alchemy_item, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                left--;
            }

            auto equipped_object_left = a_player->GetEquippedEntryData(true);
            if (equipped_object_left && equipped_object_left->object->IsWeapon() &&
                !equipped_object_left->IsPoisoned() && left > 0) {
                logger::trace("try to add poison {} to left {}"sv,
                    alchemy_item->GetName(),
                    equipped_object_left->GetDisplayName());
                equipped_object_left->PoisonObject(alchemy_item, config::mcm_setting::get_apply_poison_charges());
                a_player->RemoveItem(alchemy_item, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            }
            logger::trace("Is a poison, I am done here. return.");
            return;
        }

        logger::trace("calling drink/eat potion/food {}, count left {}"sv, obj->GetName(), left);

        const auto equip_manager = RE::ActorEquipManager::GetSingleton();
        equip_manager->EquipObject(a_player, obj);
        logger::trace("drank/ate potion/food {}. return."sv, obj->GetName());
    }

    void item::equip_ammo(const RE::TESForm* a_form, RE::PlayerCharacter*& a_player) {
        logger::trace("try to equip {}"sv, a_form->GetName());

        RE::TESBoundObject* obj = nullptr;
        auto left = 0;
        for (auto potential_items = util::player::get_inventory(a_player, RE::FormType::Ammo);
             const auto& [item, inv_data] : potential_items) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                obj = item;
                left = inv_data.first;
                break;
            }
        }

        if (!obj || left == 0) {
            logger::warn("could not find selected ammo, maybe all have been used"sv);
            return;
        }

        if (const auto current_ammo = a_player->GetCurrentAmmo(); current_ammo && current_ammo->formID == obj->formID) {
            logger::debug("Ammo {} already equipped, return."sv, obj->GetName());
            return;
        }

        const auto equip_manager = RE::ActorEquipManager::GetSingleton();
        equip_manager->EquipObject(a_player, obj);
        logger::trace("equipped {}. return."sv, obj->GetName());
    }

    void item::un_equip_ammo() {
        logger::debug("check if we need to un equip ammo"sv);
        auto player = RE::PlayerCharacter::GetSingleton();

        auto obj = player->GetCurrentAmmo();
        if (!obj || !obj->IsAmmo()) {
            return;
        }

        auto ammo = obj->As<RE::TESAmmo>();
        if (ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt) ||
            ammo->GetRuntimeData().data.flags.none(RE::AMMO_DATA::Flag::kNonBolt)) {
            auto equip_manager = RE::ActorEquipManager::GetSingleton();
            equip_manager->UnequipObject(player, ammo);
            logger::trace("Called to un equip {}"sv, ammo->GetName());
        }
        logger::trace("Done work. return"sv);
    }

    void item::find_and_consume_fitting_option(RE::ActorValue a_actor_value, RE::PlayerCharacter*& a_player) {
        //get player missing value
        auto current_actor_value = a_player->AsActorValueOwner()->GetActorValue(a_actor_value);
        auto permanent_actor_value = a_player->AsActorValueOwner()->GetPermanentActorValue(a_actor_value);
        auto temporary_actor_value =
            a_player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kHealth);
        auto max_actor_value = permanent_actor_value + temporary_actor_value;
        auto missing = max_actor_value - current_actor_value;
        logger::trace("actor value {}, current {}, max {}, missing {}"sv,
            static_cast<int>(a_actor_value),
            fmt::format(FMT_STRING("{:.2f}"), current_actor_value),
            fmt::format(FMT_STRING("{:.2f}"), max_actor_value),
            fmt::format(FMT_STRING("{:.2f}"), missing));

        //min heal, max heal
        auto min_perfect = config::mcm_setting::get_potion_min_perfect();
        auto max_perfect = config::mcm_setting::get_potion_max_perfect();
        logger::trace("min perfect {}, max perfect {}, missing {}"sv,
            fmt::format(FMT_STRING("{:.2f}"), missing * min_perfect),
            fmt::format(FMT_STRING("{:.2f}"), missing * max_perfect),
            fmt::format(FMT_STRING("{:.2f}"), missing));

        RE::TESBoundObject* obj = nullptr;
        for (auto potential_items = util::player::get_inventory(a_player, RE::FormType::AlchemyItem);
             const auto& [item, inv_data] : potential_items) {
            const auto& [num_items, entry] = inv_data;

            auto alchemy_item = item->As<RE::AlchemyItem>();
            if (alchemy_item->IsPoison() || alchemy_item->IsFood()) {
                continue;
            }
            //returns currently only the types we want
            auto actor_value = util::helper::get_actor_value_effect_from_potion(item);
            if (actor_value == RE::ActorValue::kNone) {
                continue;
            }

            if (actor_value == a_actor_value) {
                //set obj here, because if we do not have a perfect hit, we still need to consume something
                if (config::mcm_setting::get_prevent_consumption_of_last_dynamic_potion() &&
                    alchemy_item->IsDynamicForm() && num_items == 1) {
                    logger::warn(
                        "Somehow the game crashes on potions with dynamic id if the count is 0 (happens with or without the mod). So I am not consuming it. Form {}, Name {}"sv,
                        util::string_util::int_to_hex(alchemy_item->formID),
                        alchemy_item->GetName());
                    continue;
                }
                obj = alchemy_item;

                auto magnitude = alchemy_item->GetCostliestEffectItem()->GetMagnitude();
                auto duration = alchemy_item->GetCostliestEffectItem()->GetDuration();
                if (duration == 0) {
                    duration = 1;
                }

                auto max_healed = magnitude * duration;
                if (max_healed >= (missing * min_perfect) && max_healed <= (missing * max_perfect)) {
                    logger::trace("found potion {}, magnitude * duration {}"sv,
                        obj->GetName(),
                        fmt::format(FMT_STRING("{:.2f}"), max_healed));
                    break;
                }
            }
        }

        if (obj) {
            logger::trace("calling to consume potion {}"sv, obj->GetName());
            consume_potion(obj, a_player);
        } else {
            logger::warn("No suitable potion found. return.");
        }
    }
}
