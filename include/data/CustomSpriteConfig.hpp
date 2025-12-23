#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_MOD_ID

#define SPRITE1(name) SPRITE5(name, 0, 0, 0, 0)
#define SPRITE2(name, s) SPRITE5(name, 0, 0, s, s)
#define SPRITE3(name, w, h) SPRITE5(name, 0, 0, w, h)

#define SPRITE5(name,x,y,w,h) GEODE_CONCAT("custom-objects/",GEODE_MOD_ID)GEODE_STR(/x.y.w.h/)name##".png"
#define $sprite(...) (GEODE_INVOKE(GEODE_CONCAT(SPRITE,GEODE_NUMBER_OF_ARGS(__VA_ARGS__)),__VA_ARGS__))

#endif

class CustomObjectsMod;
class CustomObjectConfigBase;
class CustomObjectsManager;
class CustomSpritesManager;

struct CustomSpriteConfig {
private:
    CustomObjectsMod* mod;
    CustomObjectConfigBase* object;

    std::string frameName;
    std::string sourceFrame;
    CCPoint offset;
    CCSize size;

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

private:
    void generateFrame();

    friend CustomObjectsManager;
    friend CustomSpritesManager;
};