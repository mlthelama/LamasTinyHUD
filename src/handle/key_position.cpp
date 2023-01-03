#include "key_position.h"
#include "setting/mcm_setting.h"

namespace handle {
    using mcm = config::mcm_setting;
    using position = page_setting::position;

    key_position* key_position::get_singleton() {
        static key_position singleton;
        return std::addressof(singleton);
    }

    void key_position::init_key_position_map() {
        logger::trace("init key position map ..."sv);
        if (!this->data_) {
            this->data_ = new key_position_data();
        }


        data_->key_position_map[mcm::get_top_action_key()] = position::top;
        data_->key_position_map[mcm::get_right_action_key()] = position::right;
        data_->key_position_map[mcm::get_bottom_action_key()] = position::down;
        data_->key_position_map[mcm::get_left_action_key()] = position::left;
        logger::trace("done with init of position key map."sv);
    }

    page_setting::position key_position::get_position_for_key(const uint32_t a_key) const {
        if (const key_position_data* data = this->data_;
            data && !data_->key_position_map.empty() && data_->key_position_map.contains(a_key)) {
            const auto pos = data_->key_position_map[a_key];
            logger::trace("got position {} for key {}"sv, static_cast<uint32_t>(pos), a_key);
            return pos;
        }
        return position::total;
    }
}
