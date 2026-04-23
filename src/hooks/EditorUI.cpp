#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

#include "CustomObjectsManager.hpp"
#include "data/CustomObjectConfig.hpp"

using namespace geode::prelude;

class $modify(CustomEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;
        if (CustomObjectsManager::get()->getTotalCustomObjectsCount() == 0) return true;

        alpha::editor_tabs::addTab("custom-objects"_spr, alpha::editor_tabs::BUILD, [this] {
            std::vector<Ref<CCNode>> buttons;
            for (auto [mod, objs] : CustomObjectsManager::get()->getEditorTabLayout())
                for (auto [p, id] : objs) buttons.push_back(static_cast<CCNode*>(getCreateBtn(id, 4)));
            return alpha::editor_tabs::createEditButtonBar(buttons);
        }, [] {
            CCLabelBMFont* textLabel = CCLabelBMFont::create("+", "bigFont.fnt");
            textLabel->setScale(0.5f);
            return textLabel;
        });

        return true;
    } // init

    bool editButtonUsable() {
        int objectID, classType, objectType;
        getGroupInfo(m_selectedObject, m_selectedObjects, objectID, classType, objectType);

        if (objectID >= 10000) {
            auto obj = CustomObjectsManager::get()->getCustomObjectByID(objectID);
            if (obj && obj->hasEditObjectFunction()) return true;
        } // if

        return EditorUI::editButtonUsable();
    } // editButtonUsable

    bool editButton2Usable() {
        int objectID, classType, objectType;
        getGroupInfo(m_selectedObject, m_selectedObjects, objectID, classType, objectType);

        if (objectID >= 10000) {
            auto obj = CustomObjectsManager::get()->getCustomObjectByID(objectID);
            if (obj && obj->hasEditSpecialFunction()) return true;
        } // if

        return EditorUI::editButton2Usable();
    } // editButton2Usable

    void editObject(CCObject* p0) {
        int objectID, classType, objectType;
        getGroupInfo(m_selectedObject, m_selectedObjects, objectID, classType, objectType);

        if (objectID >= 10000) {
            auto obj = CustomObjectsManager::get()->getCustomObjectByID(objectID);
            if (obj && obj->hasEditObjectFunction()) {
                obj->customEditObject(m_selectedObject, m_selectedObjects);
            } else EditorUI::editObject(p0);
        } else EditorUI::editObject(p0);
    } // editObject

    void editObjectSpecial(int p0) {
        int objectID, classType, objectType;
        getGroupInfo(m_selectedObject, m_selectedObjects, objectID, classType, objectType);

        if (objectID >= 10000) {
            auto obj = CustomObjectsManager::get()->getCustomObjectByID(objectID);
            if (obj && obj->hasEditSpecialFunction()) {
                obj->customEditSpecial(m_selectedObject, m_selectedObjects);
            } else EditorUI::editObjectSpecial(p0);
        } else EditorUI::editObjectSpecial(p0);
    } // editObjectSpecial

    CCPoint offsetForKey(int id) {
        if (id < 10000) return EditorUI::offsetForKey(id);
        return CustomObjectsManager::get()->getCustomObjectByID(id)->objectOffset;
    } // offsetForKey
};