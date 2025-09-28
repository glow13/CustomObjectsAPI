#include "CustomObjectsSheet.hpp"

#define LODEPNG_NO_COMPILE_DECODER
#define LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
#define LODEPNG_NO_COMPILE_ERROR_TEXT

#include <finders_interface.h>
#include <lodepng.h>

bool CustomObjectsSheet::saveSpritesheetImage(std::string name, std::string path) const {
    int csf = CCDirector::get()->getContentScaleFactor();
    auto render = CCRenderTexture::create(sheetSize.width / csf, sheetSize.height / csf);
    render->begin();

    // Add each sprite to the rendered image
    for (auto obj : spritesCache) {
        auto rotatedWidth = ((obj.rect.flipped ? obj.rect.h : obj.rect.w) - SPRITE_BUFFER) / csf;
        auto rotatedHeight = ((obj.rect.flipped ? obj.rect.w : obj.rect.h) - SPRITE_BUFFER) / csf;

        auto spr = CCSprite::createWithSpriteFrameName(obj.sourceFrame.c_str());
        spr->setPosition(CCPoint(obj.rect.x, sheetSize.height - obj.rect.y) / csf);
        spr->setAnchorPoint(obj.rect.flipped ? CCPoint(0, 0) : CCPoint(0, 1));
        spr->setScaleX(rotatedWidth / spr->getContentWidth());
        spr->setScaleY(rotatedHeight / spr->getContentHeight());
        spr->setRotation(obj.rect.flipped ? 90 : 0);
        spr->visit();
    } // for

    // Save the rendered image pixel data
    auto& s = render->m_pTexture->getContentSizeInPixels();
    unsigned int w = s.width, h = s.height;
    auto buffer = new uint8_t[w * h * 4];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    render->end();
    render->release();

    // Flip the image
    std::vector<uint8_t> data(w * h * 4);
    for (int i = 0; i < h; i++) memcpy(&data[i * w * 4], &buffer[(h - i - 1) * w * 4], w * 4);
    delete[] buffer;

    return lodepng::encode((path + name + ".png"), data, w, h) == 0;
} // saveSpritesheetImage

bool CustomObjectsSheet::saveSpritesheetPlist(std::string name, std::string path) const {
    auto fullPath = path + name + ".plist";
    auto file = std::ofstream(fullPath);

    if (!file.is_open()) {
        log::error("Failed to create the spritesheet plist file \"{}.plist\"", name);
        return false;
    } // if

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
    file << "<plist version=\"1.0\">\n";
    file << "<dict>\n\t<key>frames</key>\n\t<dict>\n";

    for (auto spr : spritesCache) {
        file << "\t\t<key>"+spr.frame+"</key>\n\t\t<dict>\n";
        file << "\t\t\t<key>spriteOffset</key>\n\t\t\t<string>{0,0}</string>\n";
        file << "\t\t\t<key>spriteSize</key>\n\t\t\t<string>"+spr.sizeString()+"</string>\n";
        file << "\t\t\t<key>spriteSourceSize</key>\n\t\t\t<string>"+spr.sizeString()+"</string>\n";
        file << "\t\t\t<key>textureRect</key>\n\t\t\t<string>"+spr.rectString()+"</string>\n";
        file << "\t\t\t<key>textureRotated</key>\n\t\t\t"+spr.rotatedString()+"\n\t\t</dict>\n";
    } // for

    file << "\t</dict>\n\t<key>metadata</key>\n\t<dict>\n";
    file << "\t\t<key>format</key>\n\t\t<integer>3</integer>\n";
    file << "\t\t<key>realTextureFileName</key>\n\t\t<string>"+name+".png</string>\n";
    file << "\t\t<key>size</key>\n\t\t<string>"+sizeString()+"</string>\n";
    file << "\t\t<key>textureFileName</key>\n\t\t<string>"+name+".png</string>\n";
    file << "\t</dict>\n</dict>\n</plist>";

    file.close();
    return true;
} // saveSpritesheetPlist

CustomObjectsSheet* CustomObjectsSheet::create(const std::vector<CustomSpriteConfig>& customSprites, Quality quality) {
    std::vector<CustomObjectSprite> sprites;
    float totalArea = 0;

    // Initialize sprites vector and find side lengths
    for (const CustomSpriteConfig& sprite : customSprites) {
        auto spr = CustomObjectSprite(sprite.frame, sprite.sourceFrame, sprite.size, quality);

        sprites.emplace_back(spr);
        totalArea += spr.rect.w * spr.rect.h;
    } // for

    // Run the bin packing algorithm
    auto size = CustomObjectsSheet::binPacking(sprites);

    // Create the spritesheet object
    auto sheet = new CustomObjectsSheet();
    sheet->spritesCache = sprites;
    sheet->sheetSize = size;
    sheet->autorelease();

    // Calculate total coverage
    float coverage = std::min(totalArea / (size.width * size.height) * 100, 100.0f);
    log::debug("Generated the spritesheet with dimensions ({} x {}) and {}% total coverage", size.width, size.height, coverage);

    // Return the final spritesheet object
    return sheet;
} // create

CCSize CustomObjectsSheet::binPacking(std::vector<CustomObjectSprite> &sprites) {
    using namespace rectpack2D;

    float totalWidth = 0;
    for (auto spr : sprites) totalWidth += std::max(spr.rect.w, spr.rect.h);

    auto size = find_best_packing<empty_spaces<true>>(sprites, make_finder_input(
        totalWidth / 2, -4,
        [](rect_xywhf&) { return callback_result::CONTINUE_PACKING; },
        [](rect_xywhf&) { return callback_result::ABORT_PACKING; },
        flipping_option::ENABLED
    ));

    return CCSize(std::ceil(size.w / 4), std::ceil(size.h / 4)) * 4;
} // binPacking