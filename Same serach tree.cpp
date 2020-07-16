#include <iostream>
typedef struct TreeNode* Tree;
struct TreeNode{
    int v;
    Tree left;
    Tree right;
    int flag;
};
Tree NewNode(int V){
    Tree T;
    T = (Tree) malloc(sizeof(struct TreeNode));
    T -> v = V;
    T -> left = T -> right = nullptr;
    T -> flag = 0;
    return T;
}
Tree Insert(Tree T, int V){
    if (!T)
        T = NewNode(V);
    else{
        if (V < T -> v)
            T -> left  = Insert(T -> left, V);
        else
            T -> right = Insert(T -> right, V);
    }
    return T;
}
Tree MakeTree(int N){
    Tree T;
    int i, V;
    scanf("%d",&V);
    T = NewNode(V);
    for (i = 1; i < N; i++){
        scanf("%d",&V);
        T = Insert(T,V);
    }
    return T;
}
int check ( Tree T, int V )
{
    if ( T -> flag ) {
        if ( V < T -> v ) return check(T -> left, V);
        else if ( V > T -> v ) return check(T -> right, V);
        else return 0;
    }
    else {
        if ( V == T->v ) {
            T->flag = 1;
            return 1;
        }
        else return 0;
    } }
int Judge(Tree T, int N){
    int V, i;
    int flag = 0;
    scanf("%d",&V);
    if (V != T -> v) flag = 1;
    else T -> flag = 1;
    for (i = 1; i < N; i++){
        scanf("%d",&V);
        if ((!flag) && (!check(T,V)))
            flag = 1;
    }
    if (flag == 1) return 0;
    else return 1;
}
void ResetT(Tree T){
    if (T -> left) ResetT(T -> left);
    if (T -> right) ResetT(T -> right);
    T -> flag = 0;
}
void FreeTree(Tree T){
    if (T -> left) FreeTree(T -> left);
    if (T -> right) FreeTree( T -> right);
    free (T);
}
int main() {
    int N, L;
    Tree T;
    scanf("%d",&N);
    while (N){
        scanf("%d",&L);
        T = MakeTree(N);
        for (int i = 0; i < L; i++){
            if (Judge(T , N))
                printf("Yes\n");
            else
                printf("No\n");
            ResetT(T);
        }
        FreeTree(T);
        scanf("%d",&N);
    }
    return 0;
}
