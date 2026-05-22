#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "popcountcomparator.h"
#include "disjunct.h"
#include "mindnf.h"

class Minimizer : public QObject {
    Q_OBJECT
public:
    explicit Minimizer(vector<ll> func, ll n, QObject* parent = nullptr);
    ~Minimizer() override = default;

    void makeStep();
    ll stage() const { return stage_; }
    const map<ll, vector<ll>, PopcountComparator>& table() const { return table_; }
    const MinDNF& answer() const { return bestDNF_; }

private:
    map<ll, vector<ll>, PopcountComparator> table_;
    ll stage_ = 0;
    ll n_;
    ll m_;
    vector<ll> func_;
    MinDNF bestDNF_;

    void step1();
    void step2();
    void step3();
    void computeAnswer();
    void recursionAnswer(ll i, MinDNF& curr, vector<vector<ll>>& cells, MinDNF& best);
};

#endif
