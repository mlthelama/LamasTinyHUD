#pragma once

namespace magic {
    class shout {
    public:
        //not used
        static std::vector<RE::TESForm*> get_shouts();
        static void equip_shout(RE::TESForm* a_form, RE::PlayerCharacter*& a_player);
    };
}
