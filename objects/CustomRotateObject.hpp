#pragma once
#include "../include/CustomObjectBase.hpp"

#include <Geode/binding/EnhancedGameObject.hpp>

/**
 * This class is a custom object that has a rotating main sprite.
 * Some vanilla examples include the Saw and Gear hazard objects.
 * 
 * Use `CustomObjectConfig::setBoxRadius` to change the size of this object's hitbox.
 * This object only works properly as a `GameObjectType::Decoration` or `GameObjectType::Hazard` object!
 */
class $object(CustomRotateObject, EnhancedGameObject) {
public:
    bool init() override {
        if (!EnhancedGameObject::init(getConfig().getMainSprite().c_str())) return false;

        this->setDontDraw(true);
        this->addCustomChild(getConfig().getMainSprite(), cocos2d::CCPoint(0, 0), 0);

        if (getConfig().hasDetailSprite()) {
            auto detail = this->addCustomColorChild(getConfig().getDetailSprite());
            this->addInternalCustomColorChild(getConfig().getDetailSprite(), cocos2d::CCPoint(0, 0), 0);
            detail->setDontDraw(true);
        }

        if (getConfig().hasGlowSprite() && !this->m_editorEnabled && !this->m_hasNoGlow) {
            this->createGlow(getConfig().getGlowSprite());
            this->addInternalGlowChild(getConfig().getGlowSprite(), cocos2d::CCPoint(0, 0));
            this->m_glowSprite->setDontDraw(true);
        }

        this->m_objectType = GameObjectType::Decoration;
        this->m_objectRadius = this->m_width / 2;
        this->m_hasCustomRotation = true;
        this->m_rotationDelta = 360;
        this->m_rotationAngle = 360;

        // These are wrong in the bindings for now, I need to do a pr probably lol
        // this->m_glowColorIsLBG = true;
        this->m_customGlowColor = true; // m_glowColorIsLBG

        return true;
    }
};