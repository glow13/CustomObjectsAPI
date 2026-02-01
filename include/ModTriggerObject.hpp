#pragma once

#include "object/CustomTriggerObject.hpp"

using namespace geode::prelude;

class ModTriggerObject final : public CustomTriggerObjectBase<ModTriggerObject> {
public:
    bool init(const CustomObjectConfig* config) {
        auto modTriggerFrame = fmt::format("custom-objects/{}/0.0.0.0/mod-trigger.png", config->getModID());
        if (!EffectGameObject::init(modTriggerFrame.c_str())) return false;

        m_objectType = GameObjectType::Modifier;
        m_baseColor->m_defaultColorID = 0;
        m_isInvisible = !m_editorEnabled;
        m_width = 30;
        m_height = 30;

        setupObjectProperty<int>(51, m_targetGroupID);
        setupObjectProperty<bool>(56, m_activateGroup);

        return true;
    } // init

    void setupCustomObject() override {
        m_dontIgnoreDuration = false;
        updateObjectLabel();
    } // setupCustomObject

    void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player, std::vector<int> remaps) override {
        if (m_activateGroup) level->spawnGroup(m_targetGroupID, false, 0, remaps, m_uniqueID, 0);
        level->toggleGroup(m_targetGroupID, m_activateGroup);
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
        if (!SetupTriggerPopup::init(obj, objs, 300, 240, 1)) return false;
        preSetup();

        auto winSize = CCDirector::get()->getWinSize();
        auto helpMessage = fmt::format("This object is used to detect if a mod using the <cg>Custom Objects API</c> is installed.\n"
                                       "The trigger will activate only if the mod <cl>\"{}\"</c> is installed.\n"
                                       "Otherwise this object will not exist and therefore <cr>will not be triggered</c>.", modID);

        addTitle(modName);
        addHelpButton("Mod Trigger Help", helpMessage, 0.8);
        createMultiTriggerItemsDefault();
        addCloseButton("OK");

        auto modIDMessage = fmt::format("({})", modID);
        auto modIDPosition = CCPoint(winSize.width * 0.5, (winSize.height + m_height) * 0.5 - 28);

        auto modIDTitle = CCLabelBMFont::create(modIDMessage.c_str(), "bigFont.fnt");
        modIDTitle->setPosition(modIDPosition);
        modIDTitle->setScale(0.3);
        m_mainLayer->addChild(modIDTitle);

        auto groupIDPosition = CCPoint(winSize.width * 0.5, winSize.height * 0.5 + 30);
        createValueControlAdvanced(51, "Group ID", groupIDPosition, 0.9, true, InputValueType::Uint, 6, true, 0, 100, 0, 0, GJInputStyle::GoldLabel, 2, false);

        auto activatePosition = CCPoint(winSize.width * 0.5 - 50, winSize.height * 0.5 - 20);
        createToggleValueControl(56, "Activate Group", activatePosition, false, 0, 0, 1);

        postSetup();
        return true;
    } // init

    void onClose(cocos2d::CCObject* sender) override {
        if (auto obj = static_cast<ModTriggerObject*>(m_gameObject)) obj->updateObjectLabel();
        else for(auto obj : CCArrayExt<ModTriggerObject*>(m_gameObjects)) obj->updateObjectLabel();
        SetupTriggerPopup::onClose(sender);
    } // onClose
};