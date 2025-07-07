#include "CustomEffectGameObject.hpp"

CustomEffectGameObject* CustomEffectGameObject::create(int id) {
    auto obj = new CustomEffectGameObject();

    auto toolbox = ObjectToolbox::sharedState();
    auto spr = toolbox->intKeyToFrame(id);

    if (obj->init(spr) && obj->customInit()) {
        obj->m_objectID = id;
        obj->m_parentMode = 10;
        obj->autorelease();
        return obj;
    } // if

    delete obj;
    return nullptr;
} // create

bool CustomEffectGameObject::customInit() {
    m_objectType = GameObjectType::Modifier;
    m_isTouchTriggered = true;
    m_isMultiTriggered = true;
    m_isTrigger = true;
    return true;
} // init

void CustomEffectGameObject::triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) {
    log::info("wow get triggered");
    Notification::create("This is a custom trigger!", NotificationIcon::Success, 0.5)->show();
} // triggerObject