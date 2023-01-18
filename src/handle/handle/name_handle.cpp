#include "name_handle.h"
#include "handle/data/data_helper.h"
#include "util/constant.h"
#include "util/string_util.h"

namespace handle {
    name_handle* name_handle::get_singleton() {
        static name_handle singleton;
        return std::addressof(singleton);
    }

    void name_handle::init_names(const std::vector<data_helper*>& data_helpers) {
        if (!this->data_) {
            this->data_ = new name_handle_data();
        }

        name_handle_data* data = this->data_;
        std::string help_string;

        data->name = "<Empty>";

        std::string name_right = "<Empty>";
        if (!data_helpers.empty()) {
            if (data_helpers[0]->form) {
                name_right = data_helpers[0]->form->GetName();
            }
            data->name = name_right;
        }

        if (data_helpers.size() == 2) {
            std::string name_left = "<Empty>";
            if (data_helpers[1]->form) {
                name_left = data_helpers[1]->form->GetName();
            }
            data->name = fmt::format("{} {} {}", name_left, util::delimiter, name_right);
        }
    }

    std::string name_handle::get_item_name_string() const {
        if (const name_handle_data* data = this->data_; data) {
            return data->name;
        }
        return {};
    }
}
