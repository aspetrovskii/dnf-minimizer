#include "centralminimizerwidget.h"

#include <QColor>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QSignalBlocker>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>

#include "funccolumnwidget.h"
#include "minimizersession.h"
#include "setspanelwidget.h"

namespace {

QString headerText(ll mask, ll n) {
    QString s;
    for (ll k = 0; k < n; ++k) {
        if (((mask >> k) & 1) == 0) {
            continue;
        }
        s += QChar(static_cast<char16_t>('A' + k));
    }
    return s;
}

QString cellText(ll mask, ll row, ll n) {
    QString s;
    for (ll k = 0; k < n; ++k) {
        if (((mask >> k) & 1) == 0) {
            continue;
        }
        s += QChar(static_cast<char16_t>('0' + ((row >> k) & 1)));
    }
    return s;
}

QColor fillForStage(ll v) {
    switch (static_cast<int>(v)) {
        case 1:
            return QColor(255, 255, 80, 105);
        case 2:
            return QColor(255, 215, 70, 115);
        case 3:
            return QColor(255, 165, 50, 125);
        case 4:
            return QColor(255, 75, 65, 135);
        default:
            return QColor(255, 255, 255);
    }
}

void recomputeColWidths(MinimizerTableModel& g) {
    QFontMetrics fm(g.font);
    g.colWidths.assign(g.masks.size(), 0);
    for (size_t c = 0; c < g.masks.size(); ++c) {
        const ll mask = g.masks[c];
        int w = fm.horizontalAdvance(headerText(mask, g.n)) + 12;
        for (ll i = 0; i < g.m; ++i) {
            w = std::max(w, fm.horizontalAdvance(cellText(mask, i, g.n)) + 12);
        }
        g.colWidths[c] = w;
    }
}

int totalTableWidth(const MinimizerTableModel& g) {
    int s = 0;
    for (int w : g.colWidths) {
        s += w;
    }
    return s;
}

class HeaderPaintWidget : public QWidget {
public:
    explicit HeaderPaintWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    void setModel(const MinimizerTableModel* m) {
        model_ = m;
        updateGeometry();
        update();
    }

    QSize sizeHint() const override {
        if (!model_) {
            return QSize(200, 24);
        }
        return QSize(std::max(1, totalTableWidth(*model_)), std::max(1, model_->rowH));
    }

protected:
    void paintEvent(QPaintEvent*) override {
        if (!model_) {
            return;
        }
        QPainter p(this);
        p.fillRect(rect(), palette().base());
        p.setFont(model_->font);
        p.setPen(palette().color(QPalette::Text));
        int x = 0;
        for (size_t c = 0; c < model_->masks.size(); ++c) {
            const int cw = model_->colWidths[c];
            const QRect rr(x, 0, cw, model_->rowH);
            p.drawRect(rr);
            p.drawText(rr, Qt::AlignCenter, headerText(model_->masks[c], model_->n));
            x += cw;
        }
    }

private:
    const MinimizerTableModel* model_ = nullptr;
};

class BodyPaintWidget : public QWidget {
public:
    explicit BodyPaintWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    void setModel(const MinimizerTableModel* m) {
        model_ = m;
        updateGeometry();
        update();
    }

    QSize sizeHint() const override {
        if (!model_) {
            return QSize(200, 200);
        }
        const int w = std::max(1, totalTableWidth(*model_));
        const int h = std::max(1, static_cast<int>(model_->m) * model_->rowH);
        return QSize(w, h);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        if (!model_) {
            return;
        }
        QPainter p(this);
        p.setFont(model_->font);
        const int rh = model_->rowH;
        for (ll i = 0; i < model_->m; ++i) {
            int x = 0;
            for (size_t c = 0; c < model_->masks.size(); ++c) {
                const int cw = model_->colWidths[c];
                const QRect rr(x, static_cast<int>(i) * rh, cw, rh);
                const ll st = model_->values[c][static_cast<size_t>(i)];
                if (st == 0) {
                    p.fillRect(rr, QColor(255, 255, 255));
                } else {
                    p.fillRect(rr, fillForStage(st));
                }
                p.setPen(palette().color(QPalette::Text));
                p.drawRect(rr);
                p.drawText(rr, Qt::AlignCenter, cellText(model_->masks[c], i, model_->n));
                x += cw;
            }
        }
    }

private:
    const MinimizerTableModel* model_ = nullptr;
};

class CornerBodyWidget : public QWidget {
public:
    explicit CornerBodyWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    void setMetrics(int w, int rowH, ll m) {
        cornerW_ = w;
        rowH_ = rowH;
        m_ = m;
        updateGeometry();
        update();
    }

    QSize sizeHint() const override {
        return QSize(std::max(1, cornerW_), std::max(1, static_cast<int>(m_) * rowH_));
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.fillRect(rect(), palette().base());
        p.setPen(palette().color(QPalette::Text));
        for (ll i = 0; i < m_; ++i) {
            const QRect rr(0, static_cast<int>(i) * rowH_, width(), rowH_);
            p.drawRect(rr);
        }
    }

private:
    int cornerW_ = 36;
    int rowH_ = 22;
    ll m_ = 0;
};

}  // namespace

CentralMinimizerWidget::CentralMinimizerWidget(MinimizerSession* session, QWidget* parent)
    : QWidget(parent), session_(session) {
    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    leftScroll_ = new QScrollArea(this);
    leftScroll_->setFrameShape(QFrame::NoFrame);
    leftScroll_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    leftScroll_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    leftScroll_->setWidgetResizable(false);

    auto* leftInner = new QWidget(leftScroll_);
    auto* leftLay = new QHBoxLayout(leftInner);
    leftLay->setContentsMargins(0, 0, 0, 0);
    leftLay->setSpacing(0);
    sets_ = new SetsPanelWidget(session_, leftInner);
    func_ = new FuncColumnWidget(session_, leftInner);
    leftLay->addWidget(sets_);
    leftLay->addWidget(func_);
    leftScroll_->setWidget(leftInner);
    connect(func_, &FuncColumnWidget::dataEdited, this, [this] { emit modelChanged(); });

    auto* right = new QWidget(this);
    auto* grid = new QGridLayout(right);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);

    cornerTop_ = new QWidget(right);
    toggleBtn_ = new QPushButton(QStringLiteral("\u25B6"), cornerTop_);
    toggleBtn_->setToolTip(QStringLiteral("Панель наборов"));
    auto* ctLay = new QHBoxLayout(cornerTop_);
    ctLay->setContentsMargins(0, 0, 0, 0);
    ctLay->addWidget(toggleBtn_, 0, Qt::AlignCenter);
    connect(toggleBtn_, &QPushButton::clicked, this, &CentralMinimizerWidget::onToggleSets);

    headScroll_ = new QScrollArea(right);
    headScroll_->setFrameShape(QFrame::NoFrame);
    headScroll_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    headScroll_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    headScroll_->setWidgetResizable(false);
    headerHost_ = new HeaderPaintWidget(headScroll_);
    headScroll_->setWidget(headerHost_);

    cornerBodyScroll_ = new QScrollArea(right);
    cornerBodyScroll_->setFrameShape(QFrame::NoFrame);
    cornerBodyScroll_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cornerBodyScroll_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cornerBodyScroll_->setWidgetResizable(false);
    cornerBody_ = new CornerBodyWidget(cornerBodyScroll_);
    cornerBodyScroll_->setWidget(cornerBody_);

    bodyScroll_ = new QScrollArea(right);
    bodyScroll_->setFrameShape(QFrame::NoFrame);
    bodyScroll_->setWidgetResizable(false);
    bodyGrid_ = new BodyPaintWidget(bodyScroll_);
    bodyScroll_->setWidget(bodyGrid_);

    grid->addWidget(cornerTop_, 0, 0);
    grid->addWidget(headScroll_, 0, 1);
    grid->addWidget(cornerBodyScroll_, 1, 0);
    grid->addWidget(bodyScroll_, 1, 1);
    grid->setRowStretch(1, 1);
    grid->setColumnStretch(1, 1);

    root->addWidget(leftScroll_);
    root->addWidget(right, 1);

    connect(bodyScroll_->verticalScrollBar(), &QScrollBar::valueChanged, this,
            &CentralMinimizerWidget::syncLeftVFromBody);
    connect(leftScroll_->verticalScrollBar(), &QScrollBar::valueChanged, this,
            &CentralMinimizerWidget::syncBodyVFromLeft);
    connect(cornerBodyScroll_->verticalScrollBar(), &QScrollBar::valueChanged, this,
            &CentralMinimizerWidget::syncCornerVFromBody);
    connect(bodyScroll_->horizontalScrollBar(), &QScrollBar::valueChanged, this,
            &CentralMinimizerWidget::syncHeadHFromBody);
    connect(headScroll_->horizontalScrollBar(), &QScrollBar::valueChanged, this,
            &CentralMinimizerWidget::syncBodyHFromHead);

    bodyScroll_->viewport()->installEventFilter(this);

    sets_->hide();
    refresh();
}

void CentralMinimizerWidget::rebuildModel() {
    model_.n = session_->n();
    model_.m = session_->m();
    model_.rowH = std::max(12, static_cast<int>(std::lround(22.0 * zoom_)));
    model_.cornerW = 36;
    model_.font = QFont(QStringLiteral("Arial"), std::max(8, static_cast<int>(std::lround(11.0 * zoom_))));
    model_.masks.clear();
    model_.values.clear();
    if (!session_->minimizer()) {
        recomputeColWidths(model_);
        return;
    }
    const auto tab = session_->minimizer()->returnTable();
    for (const auto& e : tab) {
        model_.masks.push_back(e.first);
        model_.values.push_back(e.second);
    }
    recomputeColWidths(model_);
}

void CentralMinimizerWidget::applySizes() {
    if (cornerTop_) {
        cornerTop_->setFixedHeight(model_.rowH);
        cornerTop_->setFixedWidth(model_.cornerW);
    }
    toggleBtn_->setMaximumWidth(model_.cornerW);

    static_cast<CornerBodyWidget*>(cornerBody_)->setMetrics(model_.cornerW, model_.rowH, model_.m);
    static_cast<HeaderPaintWidget*>(headerHost_)->setModel(&model_);
    static_cast<BodyPaintWidget*>(bodyGrid_)->setModel(&model_);

    const int tableW = std::max(1, totalTableWidth(model_));
    const int bodyH = static_cast<int>(model_.m) * model_.rowH;
    headerHost_->resize(tableW, model_.rowH);
    bodyGrid_->resize(tableW, bodyH);
    cornerBody_->resize(model_.cornerW, bodyH);

    const int leftW = (setsVisible_ ? sets_->sizeHint().width() : 0) + func_->sizeHint().width();
    leftScroll_->widget()->setFixedSize(std::max(1, leftW), std::max(1, bodyH));
}

void CentralMinimizerWidget::setZoom(qreal z) {
    if (z < 0.5) {
        z = 0.5;
    }
    if (z > 2.5) {
        z = 2.5;
    }
    zoom_ = z;
}

void CentralMinimizerWidget::refresh() {
    rebuildModel();
    sets_->setRowHeight(model_.rowH);
    func_->setRowHeight(model_.rowH);
    applySizes();
    sets_->refresh();
    func_->refresh();
}

void CentralMinimizerWidget::setSetsPanelVisible(bool visible) {
    setsVisible_ = visible;
    sets_->setVisible(visible);
    toggleBtn_->setText(visible ? QStringLiteral("\u25C0") : QStringLiteral("\u25B6"));
    refresh();
}

void CentralMinimizerWidget::onToggleSets() {
    setSetsPanelVisible(!setsVisible_);
}

void CentralMinimizerWidget::syncLeftVFromBody(int v) {
    if (syncingV_) {
        return;
    }
    syncingV_ = true;
    QSignalBlocker b1(leftScroll_->verticalScrollBar());
    QSignalBlocker b2(cornerBodyScroll_->verticalScrollBar());
    leftScroll_->verticalScrollBar()->setValue(v);
    cornerBodyScroll_->verticalScrollBar()->setValue(v);
    syncingV_ = false;
}

void CentralMinimizerWidget::syncBodyVFromLeft(int v) {
    if (syncingV_) {
        return;
    }
    syncingV_ = true;
    QSignalBlocker b1(bodyScroll_->verticalScrollBar());
    QSignalBlocker b2(cornerBodyScroll_->verticalScrollBar());
    bodyScroll_->verticalScrollBar()->setValue(v);
    cornerBodyScroll_->verticalScrollBar()->setValue(v);
    syncingV_ = false;
}

void CentralMinimizerWidget::syncCornerVFromBody(int v) {
    if (syncingV_) {
        return;
    }
    syncingV_ = true;
    QSignalBlocker b1(bodyScroll_->verticalScrollBar());
    QSignalBlocker b2(leftScroll_->verticalScrollBar());
    bodyScroll_->verticalScrollBar()->setValue(v);
    leftScroll_->verticalScrollBar()->setValue(v);
    syncingV_ = false;
}

void CentralMinimizerWidget::syncHeadHFromBody(int v) {
    if (syncingH_) {
        return;
    }
    syncingH_ = true;
    QSignalBlocker b(headScroll_->horizontalScrollBar());
    headScroll_->horizontalScrollBar()->setValue(v);
    syncingH_ = false;
}

void CentralMinimizerWidget::syncBodyHFromHead(int v) {
    if (syncingH_) {
        return;
    }
    syncingH_ = true;
    QSignalBlocker b(bodyScroll_->horizontalScrollBar());
    bodyScroll_->horizontalScrollBar()->setValue(v);
    syncingH_ = false;
}

bool CentralMinimizerWidget::eventFilter(QObject* watched, QEvent* event) {
    if (watched == bodyScroll_->viewport() && event->type() == QEvent::Wheel) {
        auto* we = static_cast<QWheelEvent*>(event);
        if (we->modifiers() & Qt::ShiftModifier) {
            const int dx = we->angleDelta().y();
            QScrollBar* h = bodyScroll_->horizontalScrollBar();
            h->setValue(h->value() - dx);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}
