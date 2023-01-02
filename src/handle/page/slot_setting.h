#pragma once
#include "util/constant.h"

namespace handle {
    class slot_setting {
    public:
        enum class action : std::uint32_t { unset = 0, instant = 1, dual = 2 };

        RE::TESForm* form = nullptr;
        util::selection_type type = util::selection_type::unset;
        action action = action::unset;
    };
}
