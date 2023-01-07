#pragma once
#include "hud_offset_setting.h"
#include "slot_setting.h"
#include "ui/image_path.h"

namespace handle {
    class page_setting {
    public:
        enum class position : std::uint32_t { top = 0, right = 1, bottom = 2, left = 3, total = 4 };

        position pos;
        //fade_setting* fade_setting = nullptr;
        std::vector<slot_setting*> slot_settings;
        offset_setting* offset_setting = nullptr;
        ui::icon_image_type icon_type = ui::icon_image_type::icon_default;
        uint32_t icon_opacity = ui::draw_full;
        uint32_t button_press_modify = ui::draw_full;
        uint32_t key = 0;
    };
}
