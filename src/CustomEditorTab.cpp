#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <alphalaneous.editortab_api/include/EditorTabs.hpp>

using namespace geode::prelude;

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        EditorTabs::addTab(this, TabType::BUILD, "custom-objects"_spr, [this](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* {
            CCLabelBMFont* textLabel = CCLabelBMFont::create("+", "bigFont.fnt");
            textLabel->setScale(0.5f);
            EditorTabUtils::setTabIcon(toggler, textLabel);

            auto objects = CCArray::create();
            objects->addObject(getCreateBtn(1, 4));
            objects->addObject(getCreateBtn(10, 4));
            objects->addObject(getCreateBtn(5001, 4));
            objects->addObject(getCreateBtn(5002, 4));
 
            return EditorTabUtils::createEditButtonBar(objects, ui);
        });

        return true;
    } // init
};