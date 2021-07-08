#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include "classes.hpp"

using namespace std;
using daptr = data *;
using drptr = data_R *;
using diptr = data_I *;
using dsptr = data_S *;
using dbptr = data_B *;
using duptr = data_U *;
using djptr = data_J *;
using inptr = insp *;


inptr ADD(const drptr &dat){
    uint imm = dat->rs1 + dat->rs2;
    return new wreg(3, imm, dat->rd);
}
inptr SLL(const drptr &dat){
    uint offset = Binum(dat->rs2).slice(0, 5);
    uint imm = dat->rs1 << offset;
    return new wreg(3, imm, dat->rd);
}
inptr SLT(const drptr &dat){
    int r1 = dat->rs1; int r2 = dat->rs2;
    uint imm = (r1<r2) ? 1 : 0;
    return new wreg(3, imm, dat->rd);
}
inptr SLTU(const drptr &dat){
    uint r1 = dat->rs1; uint r2 = dat->rs2;
    uint imm = (r1<r2) ? 1 : 0;
    return new wreg(3, imm, dat->rd);
}
inptr XOR(const drptr &dat){
    uint imm = dat->rs1 ^ dat->rs2;
    return new wreg(3, imm, dat->rd);
}
inptr SRL(const drptr &dat){
    uint offset = Binum(dat->rs2).slice(0, 5);
    uint imm = dat->rs1 >> offset;
    return new wreg(3, imm, dat->rd);
}
inptr OR(const drptr &dat){
    uint imm = dat->rs1 | dat->rs2;
    return new wreg(3, imm, dat->rd);
}
inptr AND(const drptr &dat){
    uint imm = dat->rs1 & dat->rs2;
    return new wreg(3, imm, dat->rd);
}
inptr SUB(const drptr &dat){
    uint imm = dat->rs1 - dat->rs2;
    return new wreg(3, imm, dat->rd);
}
inptr SRA(const drptr &dat){
    uint offset = Binum(dat->rs2).slice(0, 5);
    uint r1 = dat->rs1;
    uint imm = r1 >> offset;
    if ((1<<31) & r1)
        for (int i = 0;i < offset;i++)
            imm += 1<<(31-i);
    return new wreg(3, imm, dat->rd);
}
inptr ADDI(const diptr &dat){
    uint imm = dat->rs + dat->imm;
    return new wreg(3, imm, dat->rd);
}
inptr SLLI(const diptr &dat){
    uint shamt = Binum(dat->imm).slice(0, 5);
    uint imm = dat->rs << shamt;
    return new wreg(3, imm, dat->rd);
}
inptr SLTI(const diptr &dat){
    int r1 = dat->rs; int im = dat->imm;
    uint imm = (r1<im) ? 1 : 0;
    return new wreg(3, imm, dat->rd);
}
inptr SLTIU(const diptr &dat){
    uint r1 = dat->rs; uint im = dat->imm;
    uint imm = (r1<im) ? 1 : 0;
    return new wreg(3, imm, dat->rd);
}
inptr XORI(const diptr &dat){
    uint imm = dat->rs ^ dat->imm;
    return new wreg(3, imm, dat->rd);
}
inptr SRLI(const diptr &dat){
    uint shamt = Binum(dat->imm).slice(0, 5);
    uint imm = dat->rs >> shamt;
    return new wreg(3, imm, dat->rd);
}
inptr SRAI(const diptr &dat){
    uint shamt = Binum(dat->imm).slice(0, 5);
    uint imm = dat->rs >> shamt;
    imm = Binum(imm, 32-shamt).extended();
    return new wreg(3, imm, dat->rd);
}
inptr SRI(const diptr &dat){
    switch (Binum(dat->imm).slice(5, 11))
    {
    case 0: return SRLI(dat);
    case 32: return SRAI(dat);
    default: break;
    }
}
inptr ORI(const diptr &dat){
    uint imm = dat->rs | dat->imm;
    return new wreg(3, imm, dat->rd);
}
inptr ANDI(const diptr &dat){
    uint imm = dat->rs & dat->imm;
    return new wreg(3, imm, dat->rd);
}
inptr LB(const diptr &dat){
    uint ad = dat->rs + dat->imm;
    return new ins_R(1, 1, dat->rd, ad, 1);
}
inptr LH(const diptr &dat){
    uint ad = dat->rs + dat->imm;
    return new ins_R(1, 2, dat->rd, ad, 1);
}
inptr LW(const diptr &dat){
    uint ad = dat->rs + dat->imm;
    return new ins_R(1, 4, dat->rd, ad, 1);
}
inptr LBU(const diptr &dat){
    uint ad = dat->rs + dat->imm;
    return new ins_R(1, 1, dat->rd, ad, 0);
}
inptr LHU(const diptr &dat){
    uint ad = dat->rs + dat->imm;
    return new ins_R(1, 2, dat->rd, ad, 0);
}
inptr JALR(const diptr &dat, uint &pc){
    uint opc = pc + 4;
    pc = (dat->rs + dat->imm) & (~1);
    return new wreg(3, opc, dat->rd);
}
inptr SB(const dsptr &dat){
    uint ad = dat->rs1 + dat->imm;
    return new ins_W(2, 1, dat->rs2, ad);
}
inptr SH(const dsptr &dat){
    uint ad = dat->rs1 + dat->imm;
    return new ins_W(2, 2, dat->rs2, ad);
}
inptr SW(const dsptr &dat){
    uint ad = dat->rs1 + dat->imm;
    return new ins_W(2, 4, dat->rs2, ad);
}
inptr BEQ(const dbptr &dat, uint &pc){
    if (dat->rs1 == dat->rs2) pc += dat->imm;
    return nullptr;
}
inptr BNE(const dbptr &dat, uint &pc){
    if (dat->rs1 != dat->rs2) pc += dat->imm;
    return nullptr;
}
inptr BLT(const dbptr &dat, uint &pc){
    if (int(dat->rs1) < int(dat->rs2)) pc += dat->imm;
    return nullptr;
}
inptr BGE(const dbptr &dat, uint &pc){
    if (int(dat->rs1) >= int(dat->rs2)) pc += dat->imm;
    return nullptr;
}
inptr BLTU(const dbptr &dat, uint &pc){
    if (dat->rs1 < dat->rs2) pc += dat->imm;
    return nullptr;
}
inptr BGEU(const dbptr &dat, uint &pc){
    if (dat->rs1 >= dat->rs2) pc += dat->imm;
    return nullptr;
}
inptr LUI(const duptr &dat){
    return new wreg(3, dat->imm, dat->rd);
}
inptr AUIPC(const duptr &dat, const uint &pc){
    return new wreg(3, dat->imm + pc, dat->rd);
}
inptr JAL(const djptr &dat, uint &pc){
    uint opc = pc + 4;
    pc += dat->imm;
    return new wreg(3, opc, dat->rd);
}

inptr deal_R(const drptr &dat){
    switch (dat->fun2)
    {
    case 0:{
        switch (dat->fun1)
        {
        case 0: return ADD(dat);
        case 1: return SLL(dat);
        case 2: return SLT(dat);
        case 3: return SLTU(dat);
        case 4: return XOR(dat);
        case 5: return SRL(dat);
        case 6: return OR(dat);
        case 7: return AND(dat);
        default: break;
        }
        break;
    }
    case 32:{
        switch (dat->fun1)
        {
        case 0: return SUB(dat);
        case 5: return SRA(dat);
        default:
            break;
        }
        break;
    }  
    default: break;
    }
}
inptr deal_I(const diptr &dat, uint &pc){
    switch (dat->op)
    {
    case 19: {
        switch (dat->fun)
        {
        case 0: return ADDI(dat);
        case 1: return SLLI(dat);
        case 2: return SLTI(dat);
        case 3: return SLTIU(dat);
        case 4: return XORI(dat);
        case 5: return SRI(dat);
        case 6: return ORI(dat);
        case 7: return ANDI(dat);
        default: break;
        }
        break;
    }
    case 3: {
        switch (dat->fun)
        {
        case 0: return LB(dat);
        case 1: return LH(dat);
        case 2: return LW(dat);
        case 4: return LBU(dat);
        case 5: return LHU(dat);
        default: break;
        }
        break;
    }
    case 103: {
        switch (dat->fun)
        {
        case 0: return JALR(dat, pc);
        default: break;
        }
    }
    default:
        break;
    }
}
inptr deal_S(const dsptr &dat){
    switch (dat->fun)
    {
    case 0: return SB(dat);
    case 1: return SH(dat);
    case 2: return SW(dat);
    default: break;
    }
}
inptr deal_B(const dbptr &dat, uint &pc){
    switch (dat->fun)
    {
    case 0: return BEQ(dat, pc);
    case 1: return BNE(dat, pc);
    case 4: return BLT(dat, pc);
    case 5: return BGE(dat, pc);
    case 6: return BLTU(dat, pc);
    case 7: return BGEU(dat, pc);
    default: break;
    }
}
inptr deal_U(const duptr &dat, uint &pc){
    switch (dat->op)
    {
    case 55: return LUI(dat);
    case 23: return AUIPC(dat, pc);
    default: break;
    }
}
inptr deal_J(const djptr &dat, uint &pc){
    switch (dat->op)
    {
    case 111: return JAL(dat, pc);
    default: break;
    }
}

inptr transation(daptr dat, uint &pc){
    switch (dat->cd)
    {
    case R: return deal_R(drptr(dat));
    case I: return deal_I(diptr(dat), pc);
    case S: return deal_S(dsptr(dat));
    case B: return deal_B(dbptr(dat), pc);
    case U: return deal_U(duptr(dat), pc);
    case J: return deal_J(djptr(dat), pc);
    default: break;
    }
}