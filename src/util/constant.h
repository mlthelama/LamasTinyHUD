#pragma once
namespace util {
    constexpr int32_t const_log_trace = 0;
    constexpr int32_t const_log_debug = 1;
    constexpr int32_t const_log_info = 2;

    constexpr auto empty_enum_string = "$LamasTinyHUD_ENUM"sv;

    enum class selection_type {
        item = 0,
        magic = 1,
        shout = 2,
        power = 3,
        weapon = 4
    };
}
