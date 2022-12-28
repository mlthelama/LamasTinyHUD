#pragma once
namespace magic {
    class power {
    public:
        static std::vector<RE::SpellItem*> get_powers();
        static void equip_power(RE::TESForm* a_form);
    };
}
