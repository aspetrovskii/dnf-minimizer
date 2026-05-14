#pragma once

#include <QWidget>

class MinimizerSession;

class FuncColumnWidget : public QWidget {
    Q_OBJECT
public:
    explicit FuncColumnWidget(MinimizerSession* session, QWidget* parent = nullptr);

    void setRowHeight(int h);
    int rowHeight() const { return rowHeight_; }

    void refresh();

    QSize sizeHint() const override;

signals:
    void dataEdited();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    int rowAtY(int y) const;
    QRect rowRect(int row) const;

    MinimizerSession* session_;
    int rowHeight_ = 24;
    int editingRow_ = -1;
};
