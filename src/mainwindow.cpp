#include "mainwindow.h"

#include <QFont>
#include <QGuiApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QSignalBlocker>
#include <QShortcut>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "centralminimizerwidget.h"
#include "mindnf_formatter.h"

namespace {

class AnswerCopyLabel : public QLabel {
public:
    explicit AnswerCopyLabel(QWidget* parent = nullptr) : QLabel(parent) {
        setAlignment(Qt::AlignCenter);
        setWordWrap(true);
        setFont(QFont(QStringLiteral("Arial"), 14));
        setCursor(Qt::PointingHandCursor);
        setToolTip(QStringLiteral("Копировать по клику"));
    }

    void showIdle() {
        clip_.clear();
        setTextFormat(Qt::PlainText);
        setText(QStringLiteral("\u2014"));
    }

    void showAnswer(const QString& html, const QString& clip) {
        clip_ = clip;
        setTextFormat(Qt::RichText);
        setText(html);
    }

protected:
    void mousePressEvent(QMouseEvent* e) override {
        if (e->button() == Qt::LeftButton && !clip_.isEmpty()) {
            QGuiApplication::clipboard()->setText(clip_);
        }
        QLabel::mousePressEvent(e);
    }

private:
    QString clip_;
};

}  // namespace

MainWindow::MainWindow() {
    setWindowTitle(QStringLiteral("DNF minimizer"));
    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(10, 8, 10, 8);
    root->setSpacing(8);

    auto* top = new QWidget(central);
    top->setFixedHeight(52);
    top->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    auto* topLay = new QHBoxLayout(top);
    topLay->setContentsMargins(0, 0, 0, 0);
    auto* nLab = new QLabel(QStringLiteral("N:"), top);
    nLab->setFont(QFont(QStringLiteral("Arial"), 11));
    nSpin_ = new QSpinBox(top);
    nSpin_->setRange(2, 7);
    nSpin_->setFont(QFont(QStringLiteral("Arial"), 11));
    nSpin_->setValue(static_cast<int>(session_.n()));
    connect(nSpin_, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::onNChanged);

    stepBtn_ = new QPushButton(QStringLiteral("\u0412\u043F\u0435\u0440\u0451\u0434"), top);
    resetBtn_ = new QPushButton(QStringLiteral("\u0412\u0435\u0440\u043D\u0443\u0442\u044C"), top);
    restartBtn_ = new QPushButton(QStringLiteral("\u041D\u0430\u0447\u0430\u0442\u044C \u0441\u043D\u0430\u0447\u0430\u043B\u0430"), top);
    zoomOutBtn_ = new QPushButton(QStringLiteral("\u2212"), top);
    zoomInBtn_ = new QPushButton(QStringLiteral("+"), top);
    for (auto* b : {stepBtn_, resetBtn_, restartBtn_, zoomOutBtn_, zoomInBtn_}) {
        b->setFont(QFont(QStringLiteral("Arial"), 11));
    }
    connect(stepBtn_, &QPushButton::clicked, this, &MainWindow::onStep);
    connect(resetBtn_, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(restartBtn_, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(zoomInBtn_, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(zoomOutBtn_, &QPushButton::clicked, this, &MainWindow::zoomOut);

    topLay->addWidget(nLab);
    topLay->addWidget(nSpin_);
    topLay->addSpacing(12);
    topLay->addWidget(stepBtn_);
    topLay->addWidget(resetBtn_);
    topLay->addWidget(restartBtn_);
    topLay->addSpacing(12);
    topLay->addWidget(zoomOutBtn_);
    topLay->addWidget(zoomInBtn_);
    topLay->addStretch(1);

    center_ = new CentralMinimizerWidget(&session_, central);
    connect(center_, &CentralMinimizerWidget::modelChanged, this, &MainWindow::refreshAll);

    auto* bottom = new QWidget(central);
    bottom->setFixedHeight(96);
    bottom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    auto* botLay = new QVBoxLayout(bottom);
    botLay->setContentsMargins(4, 4, 4, 4);
    answerLabel_ = new AnswerCopyLabel(bottom);
    static_cast<AnswerCopyLabel*>(answerLabel_)->showIdle();
    botLay->addWidget(answerLabel_);

    root->addWidget(top);
    root->addWidget(center_, 1);
    root->addWidget(bottom);

    auto* scIn = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus), this);
    auto* scIn2 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal), this);
    auto* scOut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus), this);
    connect(scIn, &QShortcut::activated, this, &MainWindow::zoomIn);
    connect(scIn2, &QShortcut::activated, this, &MainWindow::zoomIn);
    connect(scOut, &QShortcut::activated, this, &MainWindow::zoomOut);

    refreshAll();
}

void MainWindow::refreshAll() {
    {
        QSignalBlocker b(nSpin_);
        nSpin_->setValue(static_cast<int>(session_.n()));
    }
    stepBtn_->setEnabled(session_.canStepForward());
    center_->setZoom(zoomValue_);
    center_->refresh();
    if (session_.isConstantFunc()) {
        const ll v = session_.constantDNF();
        static_cast<AnswerCopyLabel*>(answerLabel_)->showAnswer(constantDNFDisplayHtml(v),
                                                               constantDNFClipboardPlain(v));
    } else if (session_.stage() == 4 && session_.minimizer()) {
        const MinDNF& ans = session_.minimizer()->answer();
        const QString clip = minDNFClipboardPlain(ans, session_.n());
        const QString html = minDNFDisplayHtml(ans, session_.n());
        if (ans.mindnf.empty()) {
            static_cast<AnswerCopyLabel*>(answerLabel_)->showIdle();
        } else {
            static_cast<AnswerCopyLabel*>(answerLabel_)->showAnswer(html, clip);
        }
    } else {
        static_cast<AnswerCopyLabel*>(answerLabel_)->showIdle();
    }
}

void MainWindow::onStep() {
    session_.stepForward();
    refreshAll();
}

void MainWindow::onReset() {
    session_.resetMinimization();
    refreshAll();
}

void MainWindow::onNChanged(int v) {
    session_.setN(static_cast<ll>(v));
    refreshAll();
}

void MainWindow::zoomIn() {
    zoomValue_ += 0.1;
    if (zoomValue_ > 2.5) {
        zoomValue_ = 2.5;
    }
    refreshAll();
}

void MainWindow::zoomOut() {
    zoomValue_ -= 0.1;
    if (zoomValue_ < 0.5) {
        zoomValue_ = 0.5;
    }
    refreshAll();
}
