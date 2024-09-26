#include <Geode/Geode.hpp>

using namespace geode::prelude;

typedef MiniFunction<void()> GauntletCallback;

class TableNode : public CCNode {
protected:
    CCArray* m_menus;
    AxisLayout* m_columnLayout;
    AxisLayout* m_rowLayout;
    int m_columns;
    int m_rows;
    float m_rowHeight;

    bool init(int columns, int rows);
public:
    static TableNode* create(int columns, int rows);

    void setColumnLayout(AxisLayout*);
    void setRowLayout(AxisLayout*);
    void setRowHeight(float);
    void updateAllLayouts();
    void addButton(CCMenuItemSpriteExtra*);

    ~TableNode() override;
};

class GPGauntletsPopup : public Popup<GauntletCallback> {
protected:
    CCArray* m_gauntletButtons;

    bool setup(GauntletCallback) override;
public:
    inline static std::vector<bool> projectedIDs = {
        false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false
    };

    static GPGauntletsPopup* create(GauntletCallback);

    ~GPGauntletsPopup() override;
};
