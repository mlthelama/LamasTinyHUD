#include "player_hook.h"
#include "processing/set_setting_data.h"

namespace hook {
    void player_hook::install() {
        logger::info("Hooking ..."sv);

        REL::Relocation<std::uintptr_t> player_character_vtbl{ RE::VTABLE_PlayerCharacter[0] };
        add_object_to_container_ = player_character_vtbl.write_vfunc(0x5A, add_object_to_container);
        pick_up_object_ = player_character_vtbl.write_vfunc(0xCC, pick_up_object);
        remove_item_ = player_character_vtbl.write_vfunc(0x56, remove_item);

        logger::info("Hooked."sv);
    }

    void player_hook::add_object_to_container(RE::Actor* a_this,
        RE::TESBoundObject* a_object,
        RE::ExtraDataList* a_extra_list,
        int32_t a_count,
        RE::TESObjectREFR* a_from_refr) {
        add_object_to_container_(a_this, a_object, a_extra_list, a_count, a_from_refr);

        if (a_object->GetPlayable()) {
            if (a_object->IsInventoryObject()) {
                processing::set_setting_data::set_new_item_count_if_needed(a_object, a_count);
            }
        }
    }

    void player_hook::pick_up_object(RE::Actor* a_this,
        RE::TESObjectREFR* a_object,
        uint32_t a_count,
        bool a_arg3,
        bool a_play_sound) {
        pick_up_object_(a_this, a_object, a_count, a_arg3, a_play_sound);

        if (a_object->GetPlayable()) {
            if (const auto obj = a_object->GetBaseObject(); obj->IsInventoryObject()) {
                processing::set_setting_data::set_new_item_count_if_needed(obj, static_cast<int32_t>(a_count));
            }
        }
    }

    RE::ObjectRefHandle player_hook::remove_item(RE::Actor* a_this,
        RE::TESBoundObject* a_item,
        std::int32_t a_count,
        RE::ITEM_REMOVE_REASON a_reason,
        RE::ExtraDataList* a_extra_list,
        RE::TESObjectREFR* a_move_to_ref,
        const RE::NiPoint3* a_drop_loc,
        const RE::NiPoint3* a_rotate) {
        if (a_item->GetPlayable()) {
            if (a_item->IsInventoryObject()) {
                processing::set_setting_data::set_new_item_count_if_needed(a_item, -a_count);
            }
        }

        return remove_item_(a_this, a_item, a_count, a_reason, a_extra_list, a_move_to_ref, a_drop_loc, a_rotate);
    }
}
