#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "CustomObjectsAPI.hpp"
#include "object/CustomRingGameObject.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {
    void ringJump(RingObject* obj, bool idk) {
        PlayerObject::ringJump(obj, idk);
        if (obj->m_objectID >= 10000 && m_touchedCustomRing) static_cast<CustomRingGameObject*>(obj)->onJump();
    } // ringJump
};