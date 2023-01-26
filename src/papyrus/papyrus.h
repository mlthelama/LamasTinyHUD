#pragma once

namespace papyrus {
    class hud_mcm {
    public:
        static void on_config_close(RE::TESQuest*);
        static RE::BSFixedString get_resolution_width(RE::TESQuest*);
        static RE::BSFixedString get_resolution_height(RE::TESQuest*);

        static std::vector<RE::BSFixedString> get_section_names(RE::TESQuest*, uint32_t a_position);
        static RE::BSFixedString get_page(RE::TESQuest*, uint32_t a_index, uint32_t a_position);
        static RE::BSFixedString get_position(RE::TESQuest*, uint32_t a_index, uint32_t a_position);
        static uint32_t get_selection_type(RE::TESQuest*, uint32_t a_index, bool a_left, uint32_t a_position);
        static RE::BSFixedString get_form_string(RE::TESQuest*, uint32_t a_index, bool a_left, uint32_t a_position);
        static uint32_t get_slot_action(RE::TESQuest*, uint32_t a_index, bool a_left, uint32_t a_position);
        static uint32_t get_hand_selection(RE::TESQuest*, uint32_t a_index, uint32_t a_position);
        static RE::BSFixedString get_form_name(RE::TESQuest*, uint32_t a_index, bool a_left, uint32_t a_position);
        static void reset_section(RE::TESQuest*, uint32_t a_index, uint32_t a_position);
        static void set_action_value(RE::TESQuest*, uint32_t a_index, bool a_left, uint32_t a_value, uint32_t a_position);
        static void init_config_for_position(RE::TESQuest*, uint32_t a_position);

        static bool Register(RE::BSScript::IVirtualMachine* a_vm);

    private:
        static bool is_size_ok(uint32_t a_idx, uint64_t a_size);
        static std::string get_section_by_index(uint32_t a_index, uint32_t a_position);
    };

    void Register();
}
