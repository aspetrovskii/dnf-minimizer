#pragma once

#include "includes.h"

class Disjunct {
public:
    Disjunct(ll mask, ll kit);

    Disjunct(const Disjunct&) = default;
    Disjunct(Disjunct&&) noexcept = default;
    Disjunct& operator=(const Disjunct&) = default;
    Disjunct& operator=(Disjunct&&) noexcept = default;
    ~Disjunct() = default;

    ll mask() const { return mask_; }
    ll kit() const { return kit_; }
    ll covered() const;
    ll literalCount() const;

    bool operator==(const Disjunct& other) const;
    bool operator<(const Disjunct& other) const;

private:
    ll mask_;
    ll kit_;
};
