#include <iostream>
#include<cmath>
#define MAXN 1000
#define Bank 50
#define ISLAND_RADIUS 15
typedef struct Point{
    int x;
    int y;
} Position;
Position P[MAXN];
double d;
bool Visited[MAXN] = {false};
int n;
bool FirstJump(int v){
    return (sqrt(pow(P[v].x,2) + pow(P[v].y,2)) <= ISLAND_RADIUS + d);
}

bool Jump(int v1, int v2){
    return (sqrt(pow(P[v1].x - P[v2].x,2) + pow(P[v1].y - P[v2].y,2) ) <= d);
}

bool IsSafe(int v){
    return ((abs(P[v].x) >= Bank - d) || (abs(P[v].y) >= Bank - d));
}

bool DFS(int v){
    bool answer = false;
    int i;
    Visited[v] = true;
    if (IsSafe(v))
        answer = true;
    else{
        for (i = 0; i < n; i++){
            if (!Visited[i] && Jump(v,i)){
                answer = DFS(i);
            }
            if (answer)
                break;
        }
    }
    return answer;
}

void Save007(){
    bool answer = false;
    int i;
    for (i = 0; i < n; i++){
        if (!Visited[i] && FirstJump(i)){
            answer = DFS(i);
            if (answer) break;
        }
    }
    if (answer)
        printf("Yes\n");
    else
        printf("No\n");
}
int main() {
    scanf("%d %lf",&n, &d);
    int i;
    for (i = 0; i < n; i++){
        scanf("%d %d", &(P[i].x), &(P[i].y));
    }
    Save007();
    return 0;
}
