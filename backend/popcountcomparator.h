#pragma once

#include "includes.h"

class PopcountComparator {
public:
    bool operator()(ll lhs, ll rhs) const {
        const int lhsCount = __builtin_popcount(static_cast<ull>(lhs));
        const int rhsCount = __builtin_popcount(static_cast<ull>(rhs));
        if (lhsCount != rhsCount) {
            return lhsCount < rhsCount;
        }
        return lhs < rhs;
    }
};
