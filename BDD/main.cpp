#include "bdd.h"
#include <fstream>

using namespace std;

#define N 9 // number of objects/values
#define M 4 // number of properties
#define LOG_N 4 // number of variables to encode one property value
#define N_VAR N*M*LOG_N // number of boolean variables
#define K 15

ofstream out;

void fun(char* varset, int size);

void set_constraint_1(bdd& task, bdd p[M][N][N], int prop, int obj, int val);
void set_constraint_2(bdd& task, bdd p[M][N][N], int prop1, int val1, int prop2, int val2);
void set_constraint_3(bdd& task, bdd p[M][N][N], int prop1, int val1, int prop2, int val2, bool left_bottom);
void set_constraint_4(bdd& task, bdd p[M][N][N], int prop1, int val1, int prop2, int val2);
void set_constraint_5(bdd& task, bdd p[M][N][N]);
void set_constraint_6(bdd& task, bdd p[M][N][N]);
void set_constraint_7(bdd& task, bdd p[M][N][N]);

int main(void) {
    // Initialization
    bdd_init(10000000, 100000);
    bdd_setvarnum(N_VAR);
    
    // ->--- Enter function p(k, i, j) as follows (p[k][i][j]):
    bdd p[M][N][N];
    
    for (unsigned obj = 0; obj < N; obj++) {
        for (unsigned val = 0; val < N; val++) {
            for (unsigned prop = 0; prop < M; prop++) {
                p[prop][obj][val] = bddtrue;
                for (unsigned k = 0; k < LOG_N; k++) {
                    p[prop][obj][val] &= ((val >> k) & 1) ? bdd_ithvar(LOG_N * M * obj + LOG_N * prop + k) : bdd_nithvar(LOG_N * M * obj + LOG_N * prop + k);
                }
            }
        }
    }
    // -<---
    
    bdd result = bddtrue;
    
    // Constraints of type 1
    set_constraint_1(result, p, 0, 0, 0);
    set_constraint_1(result, p, 2, 0, 5);
    set_constraint_1(result, p, 3, 0, 3);
    
    set_constraint_1(result, p, 0, 1, 4);
    set_constraint_1(result, p, 1, 1, 5);
    set_constraint_1(result, p, 3, 1, 7);
    
    set_constraint_1(result, p, 0, 2, 8);
    set_constraint_1(result, p, 1, 2, 0);
    set_constraint_1(result, p, 2, 2, 1);
    
    set_constraint_1(result, p, 0, 3, 3);
    set_constraint_1(result, p, 1, 3, 4);
    set_constraint_1(result, p, 3, 3, 0);
    
    set_constraint_1(result, p, 0, 6, 6);
    set_constraint_1(result, p, 1, 6, 7);
    set_constraint_1(result, p, 2, 6, 7);
    
    // Constraints of type 2
    set_constraint_2(result, p, 1, 1, 2, 2);
    set_constraint_2(result, p, 3, 4, 2, 3);
    set_constraint_2(result, p, 2, 6, 3, 7);
    
    set_constraint_2(result, p, 1, 2, 3, 1);
    set_constraint_2(result, p, 2, 4, 1, 4);
    set_constraint_2(result, p, 1, 6, 3, 6);
    
    // Constraints of type 3
    set_constraint_3(result, p, 0, 2, 1, 3, true);
    set_constraint_3(result, p, 1, 3, 2, 4, true);
    set_constraint_3(result, p, 2, 4, 3, 5, false);
    set_constraint_3(result, p, 3, 5, 0, 6, false);
    
    
    // Constraints of type 4
    set_constraint_4(result, p, 0, 1, 0, 2);
    set_constraint_4(result, p, 1, 2, 1, 3);
    set_constraint_4(result, p, 2, 3, 2, 4);
    set_constraint_4(result, p, 3, 4, 3, 5);
    set_constraint_4(result, p, 0, 5, 0, 6);
    set_constraint_4(result, p, 2, 4, 2, 0);
    
    // Constraints of type 5, 6, 7
    set_constraint_5(result, p);
    set_constraint_6(result, p);
    set_constraint_7(result, p);
    
    // Output
    out.open("out.txt");
    unsigned satcount = (unsigned)bdd_satcount(result);
    out << satcount << " solutions:\n" << endl;
    if (satcount) bdd_allsat(result, fun);
    out.close();
    
    bdd_done();
    return 0;
}

void set_constraint_1(bdd& task, bdd p[M][N][N], int prop, int obj, int val) {
    task &= p[prop][obj][val];
}

void set_constraint_2(bdd& task, bdd p[M][N][N], int prop1, int val1, int prop2, int val2) {
    for (unsigned i = 0; i < N; i++)
        task &= !(p[prop1][i][val1] ^ p[prop2][i][val2]);
}

void set_constraint_3(bdd& task, bdd p[M][N][N], int prop1, int val1, int prop2, int val2, bool left_bottom) {
    int offset;
    if (left_bottom) {
        offset = 2;
        for (unsigned i = 0; i < N; i++) {
            if (i % 3 == 1 || i % 3 == 2) {
                if (i + offset < N) {
                    task &= !(p[prop1][i + offset][val1] ^ p[prop2][i][val2]);
                }
            }
        }
    } else {
        offset = 4;
        for (unsigned i = 0; i < N; i++) {
            if (i % 3 == 0 || i % 3 == 1) {
                
                if (i + offset < N) {
                    task &= !(p[prop1][i + offset][val1] ^ p[prop2][i][val2]);
                }
            }
        }
    }
}

void set_constraint_4(bdd& task, bdd p[M][N][N], int prop1, int val1, int prop2, int val2) {
    bdd temp1 = bddtrue;
    bdd temp2 = bddtrue;
    int offset;
    
    offset = 2;
    for (unsigned i = 0; i < N; i++) {
        if (i % 3 == 1 || i % 3 == 2) {
            
            if (i + offset < N) {
                temp1 &= !(p[prop1][i + offset][val1] ^ p[prop2][i][val2]);
            }
        }
    }
    
    offset = 4;
    for (unsigned i = 0; i < N; i++) {
        if (i % 3 == 0 || i % 3 == 1) {
            if (i + offset < N) {
                temp2 &= !(p[prop1][i + offset][val1] ^ p[prop2][i][val2]);
            }
        }
    }
    task &= (temp1 | temp2);
}

void set_constraint_5(bdd& task, bdd p[M][N][N]) {
    for (unsigned i = 0; i < N - 1; i++)
        for (unsigned k = i + 1; k < N; k++)
            for (unsigned j = 0; j < N; j++)
                for (unsigned m = 0; m < M; m++)
                    task &= (p[m][i][j] >> !p[m][k][j]);
}

void set_constraint_6(bdd& task, bdd p[M][N][N]) {
    for (unsigned i = 0; i < N; i++) {
        bdd temp = bddtrue;
        for (unsigned j = 0; j < M; j++) {
            bdd tempInner = bddfalse;
            for (unsigned k = 0; k < N; k++) {
                tempInner |= p[j][i][k];
            }
            temp &= tempInner;
        }
        task &= temp;
    }
}

void set_constraint_7(bdd& task, bdd p[M][N][N]) {
    int offset;
    
    for (unsigned i = 0; i < N; i++) {
        for (unsigned p0 = 0; p0 < N; p0++) {
            for (unsigned p1 = 0; p1 < N; p1++) {
                if (i % 3 == 0) {
                    offset = 4;
                    if (i + offset < N) {
                        if (p0 + p1 > K) {
                            for (int j = 0; j < M; j++) {
                                task &= !(p[j][i][p0] & p[j][i + offset][p1]);
                            }
                        }
                    } else {
                        if (p1 > K) {
                            for (int j = 0; j < M; j++) {
                                task &= !(p[j][i][p1]);
                            }
                        }
                    }
                } else if (i % 3 == 2) {
                    offset = 2;
                    if (i + offset < N) {
                        if (p0 + p1 > K) {
                            for (int j = 0; j < M; j++) {
                                task &= !(p[j][i][p0] & p[j][i + offset][p1]);
                            }
                        }
                    } else {
                        if (p1 > K) {
                            for (int j = 0; j < M; j++) {
                                task &= !(p[j][i][p1]);
                            }
                        }
                    }
                }
                
                for (unsigned p2 = 0; p2 < N; p2++) {
                    if (i % 3 == 1) {
                        int offset1 = 2;
                        int offset2 = 4;
                        
                        if (i + offset2 < N) {
                            if (p0 + p1 + p2 > K) {
                                for (int j = 0; j < M; j++) {
                                    task &= !(p[j][i][p0] & p[j][i + offset1][p1] & p[j][i + offset2][p2]);
                                }
                            }
                        } else {
                            if (p2 > K) {
                                for (int j = 0; j < M; j++) {
                                    task &= !(p[j][i][p2]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

char var[N_VAR];

void print(void) {
    for (unsigned i = 0; i < N; i++) {
        out << i << ": ";
        for (unsigned j = 0; j < M; j++) {
            int J = i * M * LOG_N + j * LOG_N;
            int num = 0;
            for (unsigned k = 0; k < LOG_N; k++) num += (unsigned)(var[J + k] << k);
            out << num << ' ';
        }
        out << endl;
    }
    out << endl;
}

void build(char* varset, unsigned n, unsigned I) {
    if (I == n - 1) {
        if (varset[I] >= 0) {
            var[I] = varset[I];
            print();
            return;
        }
        var[I] = 0;
        print();
        var[I] = 1;
        print();
        return;
    }
    
    if (varset[I] >= 0) {
        var[I] = varset[I];
        build(varset, n, I + 1);
        return;
    }
    
    var[I] = 0;
    build(varset, n, I + 1);
    var[I] = 1;
    build(varset, n, I + 1);
}

void fun(char* varset, int size) {
    build(varset, size, 0);
}
