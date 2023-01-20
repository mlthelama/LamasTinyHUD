#pragma once
#include "draw_setting.h"
#include "hud_offset_setting.h"
#include "slot_setting.h"
#include "transparency_setting.h"
#include "ui/image_path.h"

namespace handle {
    class position_setting {
    public:
        enum class position : std::uint32_t { top = 0, right = 1, bottom = 2, left = 3, total = 4 };

        uint32_t page = 0;
        position pos = position::total;
        //fade_setting* fade_setting = nullptr;
        std::vector<slot_setting*> slot_settings;
        hud_offset_setting* offset_setting = nullptr;
        ui::icon_image_type icon_type = ui::icon_image_type::icon_default;
        uint32_t button_press_modify = ui::draw_full;
        uint32_t key = 0;
        transparency_setting* transparency_setting = nullptr;
        draw_setting* draw_setting = nullptr;
        float font_size = 0.f;
        //slot_setting::slot_type slot_type = slot_setting::slot_type::empty;
    };
}
