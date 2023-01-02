#pragma once
#include "page/page_setting.h"
#include "page/slot_setting.h"

namespace handle {
    class setting_execute {
    public:
        static void execute_setting(const slot_setting* a_slot);

        static page_setting* get_page_setting_for_key(uint32_t a_key);
    };
}
