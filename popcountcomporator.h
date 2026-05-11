struct PopcountComparator {
    bool operator()(int lhs, int rhs) const {
        int lhsCount = __builtin_popcount(static_cast<unsigned int>(lhs));
        int rhsCount = __builtin_popcount(static_cast<unsigned int>(rhs));
        if (lhsCount != rhsCount) return lhsCount < rhsCount;
        return lhs < rhs;
    }
};
