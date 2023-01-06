#pragma once
#include "key_position.h"

namespace handle {
    class set_data {
    public:
        static void set_slot_data();

    private:
        static void set_empty_slot(int a_pos, key_position*& a_key_pos);
    };
}
