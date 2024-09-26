#include "GPGauntletsPopup.hpp"

#include <Geode/modify/GauntletSelectLayer.hpp>
class $modify(GPGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        CCMenuItemSpriteExtra* m_timeButton;
    };

    bool init(int idk) {
        if (!GauntletSelectLayer::init(idk)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto timeMenu = CCMenu::create();
        timeMenu->setPosition(winSize.width - 30.0f, winSize.height - 30.0f);
        timeMenu->setID("time-menu"_spr);
        addChild(timeMenu, 2);

        auto f = m_fields.self();
        f->m_timeButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_timeIcon_001.png", 1.0f, [this](auto) {
            GPGauntletsPopup::create([this] {
                if (m_scrollLayer) {
                    m_scrollLayer->removeFromParent();
                    m_scrollLayer = nullptr;
                }
                m_leftButton->setVisible(false);
                m_rightButton->setVisible(false);
                auto bottomRightMenu = getChildByID("bottom-right-menu");
                if (bottomRightMenu) for (auto child : CCArrayExt<CCNode*>(bottomRightMenu->getChildren())) {
                    if (child != m_refreshButton) child->removeFromParent();
                }
                setupGauntlets();
                m_leftButton->setVisible(true);
                m_rightButton->setVisible(true);
            })->show();
        });
        f->m_timeButton->setID("time-button"_spr);
        timeMenu->addChild(f->m_timeButton);

        return true;
    }

    void setupGauntlets() {
        auto f = m_fields.self();
        if (f->m_timeButton) f->m_timeButton->setEnabled(false);

        auto glm = GameLevelManager::sharedState();
        std::vector<bool> projectedIDs;
        auto newSavedGauntlets = CCDictionary::create();
        newSavedGauntlets->retain();
        std::vector<bool> gauntletsAdded;
        for (int i = 1; i < 53; i++) {
            auto id = std::to_string(i);
            if (!glm->m_savedGauntlets->objectForKey(id) && GPGauntletsPopup::projectedIDs[i - 1]) {
                gauntletsAdded.push_back(true);
                auto gauntlet = GJMapPack::create();
                gauntlet->m_packID = i;
                gauntlet->m_levels = CCArray::create();
                gauntlet->m_levels->retain();
                glm->m_savedGauntlets->setObject(gauntlet, id);
            }
            else gauntletsAdded.push_back(false);
            if (glm->m_savedGauntlets->objectForKey(id)) newSavedGauntlets->setObject(glm->m_savedGauntlets->objectForKey(id), id);
        }

        glm->m_savedGauntlets->release();
        glm->m_savedGauntlets = newSavedGauntlets;

        GauntletSelectLayer::setupGauntlets();

        for (int i = 0; i < 52; i++) {
            if (gauntletsAdded[i]) glm->m_savedGauntlets->removeObjectForKey(std::to_string(i + 1));
        }

        if (f->m_timeButton) f->m_timeButton->setEnabled(true);
    }

    void onPlay(CCObject* sender) {
        auto glm = GameLevelManager::sharedState();
        if (glm->m_savedGauntlets->objectForKey(std::to_string(sender->getTag())))
            GauntletSelectLayer::onPlay(sender);
    }
};
