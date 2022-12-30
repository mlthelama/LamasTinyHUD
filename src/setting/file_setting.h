#pragma once

namespace config {
    class file_setting {
    public:
        static void load_setting();
        
        static int get_log_level();
        static float get_extra_size_for_image();
    };
}
