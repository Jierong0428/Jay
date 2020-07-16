#include <iostream>
typedef struct TNode* Tree;
struct TNode{
    int data;
    Tree Left;
    Tree Right;
};
int Height(Tree T){
    if (T){
        int HL = Height(T -> Left);
        int HR = Height(T -> Right);
        int MAX = HL > HR ? HL : HR;
        return (MAX + 1);
    }
    return 0;
}
Tree LLFix(Tree T){
    Tree root = T -> Left;
    T -> Left = root -> Right;
    root -> Right = T;
    return root;
}
Tree LRFix(Tree T){
    Tree root = T -> Left -> Right;
    T -> Left -> Right = root -> Left;
    root -> Left = T -> Left;
    T -> Left = root -> Right;
    root -> Right = T;
    return root;
}
Tree RLFix(Tree T){
    Tree root = T -> Right -> Left;
    T -> Right -> Left = root -> Right;
    root -> Right = T -> Right;
    T -> Right = root -> Left;
    root -> Left = T;
    return root;
}
Tree RRFix(Tree T){
    Tree root = T -> Right;
    T -> Right = root -> Left;
    root -> Left = T;
    return root;
}
Tree Insert(Tree T,int num){
    if (!T){
         T = (Tree) malloc(sizeof(struct TNode));
         T -> data = num;
         T -> Left = T -> Right = nullptr;
    }
    else{
        if (num > T -> data){
            T -> Right = Insert(T -> Right, num);
            if (Height(T -> Right) - Height(T -> Left) == 2){
                if (num > T -> Right -> data)
                    T = RRFix(T);
                else T = RLFix(T);
            }
        }
       else if (num < T -> data){
           T -> Left = Insert(T -> Left, num);
           if (Height(T -> Left) - Height(T -> Right) == 2){
               if (num < T -> Left -> data)
                   T = LLFix(T);
               else T = LRFix(T);
           }
       }
    }
    return  T;
}
int main() {
    int n, i;
    int num;
    Tree T = nullptr;
    scanf("%d",&n);
    for (i = 0; i < n; i++){
       scanf("%d",&num);
       T = Insert(T,num);
    }
    if (T)
        printf("%d",T -> data);
    return 0;
}
