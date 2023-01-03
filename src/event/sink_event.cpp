#include "sink_event.h"
#include "key_manager.h"

namespace event {
    void sink_events() {
        key_manager::sink();

        logger::info("added all sinks.");
    }
}
