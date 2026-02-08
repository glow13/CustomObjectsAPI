#pragma once

#include "object/CustomTriggerObject.hpp"

using namespace geode::prelude;

class ModTriggerObject final : public CustomTriggerObjectBase<ModTriggerObject> {
public:
    int modTriggerMode;

    bool init(const CustomObjectConfig* config) {
        auto modTriggerSpr = fmt::format("custom-objects/{}/0.0.0.0/mod-trigger.png", config->getModID());
        if (!EffectGameObject::init(modTriggerSpr.c_str())) return false;

        m_objectType = GameObjectType::Modifier;
        m_baseColor->m_defaultColorID = 0;
        m_isInvisible = !m_editorEnabled;
        m_width = 30;
        m_height = 30;

        setupObjectProperty<int>(51, m_targetGroupID);
        setupObjectProperty<int>(82, modTriggerMode);

        return true;
    } // init

    void setupCustomObject() override {
        m_dontIgnoreDuration = false;
        updateObjectLabel();
    } // setupCustomObject

    void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player, std::vector<int> remaps) override {
        if (modTriggerMode < 2) level->toggleGroup(m_targetGroupID, modTriggerMode == 0);
        else level->spawnGroup(m_targetGroupID, false, 0, remaps, m_uniqueID, 0);
    } // activateCustomObject
};

class SetupModTriggerPopup : public SetupTriggerPopup {
public:
    static SetupModTriggerPopup* create(ModTriggerObject* obj, cocos2d::CCArray* objs, std::string modID, std::string modName) {
        auto popup = new SetupModTriggerPopup();
        if (popup->init(obj, objs, modID, modName)) {
            popup->autorelease();
            return popup;
        } // if

        delete popup;
        return nullptr;
    } // create

    bool init(ModTriggerObject* obj, cocos2d::CCArray* objs, std::string modID, std::string modName) {
        if (!SetupTriggerPopup::init(obj, objs, 320, 260, 1)) return false;
        preSetup();

        auto winSize = CCDirector::get()->getWinSize();
        auto helpMessage = fmt::format("This object is used to detect if a mod using the <cg>Custom Objects API</c> is installed.\n"
                                       "The trigger will activate only if the mod <cl>\"{}\"</c> is installed. "
                                       "Otherwise this object will not exist and therefore <cr>will not be triggered</c>.\n"
                                       "You can either toggle on, toggle off, or just spawn any group ID.", modID);

        addTitle(modName);
        addHelpButton("Mod Trigger Help", helpMessage, 0.9);
        createMultiTriggerItemsDefault();
        addCloseButton("OK");

        auto modIDMessage = fmt::format("({})", modID);
        auto modIDPosition = CCPoint(winSize.width * 0.5, (winSize.height + m_height) * 0.5 - 28);

        auto modIDTitle = CCLabelBMFont::create(modIDMessage.c_str(), "bigFont.fnt");
        modIDTitle->setPosition(modIDPosition);
        modIDTitle->setScale(0.3);
        m_mainLayer->addChild(modIDTitle);

        auto groupIDPosition = CCPoint(winSize.width * 0.5, winSize.height * 0.5 + 35);
        createValueControlAdvanced(51, "Group ID", groupIDPosition, 0.9, true, InputValueType::Uint, 6, true, 0, 100, 0, 0, GJInputStyle::GoldLabel, 2, false);

        auto mode = obj ? obj->modTriggerMode : -1;
        if (!obj) for(auto go : CCArrayExt<ModTriggerObject*>(m_gameObjects)) {
            if (mode == -1) mode = go->modTriggerMode;
            else if (mode != go->modTriggerMode) { mode = 0; break; }
        } // for

        auto toggleOnPosition = CCPoint(winSize.width * 0.5 - 60, winSize.height * 0.5 - 30);
        createCustomToggleValueControl(0, mode == 0, false, "On", toggleOnPosition, true, 0, 0);

        auto toggleOffPosition = CCPoint(winSize.width * 0.5, winSize.height * 0.5 - 30);
        createCustomToggleValueControl(1, mode == 1, false, "Off", toggleOffPosition, true, 0, 0);

        auto spawnOnlyPosition = CCPoint(winSize.width * 0.5 + 60, winSize.height * 0.5 - 30);
        createCustomToggleValueControl(2, mode == 2, false, "Spawn", spawnOnlyPosition, true, 0, 0);

        postSetup();
        return true;
    } // init

    void onCustomToggleTriggerValue(cocos2d::CCObject* sender) override {
        auto value = sender->getTag();
        updateCustomToggleTrigger(0, value == 0);
        updateCustomToggleTrigger(1, value == 1);
        updateCustomToggleTrigger(2, value == 2);

        if (auto obj = static_cast<ModTriggerObject*>(m_gameObject)) obj->modTriggerMode = value;
        else for(auto obj : CCArrayExt<ModTriggerObject*>(m_gameObjects)) obj->modTriggerMode = value;
    } // onCustomToggleTriggerValue

    void onClose(cocos2d::CCObject* sender) override {
        if (auto obj = static_cast<ModTriggerObject*>(m_gameObject)) obj->updateObjectLabel();
        else for(auto obj : CCArrayExt<ModTriggerObject*>(m_gameObjects)) obj->updateObjectLabel();
        SetupTriggerPopup::onClose(sender);
    } // onClose
};