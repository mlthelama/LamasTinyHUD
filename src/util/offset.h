#pragma once

namespace offset {
    constexpr REL::RelocationID get_right_hand_slot(23151, 23608);
    constexpr REL::RelocationID get_left_hand_slot(23150, 23607);
    constexpr REL::RelocationID get_voice_slot(23153, 23610);  //se might be wrong
    constexpr REL::RelocationID get_flash_hud(51907, 52845);
    constexpr REL::RelocationID get_un_equip_spell(227784, 54669);
    constexpr REL::RelocationID get_un_equip_shout(53863, 54664);

    using t_flash_hud_menu_meter = void(_fastcall*)(RE::ActorValue a_actor_value);
    static REL::Relocation<t_flash_hud_menu_meter> flash_hud_menu_meter{ get_flash_hud };
}
