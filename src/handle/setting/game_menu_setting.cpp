#include "game_menu_setting.h"
#include "set_setting_data.h"
#include <handle/handle/page_handle.h>
#include <setting/mcm_setting.h>
#include <util/helper.h>

void handle::game_menu_setting::elden_souls_config(RE::TESForm* a_form,
    handle::position_setting::position_type a_position) {
    std::vector<data_helper*> data;

    const auto pos_max = handle::page_handle::get_singleton()->get_highest_page_id_position(a_position);
    auto max = config::mcm_setting::get_max_page_count() - 1;  //we start at 0 so count -1
    logger::trace("Max for Position {} is {}, already set before edit {}"sv,
        static_cast<uint32_t>(a_position),
        max,
        pos_max);
    if (pos_max != -1) {
        max = config::mcm_setting::get_max_page_count() - pos_max;
    }

    //check for replace mode here
    if (data.size() == max || max == 0) {
        util::helper::write_notification(fmt::format("Can not add more Items to Position", max));
        logger::trace("Max is 0, can not add anymore, return.");
        return;
    }

    const auto check_duplicates = config::mcm_setting::get_check_duplicate_items();

    const auto item = util::helper::is_suitable_for_position(a_form, a_position);
    if (item->form || (a_form && item->actor_value != RE::ActorValue::kNone)) {
        if (check_duplicates && util::helper::already_used(a_form, a_position, data)) {
            auto log_string = fmt::format("Item {} already used in that position", a_form ? a_form->GetName() : "null");
            util::helper::write_notification(log_string);
            logger::trace("{}. return."sv, log_string);  //well
            return;
        } else {
            util::helper::write_notification(fmt::format("Added Item {}", a_form ? a_form->GetName() : "null"));
            data.push_back(item);
        }
    } else {
        if (a_form && !a_form->Is(RE::FormType::Enchantment)) {
            util::helper::write_notification(fmt::format("Ignored Item {}, because it did not fit the requirement",
                a_form ? a_form->GetName() : "null"));
        }
    }

    logger::trace("Size is {}. calling to set data now."sv, data.size());

    //use set_single_slot for replacement handling
    handle::set_setting_data::set_queue_slot(a_position, data);

    logger::trace("Setting done. return.");
}
