#pragma once
#include <Geode/Geode.hpp>

#include "library/pcg/pcg_random.hpp"

#include "object/CustomGameObject.hpp"

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
    CustomObject(std::string frame, std::string mod, int id, CCSize size, std::function<GameObject*(int)> create) : sourceFrame(frame), id(id), spriteSize(size), boxSize(CCSize(30, 30)), createFunction(create) {
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, frame.substr(frame.find("/") + 1));
    } // CustomObject

    GameObject* create() { return createFunction(id); }
};

struct CustomObjectsMod {
    geode::Mod* mod;

    std::string modID;
    std::string dev;
    std::string name;

    int objectID;
    std::vector<CustomObject> objects;

    CustomObjectsMod(geode::Mod* mod, short offset) : mod(mod), modID(mod->getID()) {
        int pos = modID.find(".");
        dev = modID.substr(0, pos);
        name = modID.substr(pos + 1);

        int devNum = 0;
        for (int i = 0, f = 0; i < dev.length() && f < 3; i++) {
            if (dev[i] < 'a' || dev[i] > 'z') continue;
            devNum = (devNum * 26) + (dev[i] - 'a'), f++;
        } // for

        int modNum = 0;
        for (int i = 0, f = 0; i < name.length() && f < 6; i++) {
            if (name[i] < 'a' || name[i] > 'z') continue;
            modNum = (modNum * 26) + (name[i] - 'a'), f++;
        } // for

        pcg32 rng(modNum);
        for (int i = 0; i < devNum + offset; i++) rng();
        objectID = (pcg_extras::bounded_rand(rng, 19799) + 200) * 50;
    } // CustomObjectsMod

    void registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObjectBase::create) {
        auto sprName = spr.substr(spr.find("/") + 1);
        int id = objectID + objects.size();

        objects.emplace_back(CustomObject(spr, modID, id, size * 30, create));
        log::debug("Registered custom object with id {}", id);
    } // registerCustomObject

    void registerCustomObject(std::string spr, std::function<GameObject*(int)> create = CustomGameObjectBase::create) { registerCustomObject(spr, CCSize(1, 1), create); }
};

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    CCPoint pos;
    CCSize size;
    bool rotated;

    CustomObjectSprite() : frame(""), sourceFrame(""), pos(CCPoint(0, 0)), size(CCSize(30, 30)), rotated(false) {}
    CustomObjectSprite(std::string frame, std::string sourceFrame, CCSize size, Quality quality) : frame(frame), sourceFrame(sourceFrame), pos(CCPoint(0, 0)), size(size * quality), rotated(false) {}

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