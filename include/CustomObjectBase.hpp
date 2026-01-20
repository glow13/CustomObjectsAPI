#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>

using namespace geode::prelude;

#define $base(BaseType, ObjectBase) BaseType##Base : public CustomObjectBase<ObjectType, ObjectBase>
#define $generic(BaseType) BaseType final : public BaseType##Base<BaseType> {}

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
    template <typename ValueType>
    void setupObjectProperty(int key, ValueType& value, std::function<bool(void)> cond = 0) {
        if (objectProperties.contains(key)) {
            auto prop = static_cast<ObjectProp<std::string>*>(objectProperties[key]);
            prop->fromString(&value, prop->value);
            objectProperties.erase(key);
            delete prop;
        } //if
        objectProperties[key] = new ObjectProp<ValueType>(&value, cond);
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
    struct ObjectPropBase {
        virtual void setValue(std::string) = 0;
        virtual std::string getValue() = 0;
        virtual bool hasValue() = 0;

        bool valid(bool val) { return val; }
        bool valid(int val) {return true; }
        bool valid(float val) { return true; }
        bool valid(std::string val) { return !val.empty(); }

        std::string toString(bool val) { return val ? "1" : "0"; }
        std::string toString(int val) { return fmt::format("{}", val); }
        std::string toString(float val) { return fmt::format("{:g}", val); }
        std::string toString(std::string val) { return base64::encode(val); }

        void fromString(bool* value, std::string val) { *value = val == "1"; }
        void fromString(int* value, std::string val) { *value = std::stoi(val); }
        void fromString(float* value, std::string val) { *value = std::stof(val); }
        void fromString(std::string* value, std::string val) { *value = base64::decodeString(val).unwrapOrDefault(); }
    };

    template <typename ValueType>
    struct ObjectProp final : public ObjectPropBase {
        ValueType* var;
        ValueType value;
        std::function<bool(void)> cond;
        ObjectProp(ValueType* var, std::function<bool(void)> cond, ValueType value = ValueType()) : var(var), value(var ? *var : value), cond(cond) {}

        std::string getValue() override { return var ? ObjectPropBase::toString(*var) : ""; }
        bool hasValue() override { return var && ObjectPropBase::valid(*var) && (!cond || cond()); }
        void setValue(std::string val) override { if (var) ObjectPropBase::fromString(var, val); }
    };

    const CustomObjectConfig* config;
    std::unordered_map<int, ObjectPropBase*> objectProperties;

    gd::string getSaveString(GJBaseGameLayer* p0) override final {
        std::string propertyString;
        for (auto [key, prop] : objectProperties) {
            if (prop->hasValue()) propertyString += fmt::format(",{},{}", key, prop->getValue());
        } // for

        return ObjectBase::getSaveString(p0) + propertyString;
    } // getSaveString

    void customObjectSetup(gd::vector<gd::string>& propValues, gd::vector<void*>& propIsPresent) override final {
        ObjectBase::customObjectSetup(propValues, propIsPresent);

        for (int key = 4; key < propValues.size(); key++) {
            if (!propIsPresent[key]) continue;
            if (objectProperties.contains(key)) objectProperties[key]->setValue(propValues[key]);
            else objectProperties[key] = new ObjectProp<std::string>(nullptr, 0, propValues[key]);
        } // for

        setupCustomObject();
    } // customObjectSetup
};