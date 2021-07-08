#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include "classes.hpp"

using namespace std;
using uint = unsigned;
using cdptr = comd *;
using daptr = data *;
using drptr = data_R *;
using diptr = data_I *;
using dsptr = data_S *;
using dbptr = data_B *;
using duptr = data_U *;
using djptr = data_J *;

const uint table[6][4] = 
    {{1, 51, 0, 0}, {3, 19, 3, 103}, {1, 35, 0, 0}, {1, 99, 0, 0}, {2, 55, 23, 0}, {1, 111, 0, 0}};
const uint branch_type[3] = {111, 103, 99};
commands anatype(const uint &cmd){
    uint op = Binum(cmd).slice(0, 7);
    for (int i = 0;i < 6;i++){
        int m = table[i][0];
        for (int j = 1;j <= m;j++)
            if (op == table[i][j]) return commands(i);
    }
    return commands(6);
}
bool is_branch(const uint &cmd){
    uint op = Binum(cmd).slice(0, 7);
    for (int i = 0;i < 3;i++)
        if (op == branch_type[i]) return 1;
    return 0;
}
daptr sol_R(const cdptr cdp, const uint *rg){
    Binum bins(cdp->cmd);
    uint op = bins.slice(0, 7);
    uint rd = bins.slice(7, 12);
    uint fun1 = bins.slice(12, 15);
    uint rs1 = rg[bins.slice(15, 20)];
    uint rs2 = rg[bins.slice(20, 25)];
    uint fun2 = bins.slice(25, 32);
    return new data_R(cdp->pc, R, op, fun1, fun2, rs1, rs2, rd);
}
daptr sol_I(const cdptr cdp, const uint *rg){
    Binum bins(cdp->cmd);
    uint op = bins.slice(0, 7);
    uint rd = bins.slice(7, 12);
    uint fun = bins.slice(12, 15);
    uint rs = rg[bins.slice(15, 20)];
    uint imm = Binum(bins.slice(20, 32), 12).extended();
    return new data_I(cdp->pc, I, op, fun, rs, rd, imm);
}
daptr sol_S(const cdptr cdp, const uint *rg){
    Binum bins(cdp->cmd);
    uint op = bins.slice(0, 7);
    uint imm1 = bins.slice(7, 12);
    uint fun = bins.slice(12, 15);
    uint rs1 = rg[bins.slice(15, 20)];
    uint rs2 = rg[bins.slice(20, 25)];
    uint imm2 = bins.slice(25, 32);
    uint imm = imm1 + (imm2<<5);
    imm = Binum(imm, 12).extended();
    return new data_S(cdp->pc, S, op, fun, rs1, rs2, imm);
}
daptr sol_B(const cdptr cdp, const uint *rg){
    Binum bins(cdp->cmd);
    uint op = bins.slice(0, 7);
    uint imm1 = bins.slice(7, 8);
    uint imm2 = bins.slice(8, 12);
    uint fun = bins.slice(12, 15);
    uint rs1 = rg[bins.slice(15, 20)];
    uint rs2 = rg[bins.slice(20, 25)];
    uint imm3 = bins.slice(25, 31);
    uint imm4 = bins.slice(31, 32);
    uint imm = (imm1<<11) + (imm2<<1) + (imm3<<5) + (imm4<<12);
    imm = Binum(imm, 13).extended();
    return new data_B(cdp->pc, B, op, fun, rs1, rs2, imm);
}
daptr sol_U(const cdptr cdp, const uint *rg){
    Binum bins(cdp->cmd);
    uint op = bins.slice(0, 7);
    uint rd = bins.slice(7, 12);
    uint imm = bins.slice(12, 32);
    imm <<= 12;
    return new data_U(cdp->pc, U, op, rd, imm);
}
daptr sol_J(const cdptr cdp, const uint *rg){
    Binum bins(cdp->cmd);
    uint op = bins.slice(0, 7);
    uint rd = bins.slice(7, 12);
    uint imm1 = bins.slice(12, 20);
    uint imm2 = bins.slice(20, 21);
    uint imm3 = bins.slice(21, 31);
    uint imm4 = bins.slice(31, 32);
    uint imm = (imm3<<1) + (imm2<<11) + (imm1<<12) + (imm4<<20);
    imm = Binum(imm, 21).extended();
    return new data_J(cdp->pc, J, op, rd, imm);
}

daptr sol(const cdptr cdp, const uint *rg){
    commands t(anatype(cdp->cmd));
    switch (t)
    {
    case R: return sol_R(cdp, rg);
    case I: return sol_I(cdp, rg);
    case S: return sol_S(cdp, rg);
    case B: return sol_B(cdp, rg);
    case U: return sol_U(cdp, rg);
    case J: return sol_J(cdp, rg);
    default:{
        cerr << "wrong type" << endl;
        return nullptr;
        break;
    }  
    }
}

void des(daptr dat){
    if (!dat) return;
    switch (dat->cd)
    {
    case R: { delete drptr(dat); return; }
    case I: { delete diptr(dat); return; }
    case S: { delete dsptr(dat); return; }
    case B: { delete dbptr(dat); return; }
    case U: { delete duptr(dat); return; }
    case J: { delete djptr(dat); return; }
    default:{ cerr << "wrong type" << endl; return; }  
    }
}

void hazload(const uint &cmd, hazdata *haz, int &h){
    commands t(anatype(cmd));
    Binum bins(cmd);
    switch (t)
    {
    case R: {
        uint rs = bins.slice(15, 20);
        if (rs) haz[h++].rd = rs;
        rs = bins.slice(20, 25);
        if (rs) haz[h++].rd = rs;
        break;
    }
    case I: {
        uint rs = bins.slice(15, 20);
        if (rs) haz[h++].rd = rs;
        break;
    }
    case S: {
        uint rs = bins.slice(15, 20);
        if (rs) haz[h++].rd = rs;
        rs = bins.slice(20, 25);
        if (rs) haz[h++].rd = rs;
        break;
    }
    case B: {
        uint rs = bins.slice(15, 20);
        if (rs) haz[h++].rd = rs;
        rs = bins.slice(20, 25);
        if (rs) haz[h++].rd = rs;
        break;
    }
    default: return;
    }
}