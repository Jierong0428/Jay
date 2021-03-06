#include <iostream>
typedef int ElementType;
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};

BinTree Insert( BinTree BST, ElementType X ){
    if (!BST){
        BST = (BinTree) malloc(sizeof((struct TNode)));
        BST -> Data = X;
        BST -> Left = BST -> Right = NULL;
    }
    else{
        if (X < BST -> Data)
            BST -> Left = Insert(BST -> Left, X);
        else if (X > BST -> Data)
            BST -> Right = Insert(BST -> Right, X);
    }
    return BST;
}

BinTree Delete( BinTree BST, ElementType X ){
    Position  Tmp;
    if (!BST){
        printf("Not Found /n");
    }
    else{
        if (X < BST -> Data)
            BST -> Left = Delete(BST -> Left,X);
        else if (X > BST -> Data)
            BST -> Right = Delete(BST -> Right,X);
        else{
            if (BST -> Left && BST -> Right){
                Tmp = FindMin(BST -> Right);
                BST -> Data = Tmp -> Data;
                BST -> Right = Delete(BST -> Right,BST -> Data);
            }
            else{
                Tmp = BST;
                if (!BST -> Left)
                    BST = BST -> Right;
                else
                    BST = BST -> Right;
                free(Tmp);
            }

        }
    }
    return BST;
}

Position Find( BinTree BST, ElementType X ) {
    while (BST) {
        if (X < BST->Data)
            BST = BST->Left;
        else if (X > BST->Data)
            BST = BST->Left;
        else
            return BST;
    }
    return NULL;
}

Position FindMin( BinTree BST ){
    if (BST){
    while (BST -> Left)
        BST = BST -> Left;
    }
    return BST;
}

Position FindMax( BinTree BST ){
    if (BST){
        while (BST -> Right)
            BST = BST -> Right;
    }
    return BST;
}