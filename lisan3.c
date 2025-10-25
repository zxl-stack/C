#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>  // 添加这个头文件以使用 INT_MAX

#define MAX_VERTICES 100
#define INF INT_MAX  // 使用 INT_MAX 表示无穷大

// 边结构体定义
struct Edge {
    int src, dest, weight;
};

// 图结构体定义
struct Graph {
    int V;
    int **adjMatrix;
};

// 并查集结构体定义
struct subset {
    int parent;
    int rank;
};

// 创建图并初始化邻接矩阵
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    
    graph->adjMatrix = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        graph->adjMatrix[i] = (int*)malloc(V * sizeof(int));
    }
    
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph->adjMatrix[i][j] = INF;
        }
    }
    
    return graph;
}

// 并查集查找操作（带路径压缩）
int find(struct subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// 并查集合并操作（按秩合并）
void Union(struct subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// 边比较函数，用于排序
int compareEdges(const void* a, const void* b) {
    struct Edge* edge1 = (struct Edge*)a;
    struct Edge* edge2 = (struct Edge*)b;
    return edge1->weight - edge2->weight;
}

// Kruskal算法实现最小生成树
void KruskalMST(struct Graph* graph) {
    int V = graph->V;
    struct Edge* resultMST=(struct Edge*)malloc(sizeof(struct Edge)*V);
    int e = 0;
    int i, j;
    
    // 提取所有边
    struct Edge* edges = (struct Edge*)malloc(V * V * sizeof(struct Edge));
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (graph->adjMatrix[i][j] != INF) {
                edges[e].src = i;
                edges[e].dest = j;
                edges[e].weight = graph->adjMatrix[i][j];
                e++;
            }
        }
    }
    
    // 按权重排序边
    qsort(edges, e, sizeof(edges[0]), compareEdges);
    
    // 初始化并查集
    struct subset* subsets = (struct subset*)malloc(V * sizeof(struct subset));
    for (int v = 0; v < V; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    
    // 构建最小生成树
    e = 0;
    i = 0;
    while (e < V - 1 && i < V * V) {
        struct Edge next_edge = edges[i++];
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);
        
        if (x != y) {
            resultMST[e++] = next_edge;
            Union(subsets, x, y);
        }
    }
    
    // 输出最小生成树
    printf("最小生成树:\n");
    for (i = 0; i < e; ++i)
        printf("%d - %d : %d\n", resultMST[i].src, resultMST[i].dest, resultMST[i].weight);
    
    // 释放内存
    free(edges);
    free(subsets);
}

// 主函数
int main() {
    int V;
    
    printf("输入顶点数: ");
    scanf("%d", &V);
    
    struct Graph* graph = createGraph(V);
    
    printf("输入邻接矩阵（使用%d表示无穷大/无边）:\n", INF);
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            scanf("%d", &graph->adjMatrix[i][j]);
        }
    }
    
    KruskalMST(graph);
    
    // 释放图内存
    for (int i = 0; i < V; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
    
    return 0;
}