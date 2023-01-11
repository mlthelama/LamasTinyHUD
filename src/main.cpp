#include "hook/hook.h"
#include "papyrus/papyrus.h"
#include "setting/file_setting.h"
#include "setting/mcm_setting.h"
#include "ui/ui_renderer.h"
#include "util/constant.h"

void init_logger() {
    if (static bool initialized = false; !initialized) {
        initialized = true;
    } else {
        return;
    }

    try {
#ifndef NDEBUG
        auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
        auto path = logger::log_directory();
        if (!path) {
            stl::report_and_fail("failed to get standard log path"sv);
        }

        *path /= fmt::format("{}.log"sv, Version::PROJECT);
        auto sink = make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif
        auto log = make_shared<spdlog::logger>("global log"s, move(sink));

#ifndef NDEBUG
        log->set_level(spdlog::level::trace);
#else
        log->set_level(spdlog::level::trace);
        log->flush_on(spdlog::level::trace);
#endif

        set_default_logger(move(log));
        spdlog::set_pattern("[%H:%M:%S.%f] %s(%#) [%^%l%$] %v"s);

        logger::info("{} v{}"sv, Version::PROJECT, Version::NAME);

        try {
            config::file_setting::load_setting();
            config::mcm_setting::read_setting();
        } catch (const std::exception& e) {
            logger::warn("failed to load setting {}"sv, e.what());
        }

        switch (config::file_setting::get_log_level()) {
            case util::const_log_trace:
                spdlog::set_level(spdlog::level::trace);
                spdlog::flush_on(spdlog::level::trace);
                break;
            case util::const_log_debug:
                spdlog::set_level(spdlog::level::debug);
                spdlog::flush_on(spdlog::level::debug);
                break;
            case util::const_log_info:
                spdlog::set_level(spdlog::level::info);
                spdlog::flush_on(spdlog::level::info);
                break;
            default:
                spdlog::set_level(spdlog::level::trace);
                spdlog::flush_on(spdlog::level::trace);
                break;
        }
    } catch (const std::exception& e) {
        logger::critical("failed, cause {}"sv, e.what());
    }
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse) {
#ifndef NDEBUG
    while (!IsDebuggerPresent()) {};
#endif


    init_logger();

    logger::info("{} loading"sv, Version::PROJECT);

    Init(a_skse);

    /*SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kDataLoaded:
                logger::info("Data loaded"sv);

                logger::info("Done with adding"sv);
                break;
        }
    });*/

    papyrus::Register();
    ui::ui_renderer::install();
    hook::hook::install();

    logger::info("{} loaded"sv, Version::PROJECT);
    return true;
}

EXTERN_C [[maybe_unused]] __declspec(dllexport) constinit auto SKSEPlugin_Version = []() noexcept {
    SKSE::PluginVersionData v;
    v.PluginName(Version::PROJECT.data());
    v.AuthorName(Version::AUTHOR);
    v.PluginVersion({ Version::MAJOR, Version::MINOR, Version::PATCH, Version::BETA });
    v.UsesAddressLibrary(true);
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
    v.UsesStructsPost629(true);
    return v;
}();

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI
SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* pluginInfo) {
    pluginInfo->name = SKSEPlugin_Version.pluginName;
    pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
    pluginInfo->version = SKSEPlugin_Version.pluginVersion;


    if (a_skse->IsEditor()) {
        logger::critical("Loaded in editor, marking as incompatible"sv);
        return false;
    }

    const auto ver = a_skse->RuntimeVersion();
    if (ver < SKSE::RUNTIME_SSE_1_5_39) {
        logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
        return false;
    }

    return true;
}
