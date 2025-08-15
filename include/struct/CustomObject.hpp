#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomObject {
    int id;
    std::string frame;
    std::string sourceFrame;
    std::function<GameObject*(int)> createFunction;

    CCSize boxSize = CCSize(30, 30);
    CCSize spriteSize = CCSize(30, 30);
    GameObjectType objectType = GameObjectType::Solid;

    CustomObject() {}
    CustomObject(std::string frame, std::function<GameObject*(int)> create) : sourceFrame(frame), createFunction(create) {}

    CustomObject(std::string frame, std::string mod, int id, CCSize size, std::function<GameObject*(int)> create) {
        auto name = frame.substr(frame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, name);

        this->id = id;
        this->sourceFrame = frame;
        this->spriteSize = size;
        this->boxSize = size;
        this->createFunction = create;
    } // CustomObject

    CustomObject setSpriteSize(int w, int h) { spriteSize = CCSize(w, h); return *this; }
    CustomObject setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObject setObjectType(GameObjectType type) { objectType = type; return *this; }

    GameObject* create() {
        auto obj = createFunction(id);

        obj->m_width = boxSize.width;
        obj->m_height = boxSize.height;
        obj->m_objectType = objectType;

        return obj;
    } // create
};