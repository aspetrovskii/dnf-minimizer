#include "disjunct.h"

Disjunct::Disjunct(ll mask, ll kit) : mask_(mask), kit_(kit) {}

ll Disjunct::covered() const {
    return mask_ & kit_;
}

ll Disjunct::literalCount() const {
    return __builtin_popcount(static_cast<unsigned>(kit_));
}

bool Disjunct::operator==(const Disjunct& other) const {
    return covered() == other.covered() && mask_ == other.mask_;
}

bool Disjunct::operator<(const Disjunct& other) const {
    if (mask_ != other.mask_) {
        return mask_ < other.mask_;
    }
    return covered() < other.covered();
}
