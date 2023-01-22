#include "key_position_handle.h"
#include "setting/mcm_setting.h"

namespace handle {
    using mcm = config::mcm_setting;
    using position = position_setting::position_type;

    key_position_handle* key_position_handle::get_singleton() {
        static key_position_handle singleton;
        return std::addressof(singleton);
    }

    void key_position_handle::init_key_position_map() {
        logger::trace("init key position map ..."sv);
        if (!this->data_) {
            this->data_ = new key_position_handle_data();
        }

        data_->key_position_map[mcm::get_top_action_key()] = position::top;
        data_->key_position_map[mcm::get_right_action_key()] = position::right;
        data_->key_position_map[mcm::get_bottom_action_key()] = position::bottom;
        data_->key_position_map[mcm::get_left_action_key()] = position::left;

        data_->position_key_map[position::top] = mcm::get_top_action_key();
        data_->position_key_map[position::right] = mcm::get_right_action_key();
        data_->position_key_map[position::bottom] = mcm::get_bottom_action_key();
        data_->position_key_map[position::left] = mcm::get_left_action_key();


        logger::trace("done with init of position key map."sv);
    }

    position_setting::position_type key_position_handle::get_position_for_key(const uint32_t a_key) const {
        if (const key_position_handle_data* data = this->data_;
            data && !data->key_position_map.empty() && data->key_position_map.contains(a_key)) {
            const auto pos = data->key_position_map.at(a_key);
            logger::trace("got position {} for key {}"sv, static_cast<uint32_t>(pos), a_key);
            return pos;
        }
        return position::total;
    }

    uint32_t key_position_handle::get_key_for_position(const position_setting::position_type a_position) const {
        if (const key_position_handle_data* data = this->data_;
            data && !data->position_key_map.empty() && data->position_key_map.contains(a_position)) {
            const auto key = data->position_key_map.at(a_position);
            logger::trace("got key {} for position {}"sv, key, static_cast<uint32_t>(a_position));
            return key;
        }
        return 0;
    }
}
