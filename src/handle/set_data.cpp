#include "set_data.h"

#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "handle/slot_setting_handle.h"

namespace handle {
    void set_data::set_slot_data() {
        logger::trace("Setting handlers ..."sv);
        if (const auto form = RE::TESForm::LookupByID(config::mcm_setting::get_selected_top_item_form());
            form != nullptr) {
            slot_setting_handle::get_singleton()->init_top_setting(form,
                static_cast<util::selection_type>(config::mcm_setting::get_top_type()));
        } else {
            logger::warn("form for top type is null. skip."sv);
        }
        logger::trace("done setting. return."sv);
    }
}
