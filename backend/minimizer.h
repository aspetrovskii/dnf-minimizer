#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "popcountcomporator.h"
#include "disjunct.h"
#include "mindnf.h"

class Minimizer : public QObject {
    Q_OBJECT
public:
    explicit Minimizer(vector<ll> s, ll n, QObject *parent = nullptr);
    ~Minimizer() = default;

    void makeStep();
    vector<ll> getAnswer();
    map<ll, vector<ll>, PopcountComparator> returnTable() { return table; };
    ll returnStage() {return stage;};
    MinDNF returnAns() {return bestDNF;};

signals:


private:
    map<ll, vector<ll>, PopcountComparator> table;
    ll stage = 0;
    ll N, M;
    vector<ll> func, answer;
    MinDNF bestDNF;

    void step1(), step2(), step3(), computeAnswer();
    void recursionAnswer(ll i, MinDNF& currMinDNF, vector<vector<ll>>& cells, MinDNF& minDNFs);
};

#endif
