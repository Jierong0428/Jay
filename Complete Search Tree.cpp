#include <iostream>
#include<cmath>
#define MAXN 1000
int T[MAXN], A[MAXN];
int GetLeftLength(int N){
    int H, X, L;
    H = log2(N + 1);
    X = N + 1 - pow(2,H);
    if (X > pow(2, H - 1))
        X = pow(2, H - 1);
    L = pow(2,H - 1) + X - 1;
    return L;
}
void Solve(int ALeft,int ARight, int TRoot){
    int n, L;
    int LeftRoot, RightRoot;
    n = ARight - ALeft + 1;
    if (n == 0)
        return;
    L = GetLeftLength(n);
    T[TRoot] = A[ALeft + L];
    LeftRoot = TRoot * 2 + 1;
    RightRoot = LeftRoot + 1;
    Solve(ALeft, ALeft + L - 1, LeftRoot);
    Solve(ALeft + L + 1, ARight, RightRoot);
}
int Compare(const void* a, const void* b){
    return *(int*)a - *(int*)b;
}
int main() {
    int N,i;
    scanf("%d\n",&N);
    for (i = 0; i < N; i++)
        scanf("%d",&A[i]);
    qsort(A, N, sizeof(int), Compare);
    Solve(0,N - 1,0);
    bool first = true;
    for (i = 0; i < N;i++) {
        if (not first)
            printf(" %d", T[i]);
        else {
            first = false;
            printf("%d", T[i]);
        }
    }
    return 0;
}
