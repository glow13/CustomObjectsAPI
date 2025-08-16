#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// CustomObject config defaults
#define BOX_SIZE_DEFAULT CCSize(0, 0)
#define SPRITE_SIZE_DEFAULT CCSize(30, 30)
#define OBJECT_TYPE_DEFAULT (GameObjectType)(-1)

struct CustomObject {
    int id;
    std::string frame;
    std::string sourceFrame;
    std::function<GameObject*(CustomObject)> createFunction;

    GameObject* create() { return createFunction(*this); }

    CustomObject() : frame(""), sourceFrame(""), id(0), spriteSize(SPRITE_SIZE_DEFAULT) {}
    CustomObject(std::string frame, std::string mod, int id, CCSize size, std::function<GameObject*(CustomObject)> create) {
        auto name = frame.substr(frame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, name);

        this->id = id;
        this->sourceFrame = frame;
        this->spriteSize = size;
        this->createFunction = create;
    } // CustomObject

    // CustomObject config
    CCSize boxSize = BOX_SIZE_DEFAULT;
    CCSize spriteSize = SPRITE_SIZE_DEFAULT;
    GameObjectType objectType = OBJECT_TYPE_DEFAULT;

    CustomObject setSize(int w, int h) { spriteSize = CCSize(w, h); return *this; }
    CustomObject setBoxSize(int w, int h) { boxSize = CCSize(w, h); return *this; }
    CustomObject setObjectType(GameObjectType type) { objectType = type; return *this; }

    void applyBoxSize(GameObject* obj) { if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; } }
    void applyObjectType(GameObject* obj) { if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType; }
};