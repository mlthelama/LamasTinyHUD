#include "sink_event.h"
#include "equip_event.h"
#include "key_manager.h"
#include "menu_manager.h"

namespace event {
    void sink_events() {
        key_manager::sink();
        equip_event::sink();
        menu_manager::sink();
        
        logger::info("added all sinks.");
    }
}
