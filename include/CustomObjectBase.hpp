#pragma once
#include "CustomObjectConfig.hpp"

template <class BaseType>
class CustomObjectBase : public BaseType {
public:
    bool init(const CustomObjectConfig* config) {
        if (!BaseType::init(config->getMainSprite().c_str())) return false;

        // Add sprites to custom object
        if (!config->hasMainSprite()) this->setDontDraw(true);
        if (config->hasDetailSprite()) this->addCustomColorChild(config->getDetailSprite());

        // Add glow to custom object
        if (this->m_editorEnabled || this->m_hasNoGlow) return true;
        if (config->hasGlowSprite()) this->createGlow(config->getGlowSprite());

        return true;
    }
};