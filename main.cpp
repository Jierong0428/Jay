#include <iostream>
#define Maxsize 10001
typedef int ElementType;
typedef int SetName;
typedef int SetType[Maxsize];

void Initialization(SetType S,ElementType n){
    for (int i = 0; i < n; i++)
        S[i] = -1;
}

SetName Find(SetType S,ElementType X){
    if (S[X] < 0)
        return X;
    return S[X] = Find(S,S[X]);
}

void Union(SetType S,ElementType X1, ElementType X2){
    int Root1, Root2;
    Root1 = Find(S,X1);
    Root2 = Find(S,X2);
    if (S[Root1] < S[Root2])
        S[Root2] = Root1;
    else {
        if (S[Root1] == S[Root2])
            S[Root1]--;
        S[Root1] = Root2;

    }
}

void Input_Connection(SetType S){
    ElementType u,v;
    SetName Root1, Root2;
    scanf("%d %d\n",&u,&v);
    Root1 = Find(S,u - 1);
    Root2 = Find(S, v - 1);
    if (Root1 != Root2)
        Union(S, Root1, Root2);
}

void Check_Connection(SetType S){
    ElementType u,v;
    SetName Root1, Root2;
    scanf("%d %d\n",&u,&v);
    Root1 = Find(S,u - 1);
    Root2 = Find(S, v - 1);
    if (Root1 == Root2)
        printf("yes\n");
    else
        printf("no\n");
}

void Check_Network(SetType S, ElementType n){
    int cnt = 0;
    int i;
    for (i = 0; i < n; i++){
        if (S[i] < 0)
            cnt += 1;
    }
    if (cnt == 1)
        printf("The network is connected.\n");
    else
        printf("There are %d components.\n",cnt);
}
int main() {
    SetType S;
    int n;
    char in;
    scanf("%d",&n);
    Initialization(S,n);
    do{
        scanf("%c",&in);
        switch (in) {
            case 'I': Input_Connection(S);break;
            case 'C': Check_Connection(S);break;
            case 'S': Check_Network(S,n);break;

        }
    }
    while (in != 'S');
    return 0;
}
