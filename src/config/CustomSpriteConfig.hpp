#pragma once

enum Quality : int;
struct SheetInfo;

class CustomSpriteConfig final {
    struct Impl;
    std::unique_ptr<Impl> m_impl;
public:
    CustomSpriteConfig(class CustomObjectConfig*, std::string, int, int, int, int, bool);
    CustomSpriteConfig(geode::Mod*, std::string, int, int, int, int, bool);
    ~CustomSpriteConfig();

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;
    SheetInfo getSheetInfo(Quality) const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};