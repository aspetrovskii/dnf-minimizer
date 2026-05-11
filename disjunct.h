#include "types.h"

struct Disjunct {
    public:
    ll mask, kit;
    explicit Disjunct (ll mask, ll kit) {
        this->mask = mask;
        this->kit = kit;
    }
    Disjunct(const Disjunct&) = default;
    Disjunct(Disjunct&&) noexcept = default;
    Disjunct& operator=(const Disjunct&) = default;
    Disjunct& operator=(Disjunct&&) noexcept = default;
    ~Disjunct() = default;

    friend bool operator==(const Disjunct& d, const Disjunct& x) {
        return d.get() == x.get() && d.mask == x.mask;
    }

    ll get() const { return mask & kit; };
    ll size() const {return __builtin_popcount(mask);} ;
};