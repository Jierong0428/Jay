#include <iostream>
#define MAXN 1001
#define MINH -10001
int H[MAXN], size = 0;
void Create(){
    H[0] = MINH;
    size = 0;
}
void Insert(int x){
    int i;
    for(i = ++size; H[i / 2] > x; i /= 2)
        H[i] = H[i / 2];
    H[i] = x;

}
int main() {
    int N, M,x,i,j;
    scanf("%d %d",&N, &M);
    Create();
    for (i = 0; i < N; i++){
        scanf("%d",&x);
        Insert(x);
    }
    for (i = 0; i < M; i++){
        scanf("%d",&j);
        printf("%d",H[j]);
        while (j > 1){
            j /= 2;
            printf(" %d",H[j]);
        }
        printf("\n");
    }
    return 0;
}
