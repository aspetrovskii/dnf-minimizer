#pragma once

#include "disjunct.h"

struct MinDNF {
    public:
    vector<Disjunct> mindnf;
    ll size=0;

    MinDNF() = default;
    MinDNF(const MinDNF&) = default;
    MinDNF(MinDNF&&) noexcept = default;
    MinDNF& operator=(const MinDNF&) = default;
    MinDNF& operator=(MinDNF&&) noexcept = default;
    ~MinDNF() = default;

    MinDNF(ll x){
        this->size = x;
    }

    ll add(Disjunct x){
        for(auto d : mindnf){
            if(d == x) return 0;
        }
        mindnf.push_back(x);
        size+=x.size();
        return 1;
    }

    void del_back(){
        mindnf.pop_back();
    }
};