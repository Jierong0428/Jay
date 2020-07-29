#include <iostream>
#define MaxVertexNum 1001
typedef int ElementType;
typedef int Position;
typedef int Vertex;
typedef int WeightType;
typedef struct ENode* PtrToENode;
struct ENode{
    Vertex v1,v2;
};
typedef PtrToENode Edge;
typedef struct GNode* PtrToGNode;
struct GNode{
    int Nv, Ne;
    WeightType G[MaxVertexNum][MaxVertexNum];
};
typedef PtrToGNode MGraph;
MGraph CreateGraph(int VertexNum){
    Vertex v, w;
    MGraph Graph;
    Graph = (MGraph) malloc(sizeof(struct  GNode));
    Graph -> Ne = 0;
    Graph -> Nv = VertexNum;
    for (v = 0; v < Graph -> Nv; v++)
        for (w = 0; w < Graph -> Nv; w++)
            Graph -> G[v][w] = 0;
    return Graph;
}
void InsertEdge(MGraph Graph, Edge E){
    Graph -> G[E -> v1][E -> v2] = 1;
    Graph -> G[E -> v2][E -> v1] = 1;
}
MGraph  BuildGraph() {
    MGraph Graph;
    Edge E;
    int Nv, i;
    scanf("%d", &Nv);
    Graph = CreateGraph(Nv);
    scanf("%d", &(Graph->Ne));
    if (Graph->Ne != 0) {
        E = (Edge) malloc(sizeof(struct ENode));
        for (i = 0; i < Graph->Ne; i++) {
            scanf("%d %d", &E->v1, &E->v2);
            E->v1--;
            E->v2--;
            InsertEdge(Graph, E);
        }
    }
    return Graph;
}
struct QNode{
    ElementType* Data;
    Position Front, Rear;
    int MaxSize;
};
typedef QNode* Queue;
int Visited[MaxVertexNum] = {false};
Queue CreateQueue(int Maxsize){
    Queue Q = (Queue) malloc(sizeof(struct QNode));
    Q -> Data = (ElementType*) malloc(Maxsize * sizeof(ElementType));
    Q -> Front = Q -> Rear = 0;
    Q -> MaxSize = Maxsize;
    return Q;
}
bool IsEmpty(Queue Q){
    return (Q -> Front == Q -> Rear);
}
void Enqueue(ElementType X, Queue Q){
    Q -> Rear = (Q -> Rear + 1) % (Q -> MaxSize);
    Q -> Data[Q -> Rear] = X;
}
ElementType Dequeue(Queue Q){
    Q -> Front = (Q -> Front + 1) % (Q -> MaxSize);
    return Q -> Data[Q -> Front];
}
int BFS(Vertex V,MGraph Graph){
    int count = 1;
    int level = 0;
    Vertex W, last, tail;
    Visited[V] = true;
    Queue Q;
    Q = CreateQueue(Graph -> Nv);
    Enqueue(V,Q);
    last = V;
    while (!IsEmpty(Q)){
        V = Dequeue(Q);
        for (W = 0; W < Graph -> Nv;W++)
            if ((Graph -> G[V][W] == 1) && (!Visited[W])){
                Visited[W] =  true;
                Enqueue(W,Q);
                tail = W;
                count++;
            }
        if (V == last){
            level++;
            last = tail;
        }
        if (level == 6)
            break;

    }
    return count;
}
void resetVisit(int n){
    int i;
    for (i = 0;i < n;i++)
        Visited[i] = false;
}
int main() {
    MGraph Graph = BuildGraph();
    int count;
    double percent;
    Vertex V;
    for (V = 0; V < Graph -> Nv;V++){
        count = BFS(V,Graph);
        percent = double(count) / Graph -> Nv;
        printf("%d: %.2lf%%\n",V + 1, percent * 100);
        resetVisit(Graph -> Nv);
    }
    return 0;
}
