#pragma once
#include "CustomObjectConfig.hpp"

class CustomObjectInterface {
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    virtual GameObject* getObject() = 0;
    friend class CustomObjectConfig;
public:
    CustomObjectInterface();
    ~CustomObjectInterface();
protected:
    void setupCustomObject(GameObject*) const;
    void resetCustomObject(GameObject*) const;
    void activateCustomObject(GameObject*, GJBaseGameLayer*, PlayerObject*) const;
};

template <class BaseType>
requires std::derived_from<BaseType, GameObject>
class CustomObjectBase : public CustomObjectInterface, public BaseType {
public:
    virtual bool init(const CustomObjectConfig&& config) {
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

    GameObject* getObject() override {
        return this;
    }
};