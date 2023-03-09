#include "hook.h"
#include "hook/menu_hook.h"
#include "hook/player_hook.h"

namespace hook {

    void hook::install() {
        player_hook::install();
        menu_hook::install();
    }
}  // hook
