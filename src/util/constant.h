#pragma once
namespace util {
    constexpr int32_t const_log_trace = 0;
    constexpr int32_t const_log_debug = 1;
    constexpr int32_t const_log_info = 2;

    constexpr auto dynamic_name = "dynamic";
    constexpr auto delimiter = "|";

    const std::string ini_path = "Data\\SKSE\\Plugins\\";  // NOLINT(modernize-raw-string-literal)
    const std::string ini_default_name = "LamasTinyHUD_Custom";
    const std::string ini_elden_name = "LamasTinyHUD_Custom_Elden";
    const std::string ini_ending = ".ini";

    constexpr RE::FormID unarmed = 0x000001F4;
}
