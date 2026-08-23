#include <Geode/Geode.hpp>
#include "ModTriggerObject.hpp"

using namespace geode::prelude;

bool ModTriggerObject::init() {
    auto modTriggerSpr = fmt::format("custom-objects/{}/0.0.0.0/mod-trigger.png", getConfig().getModID());
    if (!EffectGameObject::init(modTriggerSpr.c_str())) return false;

    m_objectType = GameObjectType::Modifier;
    m_baseColor->m_defaultColorID = 0;
    m_isInvisible = !m_editorEnabled;
    m_width = 30;
    m_height = 30;

    bindObjectProperty(51, m_targetGroupID);
    bindObjectProperty(82, (int&)mode);
    bindObjectProperty(83, disabled);

    return true;
}

void ModTriggerObject::setupCustomObject() {
    m_dontIgnoreDuration = false;
    updateObjectLabel();
}

void ModTriggerObject::activateCustomObject(GJBaseGameLayer* level, PlayerObject* player, std::vector<int> remaps) {
    if (disabled) return;
    if ((int)mode < 2) level->toggleGroup(m_targetGroupID, mode != ModTriggerMode::OFF);
    else level->spawnGroup(m_targetGroupID, false, 0, remaps, m_uniqueID, 0);
}

SetupModTriggerPopup* SetupModTriggerPopup::create(ModTriggerObject* obj, cocos2d::CCArray* objs, std::string modID, std::string modName) {
    auto popup = new SetupModTriggerPopup();
    if (popup->init(obj, objs, modID, modName)) {
        popup->autorelease();
        return popup;
    }

    delete popup;
    return nullptr;
}

bool SetupModTriggerPopup::init(ModTriggerObject* obj, cocos2d::CCArray* objs, std::string modID, std::string modName) {
    if (!SetupTriggerPopup::init(obj, objs, 320, 260, 1)) return false;
    preSetup();

    auto helpMessage = fmt::format("This object is used to detect if a mod using the <cg>Custom Objects API</c> is installed. "
                                    "This specific trigger can only be activated if the mod <cl>\"{}\"</c> is installed. "
                                    "Otherwise this object will not exist and therefore <cr>will never be triggered</c>.\n\n"
                                    "You can either <cg>toggle on, toggle off, or spawn</c> any group ID.\n"
                                    "You can also <cr>disable this trigger</c> to test what its behavior is like when the mod is not installed.\n\n"
                                    "Basically a glorified spawn trigger lol", modID);

    auto winSize = CCDirector::get()->getWinSize();
    float xCenter = winSize.width * 0.5;
    float yCenter = winSize.height * 0.5;

    float lEdge = (winSize.width - m_width) * 0.5;
    float rEdge = (winSize.width + m_width) * 0.5;
    float bEdge = (winSize.height - m_height) * 0.5;

    addTitle(modName);
    addHelpButton("Mod Trigger Help", helpMessage, 0.9);
    addCloseButton("OK");

    auto modIDMessage = fmt::format("({})", modID);
    auto modIDPosition = CCPoint(xCenter, yCenter + m_height * 0.5 - 28);

    auto modIDTitle = CCLabelBMFont::create(modIDMessage.c_str(), "bigFont.fnt");
    modIDTitle->setPosition(modIDPosition);
    modIDTitle->setScale(0.3);
    m_mainLayer->addChild(modIDTitle);

    auto groupIDPosition = CCPoint(xCenter, yCenter + 35);
    createValueControlAdvanced(51, "Group ID", groupIDPosition, 0.9, true, InputValueType::Uint, 6, true, 0, 100, 0, 0, GJInputStyle::GoldLabel, 2, false);
    createMultiTriggerItems(CCPoint(lEdge + 20, bEdge + 50), CCPoint(lEdge + 20, bEdge + 20), CCPoint(rEdge - 72, bEdge + 50));

    createToggleButton("Disable", menu_selector(SetupModTriggerPopup::onDisabledToggle), disabled, CCPoint(rEdge - 72, bEdge + 20));
    createCustomToggleValueControl(0, mode == ModTriggerMode::ON, false, "On", CCPoint(xCenter - 60, yCenter - 30), true, 0, 0);
    createCustomToggleValueControl(1, mode == ModTriggerMode::OFF, false, "Off", CCPoint(xCenter, yCenter - 30), true, 0, 0);
    createCustomToggleValueControl(2, mode == ModTriggerMode::SPAWN, false, "Spawn", CCPoint(xCenter + 60, yCenter - 30), true, 0, 0);

    postSetup();
    return true;
}

void SetupModTriggerPopup::determineStartValues() {
    SetupTriggerPopup::determineStartValues();
    auto objs = getObjects()->asExt<ModTriggerObject*>();

    mode = objs[0]->mode;
    for (auto obj : objs) if (mode != obj->mode) { mode = ModTriggerMode::NONE; break; }

    disabled = objs[0]->disabled;
    for (auto obj : objs) if (disabled != obj->disabled) { disabled = false; break; }
}

void SetupModTriggerPopup::onCustomToggleTriggerValue(cocos2d::CCObject* sender) {
    mode = static_cast<ModTriggerMode>(sender->getTag());
    updateCustomToggleTrigger(0, mode == ModTriggerMode::ON);
    updateCustomToggleTrigger(1, mode == ModTriggerMode::OFF);
    updateCustomToggleTrigger(2, mode == ModTriggerMode::SPAWN);

    for(auto obj : getObjects()->asExt<ModTriggerObject*>()) obj->mode = mode;
}

void SetupModTriggerPopup::onClose(cocos2d::CCObject* sender) {
    for(auto obj : getObjects()->asExt<ModTriggerObject*>()) obj->updateObjectLabel();
    SetupTriggerPopup::onClose(sender);
}

void SetupModTriggerPopup::onDisabledToggle(cocos2d::CCObject* sender) {
    disabled = !static_cast<CCMenuItemToggler*>(sender)->m_toggled;
    for(auto obj : getObjects()->asExt<ModTriggerObject*>()) obj->disabled = disabled;
}