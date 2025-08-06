#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum Quality {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct CustomObject {
    std::string frame;
    std::string sourceFrame;
    std::function<GameObject*(int)> createFunction;

    int id;
    CCSize boxSize;
    CCSize spriteSize;

    CustomObject() : frame(""), sourceFrame(""), id(0), spriteSize(CCSize(30, 30)), boxSize(CCSize(30, 30)) {}
    CustomObject(std::string frame, int id, CCSize size, std::function<GameObject*(int)> create) : sourceFrame(frame), id(id), spriteSize(size), boxSize(CCSize(30, 30)), createFunction(create) {
        auto mod = frame.substr(0, frame.find("/"));
        auto spr = frame.substr(frame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, spr);
    } // CustomObject

    GameObject* create() { return createFunction(id); }
};

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    CCPoint pos;
    CCSize size;
    bool rotated;

    CustomObjectSprite() : frame(""), sourceFrame(""), pos(CCPoint(0, 0)), size(CCSize(30, 30)), rotated(false) {}
    CustomObjectSprite(std::string sourceFrame, CCSize size, Quality quality) : sourceFrame(sourceFrame), pos(CCPoint(0, 0)), size(size * quality), rotated(false) {
        auto mod = sourceFrame.substr(0, sourceFrame.find("/"));
        auto spr = sourceFrame.substr(sourceFrame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, spr);
    } // CustomObjectSprite

    std::string sizeString() const {
        auto rotSize = rotated ? CCSize(size.height, size.width) : size;
        return "{" + fmt::format("{},{}", rotSize.width, rotSize.height) + "}";
    } // sizeString

    std::string rectString() const {
        return "{{" + fmt::format("{},{}", pos.x, pos.y) + "}," + sizeString() + "}";
    } // rectString

    std::string rotatedString() const {
        return rotated ? "<true/>" : "<false/>";
    } // rotatedString
};