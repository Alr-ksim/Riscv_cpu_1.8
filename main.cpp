#pragma GCC optimize(2)

#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#include <queue>
#include "transfer.hpp"
#include "analysis.hpp"
#include "carried.hpp"
#include "loads.hpp"
#include "prophecy.hpp"

using namespace std;
using uint = unsigned;
using irptr = ins_R *;
cdptr cacdp;
daptr cadat;
inptr cainp;
wrptr cawrp;

const int Csize = 6;
const int Psize = 15;
const int Tsize = 32;
uint reg[32];
char ram[1 << 20];
int cains, proins;
uint tem[Tsize];
cdptr code_cache[Csize];
procy pro(Psize);

inline void readle(){
    char s[64]; int cur = 0;
        while (scanf("%s", s) != EOF){
            if (s[0] == '@') { cur = transfer(s+1); continue;}
            ram[cur++] = transfer(s);
        }
}

inline uint cdp_pc(const cdptr &cdp){
    if (cdp) return cdp->pc;
    return -1;
}
inline cdptr code_catch(const uint &pc){
    for (int i = 0;i < Csize;i++){
        if (!code_cache[i]) break;
        if (code_cache[i]->pc == pc) return new comd(pc, code_cache[i]->cmd);
    }
    uint cmd = fetch(ram, pc, 4);
    if (code_cache[cains]) delete code_cache[cains], code_cache[cains] = nullptr;
    code_cache[cains++] = new comd(pc, cmd);
    if (cains == Csize) cains = 0;
    return new comd(pc, cmd);
}
bool tryif(const uint &pc, cdptr &cdp){
    cdp = code_catch(pc);
    if (!cdp->cmd || cdp->cmd == 0x0ff00513){
        if (cdp) delete cdp;
        cdp = nullptr;
        return 0;
    }
    return 1;
}
void reload(uint &pc, uint &npc, const uint &jpc){
    if (cacdp) delete cacdp, cacdp = nullptr;
    if (tryif(jpc, cacdp)) {
        pc = jpc;
        if (is_branch(cacdp->cmd)) npc = pro.quary(pc);
        else npc = pc + 4;
        return; 
    }
    else { pc = jpc-4; npc = jpc; return; }
}

void hazsol(hazdata *haz1,const int &h1, hazdata *haz2, const int &h2, daptr dap){
    for (int i = 0;i < h1;i++)
        for (int j = 0;j < h2;j++){
            if (haz1[i].rd == haz2[j].rd) return;
        }
    return;
}

void halt(){
    printf("%u", reg[10] & (255u));
    for (int i = 0;i < Csize;i++)
        if (code_cache[i]) delete code_cache[i], code_cache[i] = nullptr;
    return;
}

int main(){
    // freopen("sample.data", "r", stdin);
    // freopen("output.txt", "w", stdout);
    readle();
    hazdata haz1[2], haz2[2];
    for (int i = 0;i < 2;i++) haz1[i] = haz2[i] = {0u, 0u};
    uint pc = 0, npc = 4;
    cacdp = code_catch(pc);
    int h1, h2, cnt = 1;
    int i = 0;
    while (true) {
        // printf("\n-----------------------------------------------------------\n");
        // printf("Pc: %u, Npc: %u\n", pc, npc);
        if (!cnt) break;
        h1 = 0, h2 = 0, cnt = 0;
        if (cawrp) {
            // cawrp->tout();
            wrstation(cawrp, reg), delete cawrp, cawrp = nullptr;
        }
        if (cainp) {
            // cainp->tout();
            cawrp = instation(cainp, ram), des(cainp), cainp = nullptr;
            if (cawrp) ++cnt;
            if (cawrp && cawrp->rd) haz1[h1++] = {cawrp->imm, cawrp->rd};
        }
        if (cadat) {
            // cadat->tout();
            uint opc(cadat->pc), jpc(cadat->pc);
            bool flag = is_branch(cadat->op);
            cainp = transation(cadat, jpc), des(cadat), cadat = nullptr;
            if (cainp) ++cnt;
            if (cainp && (cainp->type == 1) && irptr(cainp)->rd)
                haz1[h1++] = {(car_R(irptr(cainp), ram)->imm), irptr(cainp)->rd};
            if (cainp && (cainp->type == 3) && wrptr(cainp)->rd)
                haz1[h1++] = {wrptr(cainp)->imm, wrptr(cainp)->rd};
            if (flag){
                if (opc == jpc) jpc = opc + 4; 
                if (jpc != cdp_pc(cacdp)) pro.excer(opc, jpc, 1), reload(pc, npc, jpc);
                else pro.excer(opc, jpc, 0);
            }
        } 
        if (cacdp) {
            // cacdp->tout();
            hazload(cacdp->cmd, haz2, h2);
            for (int i = 0;i < h2;i++) tem[haz2[i].rd] = reg[haz2[i].rd];
            for (int i = 0;i < h1;i++) tem[haz1[i].rd] = haz1[i].imm;
            cadat = sol(cacdp, tem), delete cacdp, cacdp = nullptr;
            if (cadat) ++cnt;
        }
        if (tryif(npc, cacdp)) {
            pc = npc; ++cnt;
            if (is_branch(cacdp->cmd)) npc = pro.quary(pc);
            else npc = pc+4;
        }
    }
    halt();
    // pro.report();
    return 0;
}