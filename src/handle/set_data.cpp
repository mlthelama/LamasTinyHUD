#include "set_data.h"
#include "key_position.h"
#include "page_handle.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"

namespace handle {
    using mcm = config::mcm_setting;

    void set_data::set_slot_data() {
        logger::trace("Setting handlers ..."sv);

        //set empty for each position, it will be overwritten if it is configured
        for (auto i = 0; i < static_cast<int>(page_setting::position::total); ++i) {
            logger::trace("setting empty config for position {}"sv, i);
            page_handle::get_singleton()->init_page(0,
                static_cast<page_setting::position>(i),
                nullptr,
                util::selection_type::unset,
                mcm::get_hud_slot_position_offset(),
                mcm::get_hud_key_position_offset(),
                slot_setting::acton_type::default_action);
        }

        if (const auto form = RE::TESForm::LookupByID(mcm::get_selected_top_item_form());
            form != nullptr) {
            page_handle::get_singleton()->init_page(0,
                page_setting::position::top,
                form,
                static_cast<util::selection_type>(mcm::get_top_type()),
                config::mcm_setting::get_hud_slot_position_offset(),
                config::mcm_setting::get_hud_key_position_offset(),
                static_cast<slot_setting::acton_type>(mcm::get_top_slot_action()));
        } else {
            page_handle::get_singleton()->init_page(0,
                page_setting::position::top,
                nullptr,
                util::selection_type::unset,
                config::mcm_setting::get_hud_slot_position_offset(),
                config::mcm_setting::get_hud_key_position_offset(),
                slot_setting::acton_type::default_action);
            logger::warn("form for top type is null. remove the setting."sv);
        }

        key_position::get_singleton()->init_key_position_map();

        logger::trace("done setting. return."sv);
    }
}
