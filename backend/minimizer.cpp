#include "minimizer.h"

Minimizer::Minimizer(vector<ll> func, ll n, QObject* parent)
    : QObject(parent), n_(n), m_(1 << n), func_(std::move(func)) {
    for (ll mask = 1; mask < m_; ++mask) {
        table_[mask].resize(m_);
    }
}

void Minimizer::step1() {
    for (auto& [mask, column] : table_) {
        for (ll i = 0; i < m_; ++i) {
            if (!func_[i]) {
                column[i] = 1;
            }
        }
    }
}

void Minimizer::step2() {
    for (auto& [mask, column] : table_) {
        set<ll> localRemoved;
        for (ll i = 0; i < m_; ++i) {
            const ll localNum = mask & i;
            if (column[i]) {
                localRemoved.insert(localNum);
            }
        }
        for (ll i = 0; i < m_; ++i) {
            const ll localNum = mask & i;
            if (!column[i] && localRemoved.find(localNum) != localRemoved.end()) {
                column[i] = 2;
            }
        }
    }
}

void Minimizer::step3() {
    vector<ll> used(m_, INF);
    for (auto& [mask, column] : table_) {
        for (ll i = 0; i < m_; ++i) {
            if (column[i]) {
                continue;
            }
            if (__builtin_popcount(static_cast<unsigned>(mask)) <= used[i]) {
                used[i] = __builtin_popcount(static_cast<unsigned>(mask));
            } else {
                column[i] = 3;
            }
        }
    }
}

void Minimizer::computeAnswer() {
    vector<vector<ll>> cells(m_);
    for (auto& [mask, column] : table_) {
        for (ll i = 0; i < m_; ++i) {
            if (!column[i]) {
                cells[i].push_back(mask);
            }
        }
    }
    bestDNF_ = MinDNF(INF);
    MinDNF curr;
    recursionAnswer(0, curr, cells, bestDNF_);
    for (auto& [mask, column] : table_) {
        for (ll i = 0; i < m_; ++i) {
            if (column[i]) {
                continue;
            }
            const Disjunct cand(mask, i);
            ll inBest = 0;
            for (const auto& d : bestDNF_.disjuncts()) {
                if (d == cand) {
                    inBest = 1;
                    break;
                }
            }
            if (!inBest) {
                column[i] = 4;
            }
        }
    }
    for (ll i = 0; i < m_; ++i) {
        ll seenZero = 0;
        for (auto& [mask, column] : table_) {
            if (!column[i]) {
                if (seenZero) {
                    column[i] = 4;
                } else {
                    seenZero = 1;
                }
            }
        }
    }
}

void Minimizer::recursionAnswer(ll i, MinDNF& curr, vector<vector<ll>>& cells, MinDNF& best) {
    if (i == m_) {
        if (curr.weight() < best.weight()) {
            best = curr;
        }
        return;
    }

    if (cells[i].empty()) {
        recursionAnswer(i + 1, curr, cells, best);
        return;
    }

    for (const ll mask : cells[i]) {
        const Disjunct d(mask, i);
        const ll added = curr.add(d);
        recursionAnswer(i + 1, curr, cells, best);
        if (added) {
            curr.popLast();
        }
    }
}

void Minimizer::makeStep() {
    if (stage_ == 0) {
        step1();
    }
    if (stage_ == 1) {
        step2();
    }
    if (stage_ == 2) {
        step3();
    }
    if (stage_ == 3) {
        computeAnswer();
    }
    if (stage_ < 4) {
        ++stage_;
    }
}
