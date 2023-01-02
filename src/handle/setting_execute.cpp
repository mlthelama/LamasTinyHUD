#include "setting_execute.h"

#include "key_position.h"
#include "page_handle.h"
#include "item/potion.h"
#include "item/weapon.h"
#include "magic/power.h"
#include "magic/shout.h"
#include "magic/spell.h"
#include "setting/mcm_setting.h"

namespace handle {
    using mcm = config::mcm_setting;

    void setting_execute::execute_setting(const slot_setting* a_slot) {
        switch (a_slot->type) {
            case util::selection_type::unset:
                logger::warn("nothing to do, nothing set"sv);
                break;
            case util::selection_type::item:
                item::potion::consume_potion(a_slot->form);
                break;
            case util::selection_type::magic:
                magic::spell::instant_cast(a_slot->form);
                break;
            case util::selection_type::shout:
                magic::shout::equip_shout(a_slot->form);
                break;
            case util::selection_type::power:
                //make a setting in mcm for equip or instant cast
                magic::power::equip_power(a_slot->form);
            //magic::spell::instant_cast(top_handle);
                break;
            case util::selection_type::weapon:
                item::weapon::equip_weapon(a_slot->form);
                break;
        }
    }

    page_setting* setting_execute::get_page_setting_for_key(const uint32_t a_key) {
        const auto position = key_position::get_singleton()->get_position_for_key(a_key);
        if (position == page_setting::position::total) {
            logger::warn("nothing to do, nothing set. return."sv);
            return nullptr;
        }

        const auto page_setting = page_handle::get_singleton()->get_page_setting(position);
        if (page_setting == nullptr) {
            logger::warn("nothing to do, nothing set. return."sv);
        }
        logger::debug("position is {}, setting count {}"sv,
            static_cast<uint32_t>(position),
            page_setting->slot_settings.size());

        return page_setting;
    }
}
