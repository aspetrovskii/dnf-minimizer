#include "setspanelwidget.h"

#include <QPainter>
#include <QPaintEvent>

#include "minimizersession.h"

SetsPanelWidget::SetsPanelWidget(MinimizerSession* session, QWidget* parent)
    : QWidget(parent), session_(session) {
    setFont(QFont(QStringLiteral("Arial"), 11));
}

void SetsPanelWidget::setRowHeight(int h) {
    if (h < 8) {
        h = 8;
    }
    rowHeight_ = h;
    updateGeometry();
    update();
}

void SetsPanelWidget::refresh() {
    update();
}

QSize SetsPanelWidget::sizeHint() const {
    if (!session_) {
        return QSize(64, 100);
    }
    const int rows = static_cast<int>(session_->m());
    QFontMetrics fm(font());
    const int w = fm.horizontalAdvance(QString(session_->n(), QChar('0'))) + 28;
    return QSize(std::max(64, w), rows * rowHeight_);
}

void SetsPanelWidget::paintEvent(QPaintEvent*) {
    if (!session_) {
        return;
    }
    QPainter p(this);
    p.fillRect(rect(), palette().base());
    p.setPen(palette().color(QPalette::Text));
    const int rows = static_cast<int>(session_->m());
    const ll n = session_->n();
    for (int i = 0; i < rows; ++i) {
        const QRect rr(0, i * rowHeight_, width(), rowHeight_);
        p.drawRect(rr);
        QString bits;
        bits.reserve(static_cast<int>(n));
        for (ll k = 0; k < n; ++k) {
            bits += QChar(static_cast<char16_t>('0' + ((static_cast<ll>(i) >> k) & 1)));
        }
        p.drawText(rr, Qt::AlignCenter, bits);
    }
}
