#include <iostream>
#define MaxVertexNum 10
typedef int Vertex;
typedef int WeightType;
typedef struct ENode* PtrToENode;
struct ENode{
    Vertex v1, v2;
};
typedef PtrToENode Edge;
typedef struct GNode* PtrToGNode;
struct GNode{
    int Nv;
    int Ne;
    WeightType G[MaxVertexNum][MaxVertexNum];
};
typedef PtrToGNode MGraph;
MGraph CreateGraph(int VertexNum){
    Vertex v, w;
    MGraph Graph;
    Graph = (MGraph) malloc(sizeof(struct GNode));
    Graph -> Nv = VertexNum;
    Graph -> Ne = 0;
    for (v = 0; v < Graph -> Nv; v++)
        for (w = 0; w < Graph -> Nv; w++)
            Graph -> G[v][w] = 0;
    return Graph;
}
void InsertEdge(MGraph Graph, Edge E){
    Graph -> G[E -> v1][E -> v2] = 1;
    Graph -> G[E -> v2][E -> v1] = 1;
}
MGraph BuildGraph(){
    MGraph Graph;
    Edge E;
    int Nv,i;
    scanf("%d", &Nv);
    Graph = CreateGraph(Nv);
    scanf("%d",&Graph -> Ne);
    if (Graph -> Ne != 0){
        E = (Edge) malloc(sizeof(struct ENode));
        for (i = 0; i < Graph -> Ne; i++){
            scanf("%d %d", &E -> v1,&E -> v2 );
            InsertEdge(Graph,E);
        }

    }
    return Graph;
}
void DFS(MGraph Graph,Vertex v, int visit[]){
    visit[v] = 1;
    int i;
    printf("%d ",v);
    for (i = 0; i < Graph -> Nv; i++){
        if (visit[i] == 0 && Graph -> G[v][i] == 1)
            DFS(Graph, i, visit);
    }
}
void BFS(MGraph Graph, Vertex v, int visit[]){
    visit[v] = 1;
    int queue[MaxVertexNum];
    int first, rear,i;
    Vertex w;
    first = -1;
    rear = -1;
    queue[++rear] = v;
    while (rear > first){
        w = queue[++first];
        printf("%d ",w);
        for (i = 0;i < Graph -> Nv; i++){
            if (visit[i] == 0 && Graph -> G[w][i] == 1){
                visit[i] = 1;
                queue[++rear] = i;
            }
        }
    }


}
int main() {
    MGraph Graph;
    int visit[MaxVertexNum] = {0};
    Graph = BuildGraph();
    int i;
    for (i = 0; i < Graph -> Nv; i++){
        if (visit[i] == 0){
            printf("{ ");
            DFS(Graph,i,visit);
            printf("}\n");
        }
    }
    for (i = 0; i < MaxVertexNum;i++)
        visit[i] = 0;
    for (i = 0; i < Graph -> Nv; i++){
        if (visit[i] == 0){
            printf("{ ");
            BFS(Graph,i,visit);
            printf("}\n");
        }
    }
}
