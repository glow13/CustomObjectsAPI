#pragma once
#include "../../objects/CustomTriggerObject.hpp"

enum ModTriggerMode : int {
    ON,
    OFF,
    SPAWN,
    NONE,
};

class $object(ModTriggerObject, CustomTriggerObject) {
public:
    ModTriggerMode mode;
    bool disabled;

    bool init() override;
    void setupCustomObject() override;
    void activateCustomObject(GJBaseGameLayer*, PlayerObject*, std::vector<int>) override;
};

class SetupModTriggerPopup : public SetupTriggerPopup {
public:
    ModTriggerMode mode;
    bool disabled;

    static SetupModTriggerPopup* create(ModTriggerObject*, cocos2d::CCArray*, std::string, std::string);
    bool init(ModTriggerObject*, cocos2d::CCArray*, std::string, std::string);

    void determineStartValues() override;
    void onCustomToggleTriggerValue(cocos2d::CCObject*) override;
    void onClose(cocos2d::CCObject*) override;

    void onDisabledToggle(cocos2d::CCObject*);
};