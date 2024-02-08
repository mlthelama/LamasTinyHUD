#pragma once

namespace setting {
    class display_tweaks {
    public:
        static void load_setting();
        
        static float get_resolution_scale();
        static bool get_borderless_upscale();
    };
}  // config
