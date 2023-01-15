#pragma once
#include "ui/image_path.h"

namespace handle {
    class transparency_setting {
    public:
        uint32_t background_transparency = ui::draw_full;
        uint32_t background_icon_transparency = ui::draw_full;
        uint32_t icon_transparency = ui::draw_full;
        uint32_t key_transparency = ui::draw_full;
        uint32_t text_transparency = ui::draw_full;
    };
}
