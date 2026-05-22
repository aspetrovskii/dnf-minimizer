#include "mindnf.h"

MinDNF::MinDNF(ll maxWeight) : weight_(maxWeight) {}

ll MinDNF::add(const Disjunct& d) {
    const auto [it, inserted] = disjuncts_.emplace(d, 1);
    if (!inserted) {
        return 0;
    }
    weight_ += d.literalCount();
    return 1;
}

void MinDNF::remove(const Disjunct& d) {
    const auto it = disjuncts_.find(d);
    if (it == disjuncts_.end()) {
        return;
    }
    weight_ -= it->first.literalCount();
    disjuncts_.erase(it);
}
