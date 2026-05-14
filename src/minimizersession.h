#pragma once

#include <memory>
#include <vector>

#include "minimizer.h"

class MinimizerSession {
public:
    MinimizerSession();

    ll n() const { return n_; }
    void setN(ll n);

    ll m() const { return 1ll << n_; }

    const std::vector<ll>& func() const { return func_; }

    bool setFuncCell(ll index, ll value);
    bool toggleFuncCell(ll index);

    void resetMinimization();

    bool stepForward();
    ll stage() const;

    bool canStepForward() const;
    bool canEditFunc() const;

    const Minimizer* minimizer() const { return minimizer_.get(); }
    Minimizer* minimizer() { return minimizer_.get(); }

private:
    void rebuildMinimizer();

    ll n_;
    std::vector<ll> func_;
    std::unique_ptr<Minimizer> minimizer_;
};
