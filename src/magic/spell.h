#pragma once
namespace magic {
    class spell {
    public:
        static std::vector<RE::TESForm*> get_spells();

        static void instant_cast(RE::TESForm* a_form);
    };
}
