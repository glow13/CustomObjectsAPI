#pragma once
#include "CustomObjectConfig.hpp"
#include "CustomObjectProperty.hpp"

class CUSTOM_OBJECTS_DLL CustomObjectInterface {
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend class CustomObjectConfig;

    template <class BaseType, typename Enable>
    requires (std::derived_from<BaseType, GameObject> && !std::derived_from<BaseType, RegisteredObjectByClass>)
    friend class CustomObjectBase;

    void setupCustomObject(GameObject*) const;
    void resetCustomObject(GameObject*) const;
    void activateCustomObject(GameObject*, GJBaseGameLayer*, PlayerObject*) const;

    void bindObjectProperty(int, std::unique_ptr<ObjectPropertyInterface>&&);
    void setupObjectProperties(std::vector<std::string>&, std::vector<void*>&);
    std::string getCustomSaveString() const;

    virtual GameObject* gameObject() = 0;
protected:
    virtual bool init() = 0;
    const CustomObjectConfig&& getConfig() const;
public:
    CustomObjectInterface();
    virtual ~CustomObjectInterface();
};

template <class BaseType, typename Enable = void>
requires (std::derived_from<BaseType, GameObject> && !std::derived_from<BaseType, RegisteredObjectByClass>)
class CustomObjectBase : public CustomObjectInterface, public BaseType {
public:
    bool init() override {
        const CustomObjectConfig&& config = getConfig();
        if (!BaseType::init(config.getMainSprite().c_str())) return false;

        // Add sprites to the custom object
        if (!config.hasMainSprite()) this->setDontDraw(true);
        if (config.hasDetailSprite()) this->addCustomColorChild(config.getDetailSprite());

        // Add glow to the custom object
        if (this->m_editorEnabled || this->m_hasNoGlow) return true;
        if (config.hasGlowSprite()) this->createGlow(config.getGlowSprite());

        return true;
    }

    virtual void setupCustomObject() {
        CustomObjectInterface::setupCustomObject(this);
    }

    virtual void resetCustomObject() {
        CustomObjectInterface::resetCustomObject(this);
    }

    virtual void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) {
        CustomObjectInterface::activateCustomObject(this, level, player);
    }

    template <typename T>
    void bindObjectProperty(int key, T& value, geode::Function<bool()> cond = nullptr) {
        CustomObjectInterface::bindObjectProperty(key, std::make_unique<ObjectProperty<T>>(value, std::move(cond)));
    }

    void customObjectSetup(gd::vector<gd::string>& propValues, gd::vector<void*>& propIsPresent) override final {
        BaseType::customObjectSetup(propValues, propIsPresent);
        CustomObjectInterface::setupObjectProperties(propValues, propIsPresent);
        setupCustomObject();
    }

    gd::string getSaveString(GJBaseGameLayer* p0) override final {
        std::string saveString = BaseType::getSaveString(p0);
        return saveString + CustomObjectInterface::getCustomSaveString();
    }

    void addMainSpriteToParent(bool p0) override {
        bool disableBlend = (this->m_parentMode == 4);
        this->m_colorZLayerRelated = this->m_colorZLayerRelated || disableBlend;

        BaseType::addMainSpriteToParent(p0);

        this->m_shouldBlendBase = this->m_shouldBlendBase && !disableBlend;
        this->m_shouldBlendDetail = this->m_shouldBlendDetail && !disableBlend;
    }

    void activateObject() override {
        bool activated = this->m_isActivated;
        BaseType::activateObject();

        if (activated != this->m_isActivated && this->m_parentMode == CUSTOM_PARENT_MODE && !this->m_isInvisible && this->m_glowSprite) {
            this->m_glowSprite->removeFromParent();
            auto zLayer = (this->m_zLayer != ZLayer::Default) ? this->m_zLayer : this->m_defaultZLayer;
            auto parent = this->parentForZLayer((int)zLayer, true, CUSTOM_PARENT_MODE);
            parent->addChild(this->m_glowSprite, -1000);
        }
    }

    void customSetup() override {
        if (!this->m_particle) return BaseType::customSetup();

        this->updateParticleColor(getConfig().getParticleColor());
        this->updateParticleOpacity(getConfig().getParticleOpacity());

        if (!getConfig().getParticleBlending()) {
            this->m_particle->setBlendFunc({GL_ONE, GL_ZERO});
        }

        BaseType::customSetup();
    }

    void firstSetup() override {
        BaseType::firstSetup();
        setupCustomObject();
    }

    void resetObject() override {
        BaseType::resetObject();
        resetCustomObject();
    }

    GameObject* gameObject() override final {
        return this;
    }
};

template <class BaseType>
requires (std::derived_from<BaseType, GameObject> && !std::derived_from<BaseType, RegisteredObjectByClass>)
class CustomObjectBase<BaseType, std::enable_if_t<std::is_base_of_v<CustomObjectInterface, BaseType>>> : public BaseType {};

#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>
#define $registerObject(NAME, BASE) NAME final : public CustomObjectBase<BASE>, public RegisterCustomObject<NAME, #NAME>