#include <iostream>
#define MinData -1
#define N 64
typedef struct HuTreeNode *HuffmanTree;
typedef struct HNode *Heap;
typedef struct BiTreeNode *BinTree;
struct HuTreeNode {
    int Weight;
    HuffmanTree Left, Right;
};
struct HNode {
    HuffmanTree* Data;
    int Size;
};
struct BiTreeNode {
    int flag;
    BinTree Left, Right;
};

HuffmanTree BuiltHuffman(Heap H);
HuffmanTree DeleteMinHeap(Heap H);
void InsertMinHeap(Heap H, HuffmanTree HT);
int Wpl(HuffmanTree HT, int depth);
void CheckPrefixcode(BinTree BT, char *str, int *flag);
BinTree Delete(BinTree BT);

int main(void) {
    int i, n, m, wpl, flag, frequency[N], w;
    char ch, str[N];
    Heap H = (Heap)malloc(sizeof(struct HNode));
    HuffmanTree HT;
    BinTree BT;

    scanf("%d\n", &n);
    H->Data = (HuffmanTree*)malloc((n + 1)*sizeof(HuffmanTree));
    H->Data[0] = (HuffmanTree)malloc(sizeof(struct HuTreeNode));
    H->Data[0]->Weight = MinData;
    H->Size = 0;
    for (i = 1;i <= n;i++) {
        ch = getchar();
        if(!isalpha(ch))
            ch = getchar();
        scanf(" %d", &frequency[i]);
        HT = (HuffmanTree)malloc(sizeof(struct HuTreeNode));
        HT->Weight = frequency[i];
        HT->Left = HT->Right = NULL;
        InsertMinHeap(H, HT);
    }
    HT = BuiltHuffman(H);
    w = Wpl(HT, 0);

    ch = getchar();
    scanf("%d\n", &m);
    while (m--) {
        wpl = 0;
        flag = 0;
        BT = (BinTree)malloc(sizeof(struct BiTreeNode));
        BT->flag = 0;
        BT->Left = BT->Right = NULL;
        for (i = 0;i < n;i++) {
            scanf("%c %s\n", &ch, str);
            wpl += strlen(str) * frequency[i+1];
            if(!flag)
                CheckPrefixcode(BT, str, &flag);
        }
        if (flag || wpl > w)
            printf("No\n");
        else printf("Yes\n");
        BT = Delete(BT);
    }
}

//建立Huffman树
HuffmanTree BuiltHuffman(Heap H) {
    int i, k = H->Size - 1;
    HuffmanTree HT;
    for (i = 0;i < k;i++) {
        HT = (HuffmanTree)malloc(sizeof(struct HuTreeNode));
        HT->Left = DeleteMinHeap(H);
        HT->Right = DeleteMinHeap(H);
        HT->Weight = HT->Left->Weight + HT->Right->Weight;
        InsertMinHeap(H, HT);
    }
    return DeleteMinHeap(H);
}
//从最小堆中删除元素
HuffmanTree DeleteMinHeap(Heap H) {
    int i, child;
    HuffmanTree MinItem, LastItem;
    MinItem = H->Data[1];
    LastItem = H->Data[H->Size--];
    for (i = 1;i * 2 <= H->Size;i = child) {
        child = i * 2;
        if (child < H->Size && H->Data[child + 1]->Weight < H->Data[child]->Weight)
            child++;
        if(LastItem->Weight > H->Data[child]->Weight)
            H->Data[i] = H->Data[child];
        else break;
    }
    H->Data[i] = LastItem;
    return MinItem;
}
//向最小堆中插入元素
void InsertMinHeap(Heap H, HuffmanTree HT) {
    int i;
    for (i = ++H->Size; H->Data[i / 2]->Weight > HT->Weight; i /= 2)
        H->Data[i] = H->Data[i / 2];
    H->Data[i] = HT;
}
//计算最优编码长度
int Wpl(HuffmanTree HT, int depth) {
    if (!HT->Left && !HT->Right)
        return depth*HT->Weight;
    else
        return Wpl(HT->Left, depth + 1) + Wpl(HT->Right, depth + 1);
}
//检查是否为前缀码
void CheckPrefixcode(BinTree BT, char *str, int *flag) {
    size_t i;
    for (i = 0;i < strlen(str);i++) {
        if (BT->flag) //非叶子节点存在元素
        {
            *flag = 1;
            return;
        }
        if (str[i] == '0') {
            if (!BT->Left) {
                BinTree T = (BinTree)malloc(sizeof(struct BiTreeNode));
                T->flag = 0;
                T->Left = T->Right = NULL;
                BT->Left = T;
            }
            BT = BT->Left;
        }
        else {
            if (!BT->Right) {
                BinTree T = (BinTree)malloc(sizeof(struct BiTreeNode));
                T->flag = 0;
                T->Left = T->Right = NULL;
                BT->Right = T;
            }
            BT = BT->Right;
        }
    }
    //该节点将写入元素，如果已经存在元素（重合）或者非叶节点（存在子树），则不是前缀码
    if (BT->flag || BT->Left || BT->Right) {
        *flag = 1;
        return;
    }
    BT->flag = 1;
}
//删除树
BinTree Delete(BinTree BT) {
    if (!BT->Left && !BT->Right) {
        free(BT);
        BT = NULL;
    }
    else {
        if (BT->Left)
            BT->Left = Delete(BT->Left);
        if (BT->Right)
            BT->Right = Delete(BT->Right);
    }
    return BT;
}

