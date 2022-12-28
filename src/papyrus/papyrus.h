#pragma once
#include "util/constant.h"

namespace papyrus {
    class hud_mcm {
    public:
        static void on_config_close(RE::TESQuest*);
        static std::vector<RE::BSFixedString> get_selected_options(RE::TESQuest*, uint32_t a_id);
        static uint32_t get_form_id_for_selection(RE::TESQuest*, uint32_t a_index);

        static bool Register(RE::BSScript::IVirtualMachine* a_vm);
    private:
        inline static auto inventory_data_list_ = new std::vector<RE::InventoryEntryData>;
        inline static auto shout_data_list_ = new std::vector<RE::TESShout*>;
        inline static auto spell_data_list_ = new std::vector<RE::SpellItem*>;
        inline static auto power_data_list_ = new std::vector<RE::SpellItem*>;
        inline static auto weapon_data_list_ = new std::vector<RE::InventoryEntryData>;
        inline static util::selection_type index_ = util::selection_type::unset;

        static void clear_list();
        static bool is_size_ok(uint32_t a_idx, uint64_t a_size);
    };

    void Register();
}
