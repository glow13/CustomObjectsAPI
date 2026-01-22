#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>

#define $base(BaseType, ObjectBase) BaseType##Base : public CustomObjectBase<ObjectType, ObjectBase>
#define $generic(BaseType) BaseType final : public BaseType##Base<BaseType> {}

#define SERIALIZER_TYPE(type, valid, serialize, deserialize) \
    template<> inline Serializer serializer<type> = { \
    [](const void* v)->bool { auto val = *static_cast<const type*>(v); return valid; }, \
    [](const void* v)->std::string { auto val = *static_cast<const type*>(v); return serialize; }, \
    [](void* v, const std::string& val) { *static_cast<type*>(v) = deserialize; }};

template <class ObjectType, class ObjectBase>
class CustomObjectBase : public ObjectBase {
protected:
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

    template <class ValueType>
    void setupObjectProperty(int key, ValueType& value, std::function<bool()> cond = nullptr) {
        if (auto it = loadedSaveValues.find(key); it != loadedSaveValues.end()) {
            serializer<ValueType>.deserialize(&value, it->second);
            loadedSaveValues.erase(it);
        } // if

        objectProps.emplace(key, ObjectProp{
            &value,
            std::move(cond),
            &serializer<ValueType>
        });
    } // setupObjectProperty

    gd::string getSaveString(GJBaseGameLayer* p0) override final {
        auto saveString = ObjectBase::getSaveString(p0);

        for (auto [key, prop] : objectProps) if (prop.isValid()) {
            saveString += fmt::format(",{},{}", key, prop.serialize());
        } // for

        return saveString;
    } // getSaveString

    void customObjectSetup(gd::vector<gd::string>& propValues, gd::vector<void*>& propIsPresent) override final {
        ObjectBase::customObjectSetup(propValues, propIsPresent);

        for (int key = 4; key < propValues.size(); key++) {
            if (!propIsPresent[key]) continue;

            if (auto it = objectProps.find(key); it != objectProps.end()) {
                it->second.deserialize(propValues[key]);
            } else loadedSaveValues.emplace(key, propValues[key]);
        } // for

        setupCustomObject();
    } // customObjectSetup

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
    struct Serializer final {
        bool (*isValid)(const void*) = nullptr;
        std::string (*serialize)(const void*) = nullptr;
        void (*deserialize)(void*, const std::string&) = nullptr;
    };

    template <typename ValueType>
    static inline Serializer serializer;

    SERIALIZER_TYPE(bool, val, val ? "1" : "0", val == "1");
    SERIALIZER_TYPE(int, true, std::to_string(val), std::stoi(val));
    SERIALIZER_TYPE(float, true, std::to_string(val), std::stof(val));
    SERIALIZER_TYPE(std::string, !val.empty(), geode::utils::base64::encode(val), geode::utils::base64::decodeString(val).unwrapOr(""));

    struct ObjectProp final {
        void* value = nullptr;
        std::function<bool()> cond = nullptr;
        const Serializer* serializer = nullptr;

        inline bool isValid() { return serializer ? serializer->isValid(value) && (!cond || cond()) : false; }
        inline std::string serialize() { return serializer ? serializer->serialize(value) : ""; }
        inline void deserialize(const std::string& str) { if (serializer) serializer->deserialize(value, str); }
    };

    const CustomObjectConfig* config;
    std::unordered_map<uint16_t, ObjectProp> objectProps;
    std::unordered_map<uint16_t, std::string> loadedSaveValues;
};