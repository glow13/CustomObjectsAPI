#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>

using namespace geode::prelude;

#define OBJECT1(ObjectType) ObjectType : public CustomGameObjectBase<ObjectType>
#define OBJECT2(ObjectType, ObjectBase) ObjectType : public ObjectBase ## Base<ObjectType>
#define $object(...) GEODE_INVOKE(GEODE_CONCAT(OBJECT, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

template <class ObjectType, class ObjectBase>
class CustomObjectUtils : public ObjectBase {
public:
    static ObjectType* create(const CustomObjectConfig<ObjectType>* config) {
        auto obj = new ObjectType();
        if (obj->init(config)) {
            obj->config = config;
            obj->autorelease();
            return obj;
        } // if

        delete obj;
        return nullptr;
    } // create

    template<typename ValueType>
    ValueType getSavedValue(std::string key, ValueType defaultValue = ValueType{}) {
        if (!savedValues.contains(key)) return defaultValue;
        std::stringstream valueString(savedValues[key]);

        ValueType value;
        valueString >> value;
        return value;
    } // getSavedValue

    template<typename ValueType>
    ValueType setSavedValue(std::string key, ValueType value) {
        ValueType oldValue = getSavedValue<ValueType>(key);
        std::stringstream valueString;
        valueString.precision(2);
        valueString << value;

        savedValues[key] = valueString.str();
        return oldValue;
    } // setSavedValue

protected:
    virtual void setupCustomObject() {
        if (!(config->setupCustomObjectFunction)) return;
        (config->setupCustomObjectFunction)(static_cast<ObjectType*>(this));
    } // setupCustomObject

    virtual void resetCustomObject() {
        if (!(config->resetCustomObjectFunction)) return;
        (config->resetCustomObjectFunction)(static_cast<ObjectType*>(this));
    } // resetCustomObject

    virtual void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) {
        if (!(config->activateCustomObjectFunction)) return;
        (config->activateCustomObjectFunction)(static_cast<ObjectType*>(this), level, player);
    } // activateCustomObject

    bool commonSetup(const CustomObjectConfig<ObjectType>* config, bool addSprites = true) {
        if (!ObjectBase::init(config->mainSprite)) return false;

        // Add sprites to custom object
        if (!addSprites) return true;
        if (!config->mainSprite) this->setDontDraw(true);
        if (config->detailSprite) this->addCustomColorChild(config->detailSprite);

        // Add glow to custom object
        if (this->m_editorEnabled || this->m_hasNoGlow) return true;
        if (config->glowSprite) this->createGlow(config->glowSprite);

        return true;
    } // commonSetup

    gd::string getSaveString(GJBaseGameLayer* p0) override {
        std::string saveString = ObjectBase::getSaveString(p0);
        if (savedValues.empty()) return saveString;

        std::string valuesString;
        for (auto [key, value] : savedValues) valuesString += fmt::format("{},{},", key, value);
        valuesString = valuesString.substr(0, valuesString.length() - 1);
        return saveString += fmt::format(",500,{}", base64::encode(valuesString));
    } // getSaveString

    void addMainSpriteToParent(bool p0) override {
        bool disableBlend = (this->m_parentMode == 4);
        this->m_colorZLayerRelated = this->m_colorZLayerRelated || disableBlend;

        ObjectBase::addMainSpriteToParent(p0);

        this->m_shouldBlendBase = this->m_shouldBlendBase && !disableBlend;
        this->m_shouldBlendDetail = this->m_shouldBlendDetail && !disableBlend;
    } // addMainSpriteToParent

    void activateObject() override {
        bool activated = this->m_isActivated;
        ObjectBase::activateObject();

        if (this->m_parentMode == 10 && activated != this->m_isActivated && !this->m_isInvisible && this->m_glowSprite) {
            this->m_glowSprite->removeFromParent();
            auto zLayer = (this->m_zLayer != ZLayer::Default) ? this->m_zLayer : this->m_defaultZLayer;
            auto parent = this->parentForZLayer((int)zLayer, true, 10);
            parent->addChild(this->m_glowSprite, -1000);
        } // if
    } // activateObject

private:
    const CustomObjectConfig<ObjectType>* config;
    std::map<std::string, std::string> savedValues;

    bool loadSavedValuesFromString(std::string saveString) {

        // Are there any saved values to load?
        if (saveString.empty()) return false;

        // Base64 decode the saved string
        auto result = base64::decodeString(saveString);
        if (!result.isOk()) return false;

        // Parse the string and load the values
        std::stringstream valuesString(result.unwrap());
        std::string key, valueStr;

        savedValues.clear();
        while (std::getline(valuesString, key, ',')) {
            std::getline(valuesString, valueStr, ',');
            savedValues[key] = valueStr;
        } // while

        return true;
    } // loadSavedValuesFromString

    void setStartPos(cocos2d::CCPoint p0) override {
        if (auto editor = LevelEditorLayer::get()) {
            ObjectBase::setStartPos((editor->m_editorUI) ? p0 + config->createOffset : p0);
        } else ObjectBase::setStartPos(p0);
    } // setStartPos

    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        ObjectBase::customObjectSetup(p0, p1);
        loadSavedValuesFromString(p0[500]);
        setupCustomObject();
    } // customObjectSetup

    void resetObject() override {
        ObjectBase::resetObject();
        resetCustomObject();
    } // resetObject

    void firstSetup() override {
        ObjectBase::firstSetup();
        setupCustomObject();
    } // firstSetup
};