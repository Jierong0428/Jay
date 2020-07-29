#include <iostream>
#define MaxVertexNum 100
#define INFINITY 65535
typedef int Vertex;
typedef int WeightType;
typedef struct ENode* PtrToENode;
struct ENode{
    Vertex v1,v2;
    WeightType weight;
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
            Graph -> G[v][w] = INFINITY;
    return Graph;
}
void InsertEdge(MGraph Graph, Edge E){
    Graph -> G[E -> v1][E -> v2] = E -> weight;
    Graph -> G[E -> v2][E -> v1] = E -> weight;
}
MGraph  BuildGraph(){
    MGraph Graph;
    Edge  E;
    int Nv, i;
    scanf("%d", &Nv);
    Graph = CreateGraph(Nv);
    scanf("%d",&(Graph -> Ne));
    if (Graph -> Ne != 0){
        E = (Edge) malloc(sizeof(struct ENode));
        for (i = 0; i < Graph -> Ne;i++){
            scanf("%d %d %d",&E -> v1, &E -> v2, &E -> weight);
            E -> v1--;
            E -> v2--;
            InsertEdge(Graph,E);
        }
    }
    return Graph;
}
void Floyd(MGraph Graph, WeightType D[][MaxVertexNum]){
    Vertex i,j,k;
    for (i = 0; i < Graph -> Nv;i++)
        for (j = 0; j < Graph -> Nv;j++)
            D[i][j] = Graph -> G[i][j];
    for (k = 0; k < Graph -> Nv; k++)
        for (i = 0; i < Graph -> Nv; i++)
            for (j = 0; j < Graph -> Nv; j++)
                if (D[i][k] + D[k][j] < D[i][j])
                    D[i][j] = D[i][k] + D[k][j];

}
WeightType FindMaxDist(WeightType D[][MaxVertexNum],Vertex i, int N){
    WeightType MaxDist;
    Vertex j;
    MaxDist = 0;
    for (j = 0; j < N;j++)
        if (i != j && D[i][j] > MaxDist)
            MaxDist = D[i][j];
    return MaxDist;
}
void FindAnimal(MGraph Graph){
    WeightType D[MaxVertexNum][MaxVertexNum], MinDist, MaxDist;
    Vertex i, Animal;
    Floyd(Graph,D);
    MinDist = INFINITY;
    for (i = 0; i < Graph -> Nv; i++){
        MaxDist = FindMaxDist(D,i,Graph -> Nv);
        if (MaxDist == INFINITY) {
            printf("0\n");
            return;
        }
        if (MaxDist < MinDist){
            MinDist = MaxDist;
            Animal = i + 1;
        }
    }
    printf("%d %d",Animal, MinDist);
}
int main() {
    MGraph Graph = BuildGraph();
    FindAnimal(Graph);
    return 0;
}
