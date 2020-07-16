#include <iostream>
#include <stdio.h>
#define  ERROR -1
int *in, *pre;
int flag = 0;
typedef int ElementType;
typedef int Position;
struct SNode {
    ElementType* Data;
    Position  Top;
    int Maxsize;
};
typedef SNode* Stack;
Stack CreateStack(int Maxsize){
    Stack S = (Stack) malloc(sizeof(struct SNode));
    S -> Data = (ElementType*) malloc((sizeof(ElementType) * Maxsize));
    S -> Top = -1;
    S -> Maxsize = Maxsize;
    return S;
}
bool Isfull(Stack S){
    return (S -> Top == S -> Maxsize - 1);
}
bool IsEmpty(Stack S){
    return (S -> Top == -1);
}
ElementType pop(Stack S){
    if (IsEmpty(S))
        return ERROR;
    return (S -> Data[(S -> Top)--]);
}
bool Push(Stack S, ElementType X){
    if (Isfull(S))
        return false;
    S -> Data[++(S -> Top)] = X;
    return true;
}
void Stackclear(Stack S){
    S -> Top = -1;
}
void Post(int root, int start, int end){
    if (start > end)
        return;
    int i = start;
    while (i < end && in[i] != pre[root])
        i++;
    Post(root + 1, start, i - 1);
    Post(root + i - start + 1, i + 1, end);
    if (!flag){
        printf("%d",pre[root]);
        flag = 1;
    }
    else
        printf(" %d", pre[root]);

}
int main() {
    int N;
    int i = 0 ,j  = 0, val;
    scanf("%d", &N);
    char s[10];
    Stack S = CreateStack(N);
    in = (int*) malloc(sizeof(int) * N);
    pre = (int*) malloc(sizeof(int) * N);
    for(int k = 0; k < 2 * N; k++){
        scanf("%s",&s);
        if (s[1] == 'u') {
            scanf("%d", &val);
            pre[i++] = val;
            Push(S, val);
        }
        else{
            in[j++] = pop(S);

        }
    }
    Post(0,0,N - 1);
    return 0;
}
