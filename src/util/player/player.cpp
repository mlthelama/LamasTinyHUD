#include "player.h"
#include "handle/data/ammo_data.h"
#include "handle/handle/ammo_handle.h"
#include "setting/mcm_setting.h"
#include "util/helper.h"
#include "util/offset.h"
#include "util/string_util.h"

namespace util {
    std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>>
        player::get_inventory(RE::PlayerCharacter*& a_player, RE::FormType a_type) {
        return a_player->GetInventory([a_type](const RE::TESBoundObject& a_object) { return a_object.Is(a_type); });
    }

    bool player::is_item_worn(RE::TESBoundObject*& a_obj, RE::PlayerCharacter*& a_player) {
        auto worn = false;
        for (const auto& [item, inv_data] : get_inventory(a_player, RE::FormType::Armor)) {
            if (const auto& [count, entry] = inv_data; entry->object->formID == a_obj->formID && entry->IsWorn()) {
                worn = true;
                break;
            }
        }
        return worn;
    }

    uint32_t player::get_inventory_count(const RE::TESForm* a_form) {
        uint32_t count = 0;
        if (!a_form) {
            return count;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        if (a_form->IsWeapon()) {
            count = get_inventory_count(a_form, RE::FormType::Weapon, player);
        } else if (a_form->IsArmor()) {
            count = get_inventory_count(a_form, RE::FormType::Armor, player);
        }

        logger::trace("got {} in inventory for item {}"sv, count, a_form->GetName());

        return count;
    }

    std::vector<data_helper*> player::get_hand_assignment(bool a_two_handed) {
        std::vector<data_helper*> data;
        const auto player = RE::PlayerCharacter::GetSingleton();
        auto right_obj = player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        auto left_obj = player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();

        const auto empty_handle = config::mcm_setting::get_empty_hand_setting();

        const auto item = new data_helper();
        item->form = nullptr;
        item->left = false;
        item->type = handle::slot_setting::slot_type::empty;
        item->action_type = empty_handle ? handle::slot_setting::acton_type::un_equip :
                                           handle::slot_setting::acton_type::default_action;
        data.push_back(item);

        const auto item2 = new data_helper();
        item2->form = nullptr;
        item2->left = true;
        item2->type = handle::slot_setting::slot_type::empty;
        item2->action_type = empty_handle ? handle::slot_setting::acton_type::un_equip :
                                            handle::slot_setting::acton_type::default_action;
        data.push_back(item2);

        if (!a_two_handed) {
            a_two_handed = right_obj && util::helper::is_two_handed(right_obj);
        }

        logger::trace("got form {}, name {} on both/right hand"sv,
            right_obj ? string_util::int_to_hex(right_obj->GetFormID()) : "null",
            right_obj ? right_obj->GetName() : "null");

        logger::trace("got form {}, name {} on left hand"sv,
            left_obj ? string_util::int_to_hex(left_obj->GetFormID()) : "null",
            left_obj ? left_obj->GetName() : "null");

        if (a_two_handed && right_obj && left_obj && right_obj->formID == left_obj->formID) {
            data[0]->form = right_obj;
            data[0]->left = false;
            data[0]->type = util::helper::get_type(right_obj);
            data[0]->action_type = handle::slot_setting::acton_type::default_action;
            data.erase(data.begin() + 1);
        }

        if (right_obj) {
            data[0]->form = right_obj;
            data[0]->left = false;
            data[0]->type = util::helper::get_type(right_obj);
            data[0]->action_type = handle::slot_setting::acton_type::default_action;
        }

        if (left_obj) {
            data[1]->form = left_obj;
            data[1]->left = true;
            data[1]->type = util::helper::get_type(left_obj);
            data[1]->action_type = handle::slot_setting::acton_type::default_action;
        }

        logger::trace("got {} items in List now. return."sv, data.size());
        return data;
    }

    void player::look_for_ammo(const bool a_crossbow) {
        bool only_favorite = config::mcm_setting::get_only_favorite_ammo();
        bool sort_by_quantity = config::mcm_setting::get_sort_arrow_by_quantity();
        const auto max_items = config::mcm_setting::get_max_ammunition_type();
        auto player = RE::PlayerCharacter::GetSingleton();
        const auto inv = util::player::get_inventory(player, RE::FormType::Ammo);
        std::multimap<uint32_t, handle::ammo_data*, std::greater<>> ammo_list;
        for (const auto& [item, inv_data] : inv) {
            const auto& [num_items, entry] = inv_data;
            const auto ammo = item->As<RE::TESAmmo>();
            if (!ammo->GetPlayable() || ammo->GetRuntimeData().data.flags.any(RE::AMMO_DATA::Flag::kNonPlayable)) {
                continue;
            }

            if (only_favorite && !entry->IsFavorited()) {
                continue;
            }

            if (a_crossbow && ammo->GetRuntimeData().data.flags.none(RE::AMMO_DATA::Flag::kNonBolt) && num_items != 0) {
                logger::trace("found bolt {}, damage {}, count {}"sv,
                    ammo->GetName(),
                    ammo->GetRuntimeData().data.damage,
                    num_items);
                auto* ammo_data = new handle::ammo_data();
                ammo_data->form = ammo;
                ammo_data->item_count = num_items;
                if (sort_by_quantity) {
                    ammo_list.insert({ static_cast<uint32_t>(num_items), ammo_data });
                } else {
                    ammo_list.insert({ static_cast<uint32_t>(ammo->GetRuntimeData().data.damage), ammo_data });
                }
            } else if (!a_crossbow && num_items != 0 &&
                       ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt)) {
                logger::trace("found arrow {}, damage {}, count {}"sv,
                    ammo->GetName(),
                    ammo->GetRuntimeData().data.damage,
                    num_items);
                auto* ammo_data = new handle::ammo_data();
                ammo_data->form = ammo;
                ammo_data->item_count = num_items;
                if (sort_by_quantity) {
                    ammo_list.insert({ static_cast<uint32_t>(num_items), ammo_data });
                } else {
                    ammo_list.insert({ static_cast<uint32_t>(ammo->GetRuntimeData().data.damage), ammo_data });
                }
            }
        }
        std::vector<handle::ammo_data*> sorted_ammo;
        const auto ammo_handle = handle::ammo_handle::get_singleton();
        for (auto [dmg, data] : ammo_list) {
            sorted_ammo.push_back(data);
            logger::trace("got {} count {}"sv, data->form->GetName(), data->item_count);
            if (sorted_ammo.size() == max_items) {
                break;
            }
        }
        ammo_list.clear();
        ammo_handle->init_ammo(sorted_ammo);
    }

    bool player::has_item_or_spell(RE::TESForm* a_form) {
        auto has_it = false;
        if (!a_form) {
            return has_it;
        }

        //add option to skip check for Items
        auto player = RE::PlayerCharacter::GetSingleton();
        if (a_form->IsWeapon()) {
            has_it = get_inventory_count(a_form, RE::FormType::Weapon, player) > 0;
        } else if (a_form->IsArmor()) {
            has_it = get_inventory_count(a_form, RE::FormType::Armor, player) > 0;
        } else if (a_form->Is(RE::FormType::Light)) {
            has_it = get_inventory_count(a_form, RE::FormType::Light, player) > 0;
        } else if (a_form->Is(RE::FormType::Spell) || a_form->Is(RE::FormType::LeveledSpell)) {
            const auto spell = a_form->As<RE::SpellItem>();
            has_it = player->HasSpell(spell);
        } else if (a_form->Is(RE::FormType::AlchemyItem)) {
            has_it = get_inventory_count(a_form, RE::FormType::AlchemyItem, player) > 0;
        } else if (a_form->Is(RE::FormType::Scroll)) {
            has_it = get_inventory_count(a_form, RE::FormType::Scroll, player) > 0;
        } else if (a_form->Is(RE::FormType::Shout)) {
            const auto shout = a_form->As<RE::TESShout>();
            has_it = has_shout(player, shout);
        }

        logger::trace("Player has item/spell/shout {}, name {}, form {} "sv,
            has_it,
            a_form->GetName(),
            util::string_util::int_to_hex(a_form->formID));

        return has_it;
    }

    uint32_t
        player::get_inventory_count(const RE::TESForm* a_form, RE::FormType a_type, RE::PlayerCharacter*& a_player) {
        auto count = 0;
        auto inventory = get_inventory(a_player, a_type);
        for (const auto& [item, inv_data] : inventory) {
            if (const auto& [num_items, entry] = inv_data; entry->object->formID == a_form->formID) {
                count = num_items;
                break;
            }
        }
        return count;
    }

    bool player::has_shout(RE::Actor* a_actor, RE::TESShout* a_shout) {
        using func_t = decltype(&has_shout);
        REL::Relocation<func_t> func{ offset::has_shout };
        return func(a_actor, a_shout);
    }

}  // util
