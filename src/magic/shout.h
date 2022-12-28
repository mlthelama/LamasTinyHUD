#pragma once
namespace magic {
    class shout {
    public:
        static std::vector<RE::TESShout*> get_shouts();
        static void equip_shout(RE::TESForm* a_form);
    };
}
