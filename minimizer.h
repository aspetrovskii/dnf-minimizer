#ifndef MINIMIZER_H
#define MINIMIZER_H

#include <QObject>
#include "popcountcomporator.h"
#include "disjunct.h"
#include "mindnf.h"

const ll INF = 1e18;

class Minimizer : public QObject {
    Q_OBJECT
public:
    explicit Minimizer(vector<ll> s, ll n, QObject *parent = nullptr);
    ~Minimizer() = default;

    void makeStep();
    vector<ll> getAnswer();
    map<ll, vector<ll>, PopcountComparator> returnTable() { return table; };

signals:


private:
    map<ll, vector<ll>, PopcountComparator> table;
    ll stage = 0;
    ll N, M;
    vector<ll> func, answer;

    void step1(), step2(), step3(), computeAnswer();
    void recursionAnswer(ll i, MinDNF& currMinDNF, vector<vector<ll>>& cells, vector<MinDNF>& minDNFs);
};

#endif
