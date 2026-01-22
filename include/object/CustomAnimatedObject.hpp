#pragma once

#include "../CustomObjectBase.hpp"

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