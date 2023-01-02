#pragma once
#include "page/page_setting.h"

namespace handle {
    class key_position {
    public:
        static key_position* get_singleton();
        void init_key_position_map();
        [[nodiscard]] page_setting::position get_position_for_key(uint32_t a_key) const;

        key_position(const key_position&) = delete;
        key_position(key_position&&) = delete;

        key_position& operator=(const key_position&) const = delete;
        key_position& operator=(key_position&&) const = delete;

    private:
        key_position()
            : data_(nullptr) {}

        ~key_position() = default;

        struct key_position_data {
            std::map<uint32_t, page_setting::position> key_position_map;
        };

        key_position_data* data_;
    };
}
