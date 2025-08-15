#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomObject {
    int id;
    std::string frame;
    std::string sourceFrame;
    std::function<GameObject*(CustomObject)> createFunction;

    CCSize boxSize;
    CCSize spriteSize;
    GameObjectType objectType = GameObjectType::Solid;

    CustomObject() {}
    CustomObject(std::string frame, std::function<GameObject*(CustomObject)> create = 0) : sourceFrame(frame), spriteSize(CCSize(30, 30)), createFunction(create) {}

    CustomObject(std::string frame, std::string mod, int id, CCSize size, std::function<GameObject*(CustomObject)> create) {
        auto name = frame.substr(frame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, name);

        this->id = id;
        this->sourceFrame = frame;
        this->spriteSize = size;
        this->createFunction = create;
    } // CustomObject

    CustomObject setSize(int w, int h) { spriteSize = CCSize(w, h); return *this; }
    CustomObject setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObject setObjectType(GameObjectType type) { objectType = type; return *this; }

    void applyBoxSize(GameObject* obj) { if (!boxSize.isZero()) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; } }
    void applyObjectType(GameObject* obj) { if ((int)objectType > 0) obj->m_objectType = objectType; }

    GameObject* create() { return createFunction(*this); }
};