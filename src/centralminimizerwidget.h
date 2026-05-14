#pragma once

#include <QFont>
#include <QWidget>
#include <vector>

#include "includes.h"

struct MinimizerTableModel {
    ll n = 0;
    ll m = 0;
    int rowH = 22;
    QFont font;
    std::vector<ll> masks;
    std::vector<int> colWidths;
    std::vector<std::vector<ll>> values;
};

class MinimizerSession;
class QScrollArea;
class QPushButton;
class SetsPanelWidget;
class FuncColumnWidget;

class CentralMinimizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit CentralMinimizerWidget(MinimizerSession* session, QWidget* parent = nullptr);

    void setZoom(qreal z);
    qreal zoom() const { return zoom_; }

    void refresh();

    bool setsPanelVisible() const { return setsVisible_; }

public slots:
    void setSetsPanelVisible(bool visible);

signals:
    void modelChanged();

private slots:
    void onToggleSets();
    void syncLeftVFromBody(int v);
    void syncBodyVFromLeft(int v);
    void syncHeadHFromBody(int v);
    void syncBodyHFromHead(int v);

private:
    void rebuildModel();
    void applySizes();
    bool eventFilter(QObject* watched, QEvent* event) override;

    MinimizerSession* session_;
    qreal zoom_ = 1.0;
    MinimizerTableModel model_;

    SetsPanelWidget* sets_ = nullptr;
    FuncColumnWidget* func_ = nullptr;
    QScrollArea* leftScroll_ = nullptr;
    QScrollArea* headScroll_ = nullptr;
    QScrollArea* bodyScroll_ = nullptr;
    QPushButton* toggleBtn_ = nullptr;
    QWidget* cornerTop_ = nullptr;
    QWidget* headerHost_ = nullptr;
    QWidget* bodyGrid_ = nullptr;
    QWidget* leftInner_ = nullptr;

    bool setsVisible_ = false;
    bool syncingV_ = false;
    bool syncingH_ = false;
};
