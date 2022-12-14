#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#pragma warning(push)
#include <spdlog/sinks/basic_file_sink.h>
#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

namespace stl {
    using namespace SKSE::stl;

    constexpr std::string_view safe_string(const char* a_str) { return a_str ? a_str : ""sv; }

    template <class T>
    void write_thunk_call() {
        auto& trampoline = SKSE::GetTrampoline();
        const REL::Relocation<std::uintptr_t> hook{ T::id, T::offset };
        T::func = trampoline.write_call<5>(hook.address(), T::thunk);
    }
}

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)
#define EXTERN_C extern "C"

#include "Version.h"
