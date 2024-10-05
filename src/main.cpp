#include "GPGauntletsPopup.hpp"

#include <Geode/modify/GauntletSelectLayer.hpp>
class $modify(GPGauntletSelectLayer, GauntletSelectLayer) {
    bool init(int idk) {
        if (!GauntletSelectLayer::init(idk)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto timeMenu = CCMenu::create();
        timeMenu->setPosition(winSize.width - 30.0f, winSize.height - 30.0f);
        timeMenu->setID("time-menu"_spr);
        timeMenu->setVisible(GameLevelManager::sharedState()->m_savedGauntlets->count() > 0);
        addChild(timeMenu, 2);

        auto timeButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_timeIcon_001.png", 1.0f, [this](CCMenuItemSpriteExtra* sender) {
            GPGauntletsPopup::create([this, sender] {
                if (m_scrollLayer) {
                    m_scrollLayer->removeFromParent();
                    m_scrollLayer = nullptr;
                }
                m_leftButton->setVisible(false);
                m_rightButton->setVisible(false);
                sender->setVisible(false);
                setupGauntlets();
                m_leftButton->setVisible(true);
                m_rightButton->setVisible(true);
                sender->setVisible(true);
            })->show();
        });
        timeButton->setID("time-button"_spr);
        timeMenu->addChild(timeButton);

        return true;
    }

    void setupGauntlets() {
        auto glm = GameLevelManager::sharedState();
        auto projectedIDs = Mod::get()->getSavedValue("projected-ids", std::vector<bool>(NUM_GAUNTLETS, false));
        projectedIDs.resize(NUM_GAUNTLETS, false);
        auto newSavedGauntlets = CCDictionary::create();
        newSavedGauntlets->retain();
        std::vector<bool> gauntletsAdded;
        for (int i = 0; i < NUM_GAUNTLETS; i++) {
            auto id = std::to_string(i + 1);
            if (!glm->m_savedGauntlets->objectForKey(id) && projectedIDs[i]) {
                gauntletsAdded.push_back(true);
                auto gauntlet = GJMapPack::create();
                gauntlet->m_packID = i + 1;
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

        for (int i = 0; i < NUM_GAUNTLETS; i++) {
            if (gauntletsAdded[i]) glm->m_savedGauntlets->removeObjectForKey(std::to_string(i + 1));
        }

        if (auto timeMenu = getChildByID("time-menu"_spr)) timeMenu->setVisible(glm->m_savedGauntlets->count() > 0);
    }

    void onPlay(CCObject* sender) {
        if (GameLevelManager::sharedState()->m_savedGauntlets->objectForKey(std::to_string(sender->getTag())))
            GauntletSelectLayer::onPlay(sender);
    }
};
