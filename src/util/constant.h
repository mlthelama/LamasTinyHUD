#pragma once
namespace util {
    constexpr int32_t const_log_trace = 0;
    constexpr int32_t const_log_debug = 1;
    constexpr int32_t const_log_info = 2;

    constexpr auto empty_enum_string = "$LamasTinyHUD_ENUM"sv;

    constexpr auto hud_image_path = R"(.\Data\SKSE\Plugins\img\hud.png)";
    constexpr auto round_image_path = R"(.\Data\SKSE\Plugins\img\round.png)";
    constexpr auto key_image_path = R"(.\Data\SKSE\Plugins\img\key.png)";

    constexpr auto hud_name = "lamas_tiny_hud";

    enum class selection_type { unset = -1, weapon = 0, magic = 1, shout = 2, power = 3, item = 4 };

    enum class image_type { hud = 0, round = 1, key = 2, total = 3 };

    inline static std::map<image_type, const char*> image_type_path_map = { { image_type::hud, hud_image_path },
                                                                            { image_type::round, round_image_path },
                                                                            { image_type::key, key_image_path } };
}
