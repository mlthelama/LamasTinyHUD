#include "sink_event.h"

#include "inventory_event.h"
#include "key_manager.h"

namespace event {
    void sink_events() {
        key_manager::sink();
        inventory_event::sink();

        logger::info("added all sinks.");
    }
}
