#include "read_data.h"

#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "handle/slot_setting_handle.h"

namespace handle {
    void read_data::read_config_and_set_data() {
        logger::trace("Reading mcm config ..."sv);
        config::mcm_setting::read_setting();

        logger::trace("Setting handlers ..."sv);
        if (config::mcm_setting::get_selected_top_item_form() > 0) {
            slot_setting_handle::get_singleton()->init_top_setting(
                RE::TESForm::LookupByID(config::mcm_setting::get_selected_top_item_form()),
                static_cast<util::selection_type>(config::mcm_setting::get_top_type()));
        }

    }
}
