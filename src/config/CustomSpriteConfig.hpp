#pragma once
#include <rect_structs.h>

class CustomSpriteConfig final {
    struct SheetInfo final {
    private:
        rectpack2D::rect_wh m_offset;
        rectpack2D::rect_wh m_size;
        rectpack2D::rect_xywh m_trim;
        rectpack2D::rect_xywhf m_rect;
    public:
        std::string offString() const;
        std::string sizeString() const;
        std::string rectString() const;
        std::string sourceString() const;
        std::string rotatedString() const;

        friend CustomSpriteConfig;
        SheetInfo(int, int, int, int);
        SheetInfo(const rectpack2D::rect_xywhf&);

        inline auto& get_rect() { return m_rect; }
        inline const auto& get_rect() const { return m_rect; }
    };

    struct Impl;
    std::unique_ptr<Impl> m_impl;
public:
    CustomSpriteConfig(class CustomObjectConfig*, std::string, int, int, int, int, bool);
    ~CustomSpriteConfig();

    std::string getModID() const;
    std::string getFrameName() const;
    std::string getSourceFrame() const;

    bool isCustomSprite() const;
    bool isAnimationFrame() const;
    bool isModTrigger() const;
};