#pragma once
#include "handle/page/position_setting.h"

namespace handle {
    class key_position_handle {
    public:
        static key_position_handle* get_singleton();
        void init_key_position_map();
        void set_position_lock(position_setting::position a_position, uint32_t a_locked);

        [[nodiscard]] position_setting::position get_position_for_key(uint32_t a_key) const;
        [[nodiscard]] uint32_t get_key_for_position(position_setting::position a_pos) const;
        [[nodiscard]] bool is_position_locked(position_setting::position a_position) const;


        key_position_handle(const key_position_handle&) = delete;
        key_position_handle(key_position_handle&&) = delete;

        key_position_handle& operator=(const key_position_handle&) const = delete;
        key_position_handle& operator=(key_position_handle&&) const = delete;

    private:
        key_position_handle()
            : data_(nullptr) {}

        ~key_position_handle() = default;

        struct key_position_handle_data {
            std::map<uint32_t, position_setting::position> key_position_map;
            std::map<position_setting::position, uint32_t> position_key_map;
            std::map<position_setting::position, uint32_t> position_lock_map;
        };

        key_position_handle_data* data_;
    };
}
