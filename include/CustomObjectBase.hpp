#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>

#define $base(BaseType, ObjectBase) BaseType##Base : public CustomObjectBase<ObjectType, ObjectBase>
#define $generic(BaseType) BaseType final : public BaseType##Base<BaseType> {}

#define SERIALIZE_DEFS(type, valid, serialize, deserialize) \
    template<> static bool isValid<type>(type val) { return valid; }; \
    template<> static std::string serializeValue<type>(type val) { return serialize; }; \
    template<> static type deserializeValue<type>(std::string val) { return deserialize; };

template <class ObjectType, class ObjectBase>
class CustomObjectBase : public ObjectBase {
protected:
    using Base = CustomObjectBase<ObjectType, ObjectBase>;
    using CustomObjectConfig = CustomObjectConfig<ObjectType>;
public:
    static ObjectType* create(const CustomObjectConfig* config) {
        auto obj = new ObjectType();
        if (obj->init(config)) {
            obj->config = config;
            obj->autorelease();
            return obj;
        } // if

        delete obj;
        return nullptr;
    } // create

    bool init(const CustomObjectConfig* config) {
        if (!ObjectBase::init(config->getMainSprite().c_str())) return false;

        // Add sprites to custom object
        if (!config->hasMainSprite()) this->setDontDraw(true);
        if (config->hasDetailSprite()) this->addCustomColorChild(config->getDetailSprite());

        // Add glow to custom object
        if (this->m_editorEnabled || this->m_hasNoGlow) return true;
        if (config->hasGlowSprite()) this->createGlow(config->getGlowSprite());

        return true;
    } // init

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

protected:
    template <class ValueType>
    void setupObjectProperty(int key, ValueType& value, std::function<bool(void)> cond = 0) {
        auto& prop = objectProps.try_emplace(key).first->second;
        if (!prop.loadedSaveValue.empty()) value = deserializeValue<ValueType>(prop.loadedSaveValue);

        prop.serialize = [&value](){ return serializeValue<ValueType>(value); };
        prop.deserialize = [&value](std::string str){ value = deserializeValue<ValueType>(str); };
        prop.isValid = [&value, cond](){ return isValid<ValueType>(value) && (!cond || cond()); };
    } // setupObjectProperty

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

    void setStartPos(cocos2d::CCPoint p0) override {
        if (this->m_editorEnabled) {
            if (auto lel = LevelEditorLayer::get(); lel && lel->m_editorUI) {
                p0 += config->createOffset;
            } // if
        } // if
        ObjectBase::setStartPos(p0);
    } // setStartPos

    void customSetup() override {
        if (this->m_particle) {
            this->updateParticleColor(config->particleColor);
            this->updateParticleOpacity(config->particleOpacity);
        } // if
        ObjectBase::customSetup();
    } // customSetup

    void firstSetup() override {
        ObjectBase::firstSetup();
        setupCustomObject();
    } // firstSetup

    void resetObject() override {
        ObjectBase::resetObject();
        resetCustomObject();
    } // resetObject

private:
    struct ObjectProp final {
        std::string loadedSaveValue;
        std::function<std::string()> serialize;
        std::function<void(std::string)> deserialize;
        std::function<bool()> isValid;
    };

    template<class ValueType> static bool isValid(ValueType);
    template<class ValueType> static std::string serializeValue(ValueType);
    template<class ValueType> static ValueType deserializeValue(std::string);

    SERIALIZE_DEFS(bool, val, val ? "1" : "0", val == "1");
    SERIALIZE_DEFS(int, true, std::to_string(val), std::stoi(val));
    SERIALIZE_DEFS(float, true, std::to_string(val), std::stof(val));
    SERIALIZE_DEFS(std::string, !val.empty(), geode::utils::base64::encode(val), geode::utils::base64::decodeString(val).unwrapOr(""));

    const CustomObjectConfig* config;
    std::unordered_map<int, ObjectProp> objectProps;

    gd::string getSaveString(GJBaseGameLayer* p0) override final {
        auto saveString = ObjectBase::getSaveString(p0);

        for (auto [key, prop] : objectProps) {
            if (prop.isValid && prop.isValid()) {
                saveString += fmt::format(",{},{}", key, prop.serialize());
            } // if
        } // for

        return saveString;
    } // getSaveString

    void customObjectSetup(gd::vector<gd::string>& propValues, gd::vector<void*>& propIsPresent) override final {
        ObjectBase::customObjectSetup(propValues, propIsPresent);

        for (int key = 4; key < propValues.size(); key++) {
            if (!propIsPresent[key]) continue;

            auto& prop = objectProps.try_emplace(key).first->second;
            if (prop.deserialize) prop.deserialize(propValues[key]);
            prop.loadedSaveValue = propValues[key];
        } // for

        setupCustomObject();
    } // customObjectSetup
};