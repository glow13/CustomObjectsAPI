#pragma once
#include "../include/object/CustomObjectBase.hpp"

#include <Geode/binding/EnhancedGameObject.hpp>

/**
 * This class is a custom object that can play a frame-by-frame animation as its main sprite.
 * Some vanilla examples include the Flame and Water decoration objects.
 * 
 * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly:
 * They must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
 * 
 * Use `CustomObjectConfig::setFramesCount` and `CustomObjectConfig::setFrameTime` to configure the object's animation.
 * You can use `CustomObjectsAPI::registerCustomAnimationSprites` to quickly register every sprite in your animation.
 */
template <class ObjectType>
class $base(CustomAnimatedObject, EnhancedGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EnhancedGameObject>;
    using CustomAnimatedObject = CustomAnimatedObjectBase<ObjectType>;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::Decoration;
        this->m_hasCustomAnimation = config->hasCustomAnimation();

        return true;
    } // init
};

class $generic(CustomAnimatedObject);