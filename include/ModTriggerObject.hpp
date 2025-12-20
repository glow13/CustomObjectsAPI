#pragma once

#include "object/CustomTriggerObject.hpp"

class $object(ModTriggerObject, CustomTriggerObject) {
public:
    std::string mod = "";

    bool init(const CustomObjectConfig* config) {
        if (!CustomObjectBase::init(config)) return false;
        mod = config->mod;
        return true;
    } // init

    void setupCustomObject() override {
        if (!mod.empty()) log::info("{}", mod);
        else log::error("no mod lol");
    } // setupCustomObject
};