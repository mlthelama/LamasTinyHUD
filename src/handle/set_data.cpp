#include "set_data.h"
#include "key_position.h"
#include "page_handle.h"
#include "data/data_helper.h"
#include "setting/mcm_setting.h"
#include "util/constant.h"
#include "util/string_util.h"

namespace handle {
    using mcm = config::mcm_setting;

    void set_data::set_slot_data() {
        logger::trace("Setting handlers ..."sv);

        //set empty for each position, it will be overwritten if it is configured
        for (auto i = 0; i < static_cast<int>(page_setting::position::total); ++i) {
            set_empty_slot(i);
        }

        logger::trace("continue with overwriting data from configuration ..."sv);

        if (const auto form = RE::TESForm::LookupByID(mcm::get_selected_top_item_form());
            form != nullptr) {
            const auto top_type = static_cast<util::selection_type>(mcm::get_top_type());
            auto hand = static_cast<slot_setting::hand_equip>(mcm::get_top_hand_selection());
            auto action = static_cast<slot_setting::acton_type>(mcm::get_top_slot_action());
            if (top_type != util::selection_type::magic && top_type != util::selection_type::weapon) {
                hand = slot_setting::hand_equip::total;
            }
            logger::trace("start building data form {}, type {}, action {}, hand {}"sv,
                util::string_util::int_to_hex(form),
                static_cast<int>(top_type),
                static_cast<uint32_t>(action),
                static_cast<uint32_t>(hand));

            std::vector<data_helper*> data;
            const auto item = new data_helper();
            item->form = form;
            item->type = top_type;
            item->action_type = action;
            item->left = false;
            data.push_back(item);

            logger::trace("checking if we need to build a second data set"sv);

            if ((top_type == util::selection_type::magic || top_type == util::selection_type::weapon) && hand ==
                slot_setting::hand_equip::single) {
                //check for the left hand setting
                // check if it is the same as the other hand, if not, overwrite
                logger::trace("we do, now check if the setting is usable"sv);
                const auto top_type_left = static_cast<util::selection_type>(mcm::get_top_type_left());
                if (const auto form_left = RE::TESForm::LookupByID(mcm::get_selected_top_item_form_left());
                    form_left != nullptr && (top_type_left == util::selection_type::magic || top_type_left ==
                                             util::selection_type::weapon)) {
                    auto action_left = static_cast<slot_setting::acton_type>(mcm::get_top_slot_action_left());
                    //TODO maybe check if instant cast is set, and if the other is a weapon, then default to equip
                    if (action != action_left) {
                        logger::warn("action type {} differ from action type left {}, setting both to {}"sv,
                            static_cast<int>(action),
                            static_cast<int>(action_left),
                            static_cast<int>(action));
                        action_left = action;
                    }

                    logger::trace("start building data form {}, type {}, action {}"sv,
                        util::string_util::int_to_hex(form_left),
                        static_cast<int>(top_type_left),
                        static_cast<uint32_t>(action_left));

                    const auto item_left = new data_helper();
                    item_left->form = form_left;
                    item_left->type = top_type_left;
                    item_left->action_type = action_left;
                    item_left->left = true;
                    data.push_back(item_left);
                }
            }

            logger::trace("build data, calling handler."sv);

            page_handle::get_singleton()->init_page(0,
                page_setting::position::top,
                data,
                config::mcm_setting::get_hud_slot_position_offset(),
                config::mcm_setting::get_hud_key_position_offset(),
                hand,
                config::mcm_setting::get_icon_opacity());
        } else {
            set_empty_slot(static_cast<int>(page_setting::position::top));
            logger::warn("form for top type is null. remove the setting."sv);
        }

        key_position::get_singleton()->init_key_position_map();

        logger::trace("done setting. return."sv);
    }

    void set_data::set_empty_slot(int a_pos) {
        logger::trace("setting empty config for position {}"sv, a_pos);
        std::vector<data_helper*> data;
        const auto item = new data_helper();
        item->form = nullptr;
        item->action_type = slot_setting::acton_type::default_action;
        item->type = util::selection_type::unset;
        data.push_back(item);

        page_handle::get_singleton()->init_page(0,
            static_cast<page_setting::position>(a_pos),
            data,
            mcm::get_hud_slot_position_offset(),
            mcm::get_hud_key_position_offset(),
            slot_setting::hand_equip::total,
            config::mcm_setting::get_icon_opacity());
    }
}
