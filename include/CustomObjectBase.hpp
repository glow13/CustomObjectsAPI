#pragma once
#include "CustomObjectConfig.hpp"

class CustomObjectInterface {
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    template <class, StringConcatModIDSlash>
    friend class RegisterCustomObject;
    friend class CustomObjectConfig;
public:
    CustomObjectInterface();
    virtual ~CustomObjectInterface();
    virtual bool init() = 0;
protected:
    void setupCustomObject(GameObject*) const;
    void resetCustomObject(GameObject*) const;
    void activateCustomObject(GameObject*, GJBaseGameLayer*, PlayerObject*) const;

    const CustomObjectConfig&& getConfig() const;
    virtual GameObject* gameObject() = 0;
};

template <class BaseType>
requires std::derived_from<BaseType, GameObject>
class CustomObjectBase : public CustomObjectInterface, public BaseType {
public:
    virtual bool init() override {
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

    void customObjectSetup(gd::vector<gd::string>& propValues, gd::vector<void*>& propIsPresent) override final {
        BaseType::customObjectSetup(propValues, propIsPresent);

        // TODO object properties

        setupCustomObject();
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

    GameObject* gameObject() override {
        return this;
    }
};

#define $base(NAME, BASE) NAME : public CustomObjectBase<BASE>
#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>, public RegisterCustomObject<NAME, #NAME>