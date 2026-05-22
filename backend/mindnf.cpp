#include "mindnf.h"

MinDNF::MinDNF(ll maxWeight) : weight_(maxWeight) {}

ll MinDNF::add(const Disjunct& d) {
    for (const auto& x : disjuncts_) {
        if (x == d) {
            return 0;
        }
    }
    disjuncts_.push_back(d);
    weight_ += d.literalCount();
    return 1;
}

void MinDNF::popLast() {
    weight_ -= disjuncts_.back().literalCount();
    disjuncts_.pop_back();
}
