#pragma once
#include "data/data_helper.h"

namespace handle {
    class name_handle {
    public:
        static name_handle* get_singleton();
        void init_names(const std::vector<data_helper*>& data_helpers);

        [[nodiscard]] std::string get_item_name_string() const;

        name_handle(const name_handle&) = delete;
        name_handle(name_handle&&) = delete;

        name_handle& operator=(const name_handle&) const = delete;
        name_handle& operator=(name_handle&&) const = delete;

    private:
        name_handle()
            : data_(nullptr) {}

        ~name_handle() = default;

        struct name_handle_data {
            std::string name;
        };

        name_handle_data* data_;
    };
}
