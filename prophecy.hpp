#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;
using uint = unsigned;

class prophecy {
    private:
    const int nx0 = 4;
    int nx1;
    int value;
    void inc() { value += (value == 3) ? 0 : 1; return; }
    void dec() { value -= (value == 0) ? 0 : 1; return; }

    public:
    uint pc;
    prophecy(): pc(-1), nx1(4), value(0) {}
    void restore(const uint &p) {
        pc = p; nx1 = 4; value = 0;
        return; 
    }
    void deal(const int &dx){
        if (dx != 4) nx1 = dx, inc(); else dec();
        return;
    }
    uint ppcy() {
        if (value & 2) return pc+nx1;
        else return pc+nx0;
    }
};

class procy {
    private:
    prophecy *pro;
    uint len;
    uint ins;
    int sum, err;
    uint locate(const uint &x){
        for (int i = 0;i < len;i++){
            if (pro[i].pc == -1) return -1;
            if (pro[i].pc == x) return i;
        }  
        return -1;
    }

    public:
    procy(const uint &l):len(l), ins(0), sum(0), err(0) { pro = new prophecy[len]; }
    ~procy() { delete [] pro; }
    uint quary(const uint &pc){
        uint pos = locate(pc); ++sum;
        if (pos == -1) return pc+4;
        else return pro[pos].ppcy();
    }
    void excer(const uint &pc, const uint &npc,const bool &er = 0){
        uint pos = locate(pc);
        if (er) ++err;
        if (pos == -1) pos = ins, pro[ins++].restore(pc);
        if (ins == len) ins = 0;
        pro[pos].deal(npc-pc);
        return;
    }
    void report(){
        cerr << "sum: " << sum << "   err: " << err << endl;
        cerr << "rate: " << 1.0 - double(err)/sum << endl;
        return;
    }
};