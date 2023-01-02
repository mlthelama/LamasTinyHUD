#pragma once

namespace handle {
    class fade_setting {
    public:
        enum class action : std::uint32_t {
            unset = 0,
            in = 1,
            out = 2
        };

        enum class alpha : uint32_t {
            min = 50,
            max = 255
        };

        action action = action::unset;
        alpha alpha = alpha::min;
    };
}
