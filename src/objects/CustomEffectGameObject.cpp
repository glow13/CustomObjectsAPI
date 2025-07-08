#include "CustomEffectGameObject.hpp"

CustomEffectGameObject* CustomEffectGameObject::create(int id) {
    auto obj = new CustomEffectGameObject();
    if (obj->init(id)) return obj;

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

void CustomEffectGameObject::setToDefault() {
    m_bouncePower = 8;
} // setToDefault

void CustomEffectGameObject::onTrigger(GJBaseGameLayer* playLayer) {
    playLayer->m_player1->setYVelocity(m_bouncePower, 1);
    m_bouncePower++;
} // onTrigger