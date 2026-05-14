#pragma once

#include <QMainWindow>

#include "minimizersession.h"

class CentralMinimizerWidget;
class QSpinBox;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void refreshAll();
    void onStep();
    void onReset();
    void onNChanged(int v);
    void zoomIn();
    void zoomOut();

private:
    MinimizerSession session_;
    CentralMinimizerWidget* center_ = nullptr;
    QLabel* answerLabel_ = nullptr;
    QSpinBox* nSpin_ = nullptr;
    QPushButton* stepBtn_ = nullptr;
    QPushButton* resetBtn_ = nullptr;
    QPushButton* restartBtn_ = nullptr;
    QPushButton* zoomInBtn_ = nullptr;
    QPushButton* zoomOutBtn_ = nullptr;
    qreal zoomValue_ = 1.0;
};
