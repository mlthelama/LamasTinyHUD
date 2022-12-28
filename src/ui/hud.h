#pragma once

namespace ui {
    class hud {
        static void draw_ui();
        static void callback(SKSE::MessagingInterface::Message* a_msg);

        //static inline bool show_hud_ = false;
        static inline bool open_ = false;

    public:
        hud() = delete;

        static bool install();

        static float get_resolution_scale_width();
        static float get_resolution_scale_height();

        static bool is_open();
    };
}
