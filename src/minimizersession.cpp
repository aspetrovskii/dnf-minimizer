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

void MinimizerSession::updateConstantDNF() {
    ll sum = 0;
    for (ll v : func_) {
        sum += v;
    }
    if (sum == 0) {
        constantDNF_ = 0;
    } else if (sum == m()) {
        constantDNF_ = 1;
    } else {
        constantDNF_ = -1;
    }
}

void MinimizerSession::rebuildMinimizer() {
    minimizer_ = std::make_unique<Minimizer>(func_, n_, nullptr);
    updateConstantDNF();
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
    const ll st = stage();
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
    const ll st = stage();
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
    if (isConstantFunc()) {
        return 4;
    }
    return minimizer_->stage();
}

bool MinimizerSession::canStepForward() const {
    if (isConstantFunc()) {
        return false;
    }
    return minimizer_ && minimizer_->stage() < 4;
}

bool MinimizerSession::canEditFunc() const {
    if (!minimizer_) {
        return false;
    }
    if (isConstantFunc()) {
        return true;
    }
    const ll st = minimizer_->stage();
    return st == 0 || st == 4;
}
