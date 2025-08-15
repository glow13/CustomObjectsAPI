#pragma once
#include <Geode/Geode.hpp>

#include "library/pcg/pcg_random.hpp"

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

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

    void registerCustomObject(CustomObject obj) {
        auto name = obj.sourceFrame.substr(obj.sourceFrame.find("/") + 1);
        obj.frame = fmt::format("custom-objects/{}/{}/{}/{}", modID, obj.spriteSize.width, obj.spriteSize.height, name);
        obj.id = objectID + objects.size();

        objects.emplace_back(obj);
        log::debug("Registered custom object with id {}", obj.id);
    } // registerCustomObject

    void registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObjectBase::create) {
        auto sprName = spr.substr(spr.find("/") + 1);
        int id = objectID + objects.size();

        objects.emplace_back(CustomObject(spr, modID, id, size * 30, create));
        log::debug("Registered custom object with id {}", id);
    } // registerCustomObject

    void registerCustomObject(std::string spr, std::function<GameObject*(int)> create = CustomGameObjectBase::create) { registerCustomObject(spr, CCSize(1, 1), create); }
};