#include "minimizersession.h"

MinimizerSession::MinimizerSession() : n_(2), func_(4, 0) {
    rebuildMinimizer();
}

void MinimizerSession::setN(ll n) {
    if (n < 2) {
        n = 2;
    }
    if (n > 7) {
        n = 7;
    }
    if (n == n_) {
        return;
    }
    const ll newM = 1ll << n;
    if (n > n_) {
        func_.resize(static_cast<size_t>(newM), 0);
    } else {
        func_.resize(static_cast<size_t>(newM));
    }
    n_ = n;
    rebuildMinimizer();
}

void MinimizerSession::rebuildMinimizer() {
    minimizer_ = std::make_unique<Minimizer>(func_, n_, nullptr);
}

void MinimizerSession::resetMinimization() {
    rebuildMinimizer();
}

bool MinimizerSession::setFuncCell(ll index, ll value) {
    if (value != 0 && value != 1) {
        return false;
    }
    if (index < 0 || index >= m()) {
        return false;
    }
    const ll st = minimizer_->returnStage();
    if (st >= 1 && st <= 3) {
        return false;
    }
    if (st == 4) {
        resetMinimization();
    }
    func_[static_cast<size_t>(index)] = value;
    rebuildMinimizer();
    return true;
}

bool MinimizerSession::toggleFuncCell(ll index) {
    if (index < 0 || index >= m()) {
        return false;
    }
    const ll st = minimizer_->returnStage();
    if (st >= 1 && st <= 3) {
        return false;
    }
    if (st == 4) {
        resetMinimization();
    }
    func_[static_cast<size_t>(index)] ^= 1;
    rebuildMinimizer();
    return true;
}

bool MinimizerSession::stepForward() {
    if (!canStepForward()) {
        return false;
    }
    minimizer_->makeStep();
    return true;
}

ll MinimizerSession::stage() const {
    return minimizer_->returnStage();
}

bool MinimizerSession::canStepForward() const {
    return minimizer_ && minimizer_->returnStage() < 4;
}

bool MinimizerSession::canEditFunc() const {
    if (!minimizer_) {
        return false;
    }
    const ll st = minimizer_->returnStage();
    return st == 0 || st == 4;
}
