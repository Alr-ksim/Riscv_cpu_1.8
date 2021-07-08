#include <cstdio>
#include <iostream>
#include <cstring>
#include "classes.hpp"
#include "transfer.hpp"

using namespace std;
using inptr = insp *;
using irptr = ins_R *;
using iwptr = ins_W *;
using wrptr = wreg *;

wrptr car_R(const irptr &dat, const char *rm){
    uint imm = fetch(rm, dat->ad, dat->bit);
    if (dat->ex) imm = Binum(imm, 8*dat->bit).extended();
    return new wreg(3, imm, dat->rd);
}
wrptr car_W(const iwptr &dat, char *rm){
    Binum imm(dat->imm);
    for (int i = 0;i < dat->bit;i++){
        rm[dat->ad + i] = imm.slice(8*i, 8*(i+1));
    }
    return nullptr;
}

wrptr instation(const inptr &dat, char *rm){
    switch (dat->type)
    {
    case 1: return car_R(irptr(dat), rm);
    case 2: return car_W(iwptr(dat), rm);
    case 3: return wrptr(dat);
    default: return nullptr;
    }
}

void des(inptr dat){
    if (!dat) return;
    switch (dat->type)
    {
    case 1: { delete irptr(dat); return; }
    case 2: { delete iwptr(dat); return; }
    case 3: { return; }
    default: { cerr << "bad inp " << dat->type << endl; return; }
    }
}

void wrstation(const wrptr &dat, uint *rg){
    rg[dat->rd] = dat->imm;
    if (dat->rd == 0) rg[dat->rd] = 0;
    return;
}