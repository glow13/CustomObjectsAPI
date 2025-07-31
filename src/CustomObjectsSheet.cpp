#include "CustomObjectsSheet.hpp"
#include "CustomObjectsAPI.hpp"

#include "rectpack2D/finders_interface.hpp"
#include "stb/stb_image_write.hpp"

bool CustomObjectsSheet::saveSpritesheetImage(std::string name, std::string path) const {
    auto render = CCRenderTexture::create(sheetSize.width / 4, sheetSize.height / 4);
    render->begin();

    for (auto obj : spritesCache) {
        auto rotatedSize = (obj.rotated ? CCSize(obj.size.height, obj.size.width) : obj.size) / 4;

        auto spr = CCSprite::createWithSpriteFrameName(obj.sourceFrame.c_str());
        spr->setPosition(CCPoint(obj.pos.x, sheetSize.height - obj.pos.y) / 4);
        spr->setAnchorPoint(obj.rotated ? CCPoint(0, 0) : CCPoint(0, 1));
        spr->setScaleX((rotatedSize.width) / spr->getContentWidth());
        spr->setScaleY((rotatedSize.height) / spr->getContentHeight());
        spr->setRotation(obj.rotated ? 90 : 0);
        spr->visit();
    } // for

    render->end();
    auto image = render->newCCImage();

    auto success = stbi_write_png(
        (path + name + ".png").c_str(),
        image->getWidth(),
        image->getHeight(),
        4,
        image->getData(),
        image->getWidth() * 4
    );

    image->release();
    return success;
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
    file << "\t\t<key>realTextureFileName</key>\n\t\t<string>custom-objects/"+name+".png</string>\n";
    file << "\t\t<key>size</key>\n\t\t<string>"+sizeString()+"</string>\n";
    file << "\t\t<key>textureFileName</key>\n\t\t<string>custom-objects/"+name+".png</string>\n";
    file << "\t</dict>\n</dict>\n</plist>";

    file.close();
    return true;
} // saveSpritesheetPlist

CustomObjectsSheet* CustomObjectsSheet::create(std::map<int, ModCustomObject> objects, Quality quality) {
    std::vector<CustomObjectSprite> sprites;
    float totalArea = 0;

    // Initialize sprites vector and find side lengths
    for (auto [id, obj] : objects) {
        auto spr = CustomObjectSprite(obj.sourceFrame, obj.spriteSize, quality);

        // Check if this sprite is already present
        if (std::find_if(sprites.begin(), sprites.end(), [spr](CustomObjectSprite other) {
            return spr.sourceFrame == other.sourceFrame && spr.size == other.size;
        }) < sprites.end()) continue;

        sprites.emplace_back(spr);
        totalArea += spr.size.width * spr.size.height;
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
    auto rectangles = std::vector<rect_xywhf>(sprites.size());

    float totalWidth = 0;
    for (auto spr : sprites) {
        rectangles.emplace_back(rect_xywhf(0, 0, spr.size.width + 2, spr.size.height + 2, false));
        totalWidth += std::max(spr.size.width, spr.size.height);
    } // for

    auto size = find_best_packing<empty_spaces<true>>(rectangles, make_finder_input(
        totalWidth / 2, -4,
        [](rect_xywhf&) { return callback_result::CONTINUE_PACKING; },
        [](rect_xywhf&) { return callback_result::ABORT_PACKING; },
        flipping_option::ENABLED
    ));

    for (auto &spr : sprites) for (int i = 0; i < rectangles.size(); i++) {
        if (spr.size.width == rectangles[i].w - 2 && spr.size.height == rectangles[i].h - 2) {
            spr.pos = CCPoint(rectangles[i].x, rectangles[i].y);
        } else if (spr.size.width == rectangles[i].h - 2 && spr.size.height == rectangles[i].w - 2) {
            spr.pos = CCPoint(rectangles[i].x, rectangles[i].y);
            spr.size.swap();
            spr.rotated = true;
        } else continue;

        rectangles.erase(rectangles.begin() + i);
        break;
    } // for

    return CCSize(std::ceil((size.w - 2) / 4), std::ceil((size.h - 2) / 4)) * 4;
} // binPacking
