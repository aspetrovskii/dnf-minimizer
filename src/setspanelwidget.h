#pragma once

#include <QWidget>

class MinimizerSession;

class SetsPanelWidget : public QWidget {
public:
    explicit SetsPanelWidget(MinimizerSession* session, QWidget* parent = nullptr);

    void setRowHeight(int h);
    int rowHeight() const { return rowHeight_; }

    void refresh();

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    MinimizerSession* session_;
    int rowHeight_ = 24;
};
