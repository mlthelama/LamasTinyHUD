#pragma once
#include "handle/page/slot_setting.h"

namespace item {
    class potion {
    public:
        static void consume_potion(handle::slot_setting*& a_slot, RE::PlayerCharacter*& a_player);
    };
}
