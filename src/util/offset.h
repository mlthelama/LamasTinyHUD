#pragma once

namespace offset {
    constexpr REL::RelocationID get_right_hand_slot(23151, 23608);
    constexpr REL::RelocationID get_left_hand_slot(23150, 23607);
    constexpr REL::RelocationID get_flash_hud(51907, 52845);


    typedef void (_fastcall* t_flash_hud_menu_meter)(RE::ActorValue a_actor_value);
    static REL::Relocation<t_flash_hud_menu_meter> flash_hud_menu_meter{ get_flash_hud };
}
