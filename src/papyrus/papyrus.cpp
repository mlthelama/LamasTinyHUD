#include "papyrus.h"
#include "setting/mcm_setting.h"

namespace papyrus
{
    static const char* mcm_name = "lamas_tiny_hud_mcm";
    void hud_mcm::on_config_close(RE::TESQuest*)
    {
        config::mcm_setting::read_setting();
    }

    bool hud_mcm::Register(RE::BSScript::IVirtualMachine* a_vm)
    {
        a_vm->RegisterFunction("OnConfigClose", mcm_name, on_config_close);

        logger::info("Registered {} class", mcm_name);
        return true;
    }

    void Register()
    {
        const auto papyrus = SKSE::GetPapyrusInterface();
        papyrus->Register(hud_mcm::Register);
        logger::info("Registered papyrus functions");
    }
}
