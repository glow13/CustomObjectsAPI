#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>

using namespace geode::prelude;

template <class T>
class CustomTriggerObject : public EffectGameObject {
public:
    static T* create(CustomObjectConfig config) {
        auto obj = new T();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomTrigger() {}
    virtual void resetCustomTrigger() {}
    virtual void activateCustomTrigger(GJBaseGameLayer* playLayer) {}

    bool init(CustomObjectConfig config) {
        if (!EffectGameObject::init(config.frame.c_str())) return false;

        m_objectID = config.id;
        m_parentMode = 10;
        m_objectType = GameObjectType::Modifier;
        m_isTrigger = true;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);

        setupCustomTrigger();
        autorelease();

        return true;
    } // init

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
        std::stringstream valueString(value);

        savedValues[key] = valueString.str();
        return oldValue;
    } // setSavedValue

private:
    std::map<std::string, std::string> savedValues;

    bool loadSavedValues(gd::vector<gd::string>& p0) {

        // Are there any saved values to load?
        if (p0[442].empty()) return false;

        // Base64 decode the saved string
        auto result = base64::decodeString(p0[442]);
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
    } // loadSavedValues

    gd::string getSaveString(GJBaseGameLayer* p0) override {
        std::string saveString = EffectGameObject::getSaveString(p0);
        if (savedValues.empty()) return saveString;

        std::string valuesString;
        for (auto [key, value] : savedValues) valuesString += fmt::format("{},{},", key, value);
        valuesString = valuesString.substr(0, valuesString.length() - 1);
        return saveString += fmt::format(",442,{}", base64::encode(valuesString));
    } // getSaveString

    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        EffectGameObject::customObjectSetup(p0, p1);
        m_isTrigger = true;
        loadSavedValues(p0);
        for (auto [key, value] : savedValues) log::info("{} => {}", key, value);
        setupCustomTrigger();
    } // customObjectSetup

    void resetObject() override {
        EffectGameObject::resetObject();
        resetCustomTrigger();
    } // resetObject

    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) override {
        activateCustomTrigger(p0);
    } // triggerObject
};

class CustomTriggerObjectBase : public CustomTriggerObject<CustomTriggerObjectBase> {};