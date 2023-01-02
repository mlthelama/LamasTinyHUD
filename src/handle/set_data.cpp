#include "set_data.h"

#include "page_handle.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"

namespace handle {
    void set_data::set_slot_data() {
        logger::trace("Setting handlers ..."sv);
        if (const auto form = RE::TESForm::LookupByID(config::mcm_setting::get_selected_top_item_form());
            form != nullptr) {
            page_handle::get_singleton()->init_page(0,
                page_setting::position::top,
                form,
                static_cast<util::selection_type>(config::mcm_setting::get_top_type()));
        } else {
            page_handle::get_singleton()->init_page(0,
                page_setting::position::top,
                nullptr,
                util::selection_type::unset);
            logger::warn("form for top type is null. remove the setting."sv);
        }
        logger::trace("done setting. return."sv);
    }
}
