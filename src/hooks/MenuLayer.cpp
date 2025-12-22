#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "CustomSpritesManager.hpp"

#define LODEPNG_NO_COMPILE_DECODER
#define LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
#define LODEPNG_NO_COMPILE_ERROR_TEXT
#include <lodepng.h>

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        ByteVector pixels;

        auto spr = CCSprite::createWithSpriteFrameName("edit_eMoveComBtn_001.png");
        auto size = CustomSpritesManager::getPixelDataFromSprite(spr, pixels);

        // Rotate image
        ByteVector image(pixels.size());
        for (int y = 0; y < size.height; y++) for (int x = 0; x < size.width; x++) {
            image[((size.width - x - 1) * size.height + y) * 4 + 0] = pixels[(y * size.width + x) * 4 + 0];
            image[((size.width - x - 1) * size.height + y) * 4 + 1] = pixels[(y * size.width + x) * 4 + 1];
            image[((size.width - x - 1) * size.height + y) * 4 + 2] = pixels[(y * size.width + x) * 4 + 2];
            image[((size.width - x - 1) * size.height + y) * 4 + 3] = pixels[(y * size.width + x) * 4 + 3];
        } // for
        size.swap();

        if (auto sprite = CustomSpritesManager::getSpriteFromPixelData(image, size)) {
            sprite->setAnchorPoint(CCPoint(0, 0));
            sprite->setScale(4);
            addChild(sprite);
        } // if

        // Reverse premultiply alpha
        for (uint32_t i = 0; i < image.size(); i += 4) {
            float a = image[i+3];
            if (a == 0 || a == 255) continue;

            image[i+0] *= 255 / a;
            image[i+1] *= 255 / a;
            image[i+2] *= 255 / a;
        } // for

        auto path = CustomSpritesManager::get()->getCacheDirectory();
        lodepng::encode((path + "sprite" + ".png"), image, size.width, size.height);

        return true;
    } // init
};