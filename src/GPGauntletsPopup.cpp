#include "GPGauntletsPopup.hpp"

using namespace geode::prelude;

TableNode* TableNode::create(int columns, int rows) {
    auto ret = new TableNode();
    if (ret->init(columns, rows)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TableNode::init(int columns, int rows) {
    if (!CCNode::init()) return false;

    setAnchorPoint({ 0.5f, 0.5f });
    m_menus = CCArray::create();
    m_menus->retain();
    m_columns = columns;
    m_rows = rows;

    return true;
}

void TableNode::setColumnLayout(AxisLayout* columnLayout) {
    m_columnLayout = columnLayout;
    setLayout(m_columnLayout);
}

void TableNode::setRowLayout(AxisLayout* rowLayout) {
    m_rowLayout = rowLayout;
    for (auto menu : CCArrayExt<CCMenu*>(m_menus)) {
        menu->setLayout(m_rowLayout);
    }
}

void TableNode::setRowHeight(float rowHeight) {
    m_rowHeight = rowHeight;
    for (auto menu : CCArrayExt<CCMenu*>(m_menus)) {
        menu->setContentSize({ m_obContentSize.width, rowHeight });
    }
}

void TableNode::updateAllLayouts() {
    for (auto menu : CCArrayExt<CCMenu*>(m_menus)) {
        menu->updateLayout();
    }
    updateLayout();
}

void TableNode::addButton(CCMenuItemSpriteExtra* button) {
    CCMenu* menu = nullptr;
    if (m_menus->count() <= 0 || static_cast<CCMenu*>(m_menus->objectAtIndex(m_menus->count() - 1))->getChildrenCount() >= m_columns) {
        menu = CCMenu::create();
        menu->setContentSize({ m_obContentSize.width, m_rowHeight });
        menu->setLayout(m_rowLayout);
        addChild(menu);
        m_menus->addObject(menu);
    } else menu = static_cast<CCMenu*>(m_menus->objectAtIndex(m_menus->count() - 1));

    menu->addChild(button);
}

TableNode::~TableNode() {
    CC_SAFE_RELEASE(m_menus);
}

GPGauntletsPopup* GPGauntletsPopup::create(GauntletCallback callback) {
    auto ret = new GPGauntletsPopup();
    if (ret->initAnchored(350.0f, 270.0f, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GPGauntletsPopup::setup(GauntletCallback callback) {
    setTitle("Project Gauntlets");

    m_enabledGauntlets = Mod::get()->getSavedValue("projected-ids", std::vector<bool>(NUM_GAUNTLETS, false));
    m_enabledGauntlets.resize(NUM_GAUNTLETS, false);

    auto savedGauntlets = GameLevelManager::get()->m_savedGauntlets;
    auto table = TableNode::create(6, (NUM_GAUNTLETS + 5 - savedGauntlets->count()) / 6);
    table->setColumnLayout(ColumnLayout::create()->setGap(7.5f)->setAxisReverse(true));
    table->setRowLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    table->setRowHeight(60.0f);
    table->setContentSize({ 350.0f, 190.0f });
    table->setPosition({ 175.0f, 140.0f });
    m_mainLayer->addChild(table);

    for (int i = 0; i < NUM_GAUNTLETS; i++) {
        if (savedGauntlets->objectForKey(std::to_string(i + 1))) continue;

        auto gauntletSprite = GauntletSprite::create((GauntletType)(i + 1), false);
        gauntletSprite->setScale(0.7f);
        auto innerSprite = gauntletSprite->getChildByType<CCSprite>(0);
        innerSprite->setColor(m_enabledGauntlets[i] ? ccColor3B { 255, 255, 255 } : ccColor3B { 125, 125, 125 });
        auto gauntletButton = CCMenuItemExt::createSpriteExtra(gauntletSprite, [this, i, innerSprite](auto) {
            m_enabledGauntlets[i] = !m_enabledGauntlets[i];
            innerSprite->setColor(m_enabledGauntlets[i] ? ccColor3B { 255, 255, 255 } : ccColor3B { 125, 125, 125 });
        });
        table->addButton(gauntletButton);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", "goldFont.fnt", "GJ_button_01.png", 0.8f), [this, callback](auto) {
        Mod::get()->setSavedValue("projected-ids", m_enabledGauntlets);
        callback();
        onClose(nullptr);
    });
    confirmButton->setPosition({ 175.0f, 25.0f });
    m_buttonMenu->addChild(confirmButton);

    return true;
}
