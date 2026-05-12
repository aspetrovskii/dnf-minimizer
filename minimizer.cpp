#include "minimizer.h"

Minimizer::Minimizer(vector<ll> s, ll n, QObject *parent) {
    N = n; M = (1<<n);
    for(ll mask=1; mask<M; ++mask){
        table[mask].resize(M);
    }

    func = s;
}

void Minimizer::step1() {
    for(auto& [mask, column] : table){
        for(ll i=0; i<M; ++i){
            if(!func[i]){
                column[i]=1;
            }
        }
    }
}

void Minimizer::step2() {
    for(auto& [mask, column] : table){
        set<ll> localRemoved;
        for(ll i=0; i<M; ++i){
            ll localNum = mask & i;
            if(column[i]){
                localRemoved.insert(localNum);
            }
        }
        for(ll i=0; i<M; ++i){
            ll localNum = mask & i;
            if(!column[i] && localRemoved.find(localNum) != localRemoved.end()){
                column[i]=2;
            }
        }
    }
}

void Minimizer::step3() {
    vector<ll> used(M, INF);
    for(auto& [mask, column] : table){
        for(ll i=0; i<M; ++i){
            if(column[i]) continue;
            if(__builtin_popcount(mask) <= used[i]){
                used[i] = __builtin_popcount(mask);
            }else{
                column[i] = 3;
            }
        }
    }

    computeAnswer();
}

void Minimizer::computeAnswer() {
    vector<vector<ll>> cells(M);
    for(auto& [mask, column] : table){
        for(ll i=0; i<M; ++i){
            if(!column[i]) cells[i].push_back(mask);
        }
    }
    vector<MinDNF> minDNFs; //size, disjuncts
    MinDNF currMinDNF;
    recursionAnswer(0, currMinDNF, cells, minDNFs);
}

void Minimizer::recursionAnswer(ll i, MinDNF& currMinDNF, vector<vector<ll>>& cells, vector<MinDNF>& minDNFs){
    if(i==M){
        MinDNF finalMinDNF = currMinDNF;
        minDNFs.push_back(finalMinDNF);
        return;
    }

    for(auto mask : cells[i]){
        Disjunct D(mask, i);
        ll added = currMinDNF.add(D);
        recursionAnswer(i+1, currMinDNF, cells, minDNFs);
        if(added) currMinDNF.del_back();
    }
}

void Minimizer::makeStep(){
    if(stage==0) step1();
    if(stage==1) step2();
    if(stage==2) step3();
    if(stage<3) ++stage;
}