#ifndef CLASSES
#define CLASSES

#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;
using uint = unsigned;


enum commands {R = 0, I, S, B, U, J, W};

class Binum {
    private:
    uint data;
    int len;

    public:
    Binum(const uint &d, const int &l = 32): data(d), len(l) {}
    Binum(uint &&d, const int &l = 32): data(d), len(l) {}
    uint slice(const int &l, const int &r){
        uint x = data;
        x <<= (32-r);
        x >>= (32-(r-l));
        return x;
    }
    uint extended(){
        if ((len == 32) || !(data & (1<<(len-1)))) return data;
        uint x = ~((1<<len)-1);
        return data+x;
    }
    void tout(){
        for (int i = len-1;i >= 0;i--)
            if (data & (1<<i)) printf("1");
            else printf("0");
        printf("\n");
        return;
    }
};

class comd {
    public:
    uint cmd, pc;
    comd(uint p, uint c): pc(p), cmd(c) {}
    void tout(){
        printf("#PC = %u\n", pc);
        Binum(cmd).tout();
    }
};

class data {
    public:
    commands cd;
    uint pc;
    uint op;
    data() : cd(W) {};
    data(uint p, commands c, uint o) :pc(p), cd(c), op(o) {}
    virtual void tout() = 0;
};

class data_R : public data {
    public:
    uint fun1, fun2;
    uint rs1, rs2, rd;
    data_R(uint p, commands c, uint o, uint f1, uint f2, uint r1, uint r2, uint r):
        data(p, c, o), fun1(f1), fun2(f2), rs1(r1), rs2(r2), rd(r) {}
    virtual void tout() {
        printf("#R : \n");
        printf("OP: "); Binum(op, 7).tout();
        printf("F1: "); Binum(fun1, 3).tout();
        printf("F2: "); Binum(fun2, 7).tout();
        printf("R1: %d\n", rs1); // Binum(rs1).tout();
        printf("R2: %d\n", rs2); // Binum(rs2).tout();
        printf("Rd: "); Binum(rd, 5).tout();
    }
};
class data_I : public data {
    public:
    uint fun;
    uint rs, rd;
    uint imm;
    data_I(uint p, commands c, uint o, uint f, uint r1, uint r, uint im):
        data(p, c, o), fun(f), rs(r1), rd(r), imm(im) {}
    virtual void tout() {
        printf("#I : \n");
        printf("OP: "); Binum(op, 7).tout();
        printf("F1: "); Binum(fun, 3).tout();
        printf("RS: %d\n", rs); // Binum(rs).tout();
        printf("Rd: "); Binum(rd, 5).tout();
        printf("Im: "); Binum(imm, 32).tout();
    }
};
class data_S : public data {
    public:
    uint fun;
    uint rs1, rs2;
    uint imm;
    data_S(uint p, commands c, uint o, uint f, uint r1, uint r2, uint im):
        data(p, c, o), fun(f), rs1(r1), rs2(r2), imm(im) {}
    virtual void tout() {
        printf("#S : \n");
        printf("OP: "); Binum(op, 7).tout();
        printf("F1: "); Binum(fun, 3).tout();
        printf("R1: %d\n", rs1); // Binum(rs1).tout();
        printf("R2: %d\n", rs2); // Binum(rs2).tout();
        printf("Im: "); Binum(imm, 32).tout();
    }
};
class data_B : public data {
    public:
    uint fun;
    uint rs1, rs2;
    uint imm;
    data_B(uint p, commands c, uint o, uint f, uint r1, uint r2, uint im):
        data(p, c, o), fun(f), rs1(r1), rs2(r2), imm(im) {}
    virtual void tout() {
        printf("#B : \n");
        printf("OP: "); Binum(op, 7).tout();
        printf("F1: "); Binum(fun, 3).tout();
        printf("R1: %d\n", rs1); // Binum(rs1).tout();
        printf("R2: %d\n", rs2); // Binum(rs2).tout();
        printf("Im: "); Binum(imm, 32).tout();
    }
};
class data_U : public data {
    public:
    uint rd;
    uint imm;
    data_U(uint p, commands c, uint o, uint r, uint im): data(p, c, o), rd(r), imm(im) {}
    virtual void tout() {
        printf("#U : \n");
        printf("OP: "); Binum(op, 7).tout();
        printf("Rd: "); Binum(rd, 5).tout();
        printf("Im: "); Binum(imm, 32).tout();
    }
};
class data_J : public data {
    public:
    uint rd;
    uint imm;
    data_J(uint p, commands c, uint o, uint r, uint im): data(p, c, o), rd(r), imm(im) {}
    virtual void tout() {
        printf("#J : \n");
        printf("OP: "); Binum(op, 7).tout();
        printf("Rd: "); Binum(rd, 5).tout();
        printf("Im: "); Binum(imm, 32).tout();
    }
};

class insp {
    public:
    int type;
    insp(): type(0) {}
    insp(int t): type(t) {}
    virtual void tout() = 0;
};

class ins_R : public insp {
    public:
    int bit, ex;
    uint rd;
    uint ad;
    ins_R(int t, int b, uint r, uint a, int e): insp(t), bit(b), rd(r), ad(a), ex(e) {}
    void tout(){
        printf("#RE: \n");
        printf("read: %d bit to %d from %d\n", bit, rd, ad);
        return;
    }
};
class ins_W : public insp {
    public:
    int bit;
    uint imm;
    uint ad;
    ins_W(int t, int b, uint i, uint a): insp(t), bit(b), imm(i), ad(a) {}
    void tout(){
        printf("#WR: \n");
        printf("writ: %d bit to %d from %d\n", bit, ad, imm);
        return;
    }
};

class wreg : public insp{
    public:
    uint imm;
    uint rd;
    wreg(int t, int i, int r): insp(t), imm(i), rd(r) {}
    void tout(){
        printf("#WB: \n");
        printf("writ: %d to %d\n", imm, rd);
        return;
    }
};

struct hazdata{
    uint imm, rd;
};

#endif