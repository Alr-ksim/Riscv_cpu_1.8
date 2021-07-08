#ifndef TRANSFER
#define TRANSFER

#include <cstdio>
#include <iostream>
#include <cstring>
#include "classes.hpp"

using namespace std;
using uint = unsigned;

inline bool isnum(const char &ch){
    return ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F'));
}
inline int digit(const char &ch){
    if (ch >= '0' && ch <= '9') return (ch - '0');
    else return (ch - 'A' + 10);
}
int transfer(char *s){
    uint x = 0; uint len = strlen(s);
    for (int i = 0;i < len; i++) 
        x = (x << 4) + digit(s[i]);
    return x;
}

uint fetch(const char *src, uint cur, int bit){
    uint x = 0;
    for (int i = 0;i < bit;i++)
        x += (Binum(uint(src[cur+i])).slice(0, 8)) << (i<<3);
    return x;
}

#endif