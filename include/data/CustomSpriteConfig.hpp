#pragma once
#include <Geode/Geode.hpp>

#include "../CustomObjectsDLL.hpp"

using namespace geode::prelude;

class CustomObjectsMod;
class CustomObjectConfigBase;

struct CUSTOM_OBJECTS_DLL CustomSpriteConfig {
private:
    CustomObjectsMod* mod;
    CustomObjectConfigBase* object;

    std::string frameName;
    std::string sourceFrame;
    CCPoint offset;
    CCSize size;

    void generateFrame();

    friend class CustomObjectsManager;
    friend class CustomSpritesManager;

public:
    CustomSpriteConfig() : frameName(""), sourceFrame(""), offset(CCPointZero), size(CCSizeZero) {}
    CustomSpriteConfig(CustomObjectsMod* mod, CustomObjectConfigBase* object, std::string frame, int offX, int offY, int sizeW, int sizeH);

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    CCPoint getOffset() const;
    CCSize getSize() const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};