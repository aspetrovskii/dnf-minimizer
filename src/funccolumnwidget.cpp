#include "funccolumnwidget.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>

#include "minimizersession.h"

FuncColumnWidget::FuncColumnWidget(MinimizerSession* session, QWidget* parent)
    : QWidget(parent), session_(session) {
    setFocusPolicy(Qt::StrongFocus);
    setFont(QFont(QStringLiteral("Arial"), 11));
}

void FuncColumnWidget::setRowHeight(int h) {
    if (h < 8) {
        h = 8;
    }
    rowHeight_ = h;
    updateGeometry();
    update();
}

void FuncColumnWidget::refresh() {
    if (editingRow_ >= session_->m()) {
        editingRow_ = static_cast<int>(session_->m()) - 1;
    }
    if (editingRow_ < -1) {
        editingRow_ = -1;
    }
    update();
}

QSize FuncColumnWidget::sizeHint() const {
    if (!session_) {
        return QSize(56, 100);
    }
    const int rows = static_cast<int>(session_->m());
    QFontMetrics fm(font());
    const int textW = std::max(fm.horizontalAdvance(QStringLiteral("0")),
                               fm.horizontalAdvance(QStringLiteral("1")));
    const int w = std::max(56, textW + 32);
    return QSize(w, rows * rowHeight_);
}

int FuncColumnWidget::rowAtY(int y) const {
    if (rowHeight_ <= 0) {
        return -1;
    }
    const int r = y / rowHeight_;
    if (r < 0 || r >= static_cast<int>(session_->m())) {
        return -1;
    }
    return r;
}

QRect FuncColumnWidget::rowRect(int row) const {
    return QRect(0, row * rowHeight_, width(), rowHeight_);
}

void FuncColumnWidget::paintEvent(QPaintEvent*) {
    if (!session_) {
        return;
    }
    QPainter p(this);
    p.fillRect(rect(), palette().base());
    p.setPen(palette().color(QPalette::Text));
    const int rows = static_cast<int>(session_->m());
    const auto& f = session_->func();
    for (int i = 0; i < rows; ++i) {
        const QRect rr = rowRect(i);
        if (session_->canEditFunc() && i == editingRow_) {
            p.fillRect(rr, QColor(255, 255, 0, 90));
        }
        p.drawRect(rr);
        const ll v = f[static_cast<size_t>(i)];
        const QString s = QString::number(static_cast<int>(v));
        p.drawText(rr, Qt::AlignCenter, s);
    }
    if (!session_->canEditFunc()) {
        p.fillRect(rect(), QColor(0, 0, 0, 35));
    }
}

void FuncColumnWidget::mousePressEvent(QMouseEvent* event) {
    if (!session_ || event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
    const int r = rowAtY(static_cast<int>(event->pos().y()));
    if (r < 0) {
        editingRow_ = -1;
        update();
        QWidget::mousePressEvent(event);
        return;
    }
    if (!session_->canEditFunc()) {
        QWidget::mousePressEvent(event);
        return;
    }
    editingRow_ = r;
    setFocus();
    update();
}

void FuncColumnWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (!session_ || event->button() != Qt::LeftButton) {
        QWidget::mouseDoubleClickEvent(event);
        return;
    }
    const int r = rowAtY(static_cast<int>(event->pos().y()));
    if (r < 0) {
        QWidget::mouseDoubleClickEvent(event);
        return;
    }
    if (!session_->canEditFunc()) {
        QWidget::mouseDoubleClickEvent(event);
        return;
    }
    session_->toggleFuncCell(r);
    editingRow_ = -1;
    update();
    emit dataEdited();
    QWidget::mouseDoubleClickEvent(event);
}

void FuncColumnWidget::keyPressEvent(QKeyEvent* event) {
    if (!session_ || editingRow_ < 0 || !session_->canEditFunc()) {
        QWidget::keyPressEvent(event);
        return;
    }
    const int key = event->key();
    if (key == Qt::Key_0 || key == Qt::Key_1) {
        const ll val = (key == Qt::Key_1) ? 1 : 0;
        if (session_->setFuncCell(editingRow_, val)) {
            if (editingRow_ + 1 < static_cast<int>(session_->m())) {
                editingRow_++;
            } else {
                editingRow_ = static_cast<int>(session_->m()) - 1;
            }
            update();
            emit dataEdited();
        }
        return;
    }
    if (key == Qt::Key_Escape) {
        editingRow_ = -1;
        update();
        return;
    }
    QWidget::keyPressEvent(event);
}
