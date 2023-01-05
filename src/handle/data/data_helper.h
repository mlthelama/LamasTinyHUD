#pragma once
#include "handle/page/slot_setting.h"
#include "util/constant.h"

class data_helper {
public:
    RE::TESForm* form = nullptr;
    util::selection_type type = util::selection_type::unset;
    handle::slot_setting::acton_type action_type = handle::slot_setting::acton_type::default_action;
    bool left = false;
};
