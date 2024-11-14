#define NUM_GAUNTLETS 54

typedef std::function<void()> const& GauntletCallback;

class TableNode : public cocos2d::CCNode {
protected:
    cocos2d::CCArray* m_menus;
    geode::AxisLayout* m_columnLayout;
    geode::AxisLayout* m_rowLayout;
    int m_columns;
    int m_rows;
    float m_rowHeight;

    bool init(int columns, int rows);
public:
    static TableNode* create(int columns, int rows);

    void setColumnLayout(geode::AxisLayout*);
    void setRowLayout(geode::AxisLayout*);
    void setRowHeight(float);
    void updateAllLayouts();
    void addButton(CCMenuItemSpriteExtra*);

    ~TableNode() override;
};

class GPGauntletsPopup : public geode::Popup<GauntletCallback> {
protected:
    std::vector<bool> m_enabledGauntlets;

    bool setup(GauntletCallback) override;
public:
    static GPGauntletsPopup* create(GauntletCallback);
};
