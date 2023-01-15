#pragma once
#include "handle/page/slot_setting.h"

class data_helper {
public:
    RE::TESForm* form = nullptr;
    handle::slot_setting::slot_type type = handle::slot_setting::slot_type::empty;
    handle::slot_setting::acton_type action_type = handle::slot_setting::acton_type::default_action;
    bool left = false;
};
