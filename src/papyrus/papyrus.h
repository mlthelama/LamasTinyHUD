#pragma once

namespace papyrus {
    class hud_mcm {
    public:
        static void on_config_close(RE::TESQuest*);
        //static void force_page_reset(RE::TESQuest*);

        static bool Register(RE::BSScript::IVirtualMachine* a_vm);
    };

    void Register();
}
