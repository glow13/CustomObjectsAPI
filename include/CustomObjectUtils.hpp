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
    static ObjectType* create(CustomObjectConfig config) {
        auto obj = new ObjectType();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomObject() {}
    virtual void resetCustomObject() {}

    bool commonSetup(CustomObjectConfig config) {
        auto frame = (config.frame.empty()) ? config.detailFrame : config.frame;
        if (!ObjectBase::init(frame.c_str())) return false;

        this->m_objectType = GameObjectType::Decoration;
        this->m_objectID = config.id;
        this->m_parentMode = 10;

        if (config.frame.empty()) this->setDontDraw(true);
        if (!config.detailFrame.empty()) this->addCustomColorChild(config.detailFrame);

        this->autorelease();
        return true;
    } // commonSetup

    bool applyConfig(CustomObjectConfig config, std::convertible_to<int> auto... options) {
        bool success = true;

        (..., [this, &config, &success] (Config option) {
            if (!config.applyConfigOption(this, option)) success = false;
        }(options));

        return success;
    } // applyConfig

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

private:
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

    void setStartPos(cocos2d::CCPoint p0) override {
        if (auto editor = LevelEditorLayer::get()) {
            ObjectBase::setStartPos((editor->m_editorUI) ? p0 + this->m_unk464 : p0);
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