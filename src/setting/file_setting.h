#pragma once

namespace config {
    class file_setting {
    public:
        static void load_setting();
        static int get_log_level();
    };
}
