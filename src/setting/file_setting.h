#pragma once

namespace config {
    class file_setting {
    public:
        static void load_setting();

        static int get_log_level();
        static bool get_draw_key_background();
    };
}
