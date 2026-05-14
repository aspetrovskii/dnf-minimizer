#pragma once

#include "includes.h"

struct PopcountComparator {
    bool operator()(ll lhs, ll rhs) const {
        int lhsCount = __builtin_popcount(static_cast<ull>(lhs));
        int rhsCount = __builtin_popcount(static_cast<ull>(rhs));
        if (lhsCount != rhsCount) return lhsCount < rhsCount;
        return lhs < rhs;
    }
};
