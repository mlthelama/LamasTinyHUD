#pragma once
#include "fade_setting.h"
#include "offset_setting.h"
#include "slot_setting.h"

namespace handle {
    class page_setting {
    public:
        enum class position : std::uint32_t { top = 0, right = 1, down = 2, left = 3, total = 4 };

        position pos;
        fade_setting* fade_setting = nullptr;
        std::vector<slot_setting*> slot_settings;
        offset_setting* offset_setting = nullptr;
    };
}
