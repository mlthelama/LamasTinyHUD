#pragma once

namespace papyrus {
    class hud_mcm {
    public:
        static void on_config_close(RE::TESQuest*);
        static std::vector<RE::BSFixedString> get_selected_options(RE::TESQuest*, uint32_t a_id);
        static uint32_t get_form_id_for_selection(RE::TESQuest*, uint32_t a_index);

        static bool Register(RE::BSScript::IVirtualMachine* a_vm);
    };

    void Register();
}
