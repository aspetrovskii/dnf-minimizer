#pragma once

#include "disjunct.h"

class MinDNF {
public:
    MinDNF() = default;
    explicit MinDNF(ll maxWeight);

    MinDNF(const MinDNF&) = default;
    MinDNF(MinDNF&&) noexcept = default;
    MinDNF& operator=(const MinDNF&) = default;
    MinDNF& operator=(MinDNF&&) noexcept = default;
    ~MinDNF() = default;

    bool empty() const { return disjuncts_.empty(); }
    ll weight() const { return weight_; }
    const vector<Disjunct>& disjuncts() const { return disjuncts_; }

    ll add(const Disjunct& d);
    void popLast();

private:
    vector<Disjunct> disjuncts_;
    ll weight_ = 0;
};
