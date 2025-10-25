#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

// 定义边的结构体，包含两个顶点u、v和边的代价cost
typedef struct {
    int u;
    int v;
    int cost;
} Edge;

// 并查集结构体，用于检查连通性和防止回路
typedef struct {
    int *parent;  
    int *rank;   
} UnionFind;

// 初始化二维数组
int** Init_arr(int width, int height) {
    int **arr = (int **)malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++) {
        arr[i] = (int *)malloc(height * sizeof(int));
    }
    return arr;
}

// 创建并初始化并查集
UnionFind uf_create(int size) {
    UnionFind uf;
    uf.parent = (int *)malloc((size + 1) * sizeof(int));  
    uf.rank = (int *)malloc((size + 1) * sizeof(int));   
    for (int i = 1; i <= size; i++) {
        uf.parent[i] = i;  
        uf.rank[i] = 1;    
    }
    return uf;
}

// 查找操作，带路径压缩优化
int uf_find(UnionFind *uf, int x) {
    if (uf->parent[x] == x) 
        return x;
    else
        return uf->parent[x] = uf_find(uf, uf->parent[x]);  // 在查找根节点的同时，把查找路径上的点都指向根 
}

// 合并操作
void uf_union(UnionFind *uf, int x, int y) {
    int xRoot = uf_find(uf, x);
    int yRoot = uf_find(uf, y);
    if (xRoot == yRoot) return;  
    if (uf->rank[xRoot] < uf->rank[yRoot]) {
        uf->parent[xRoot] = yRoot;  
    } else {
        uf->parent[yRoot] = xRoot;
        if (uf->rank[xRoot] == uf->rank[yRoot]) {
            uf->rank[xRoot]++;  
        }
    }
}

// 构造路径函数，从起点出发遍历邻接表生成路径
void construct_path(int n, int **adj, int start, int *path) {
    int *visited = (int *)malloc((n + 1) * sizeof(int));  // 记录已访问的顶点
    for (int i = 1; i <= n; i++) visited[i] = 0;
    int current = start;
    int prev = -1;
    path[0] = current;  // 起点加入路径
    visited[current] = 1;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < 2; j++) {
            int next = adj[current][j];  // 遍历邻接顶点
            if (next != -1 &&!visited[next]) {
                prev = current;
                current = next;
                path[i] = current;  // 将未访问的邻接顶点加入路径
                visited[current] = 1;
                break;
            }
        }
    }
}

// 实现Partition函数用于快速排序
int Partition(Edge arr[], int low, int high) {
    Edge pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].cost < pivot.cost) {
            i++;
            Edge temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Edge temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// 实现QuickSort函数用于对Edge数组排序
void QuickSort(Edge arr[], int low, int high) {
    if (low < high) {
        int pi = Partition(arr, low, high);
        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
    }
}

int main() {
    int n;
    printf("请输入城市数量 n: ");
    scanf("%d", &n); 
    printf("请输入 %d x %d 的代价矩阵（用 -1 表示不可达）:\n", n, n);
    // 动态分配代价矩阵
    int **cost = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        cost[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);  
            if (i == j) cost[i][j] = INT_MAX; 
        }
    }

    // 提取所有边并存储
    Edge *edges = (Edge *)malloc(n * n * sizeof(Edge));
    int edgeCount = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
                edges[edgeCount].u = i + 1;  // 顶点编号从1开始
                edges[edgeCount].v = j + 1;
                edges[edgeCount].cost = cost[i][j];
                edgeCount++;
        }
    }
    QuickSort(edges, 0, edgeCount - 1);
    // 初始化并查集、度数组和邻接表
    UnionFind uf = uf_create(n);
    int *degree = (int *)calloc(n + 1, sizeof(int));
    int **adj = Init_arr(n + 1, 2);  
    for (int i = 1; i <= n; i++) {
        adj[i][0] = adj[i][1] = -1;  
    }
    int tspLength = 0;  // 记录总代价
    int edgesSelected = 0;  // 记录已选边数

    // 贪心选择边，直到形成生成树
    for (int i = 0; i < edgeCount && edgesSelected < n - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int c = edges[i].cost;

        // 如果顶点的度数超过2，跳过（避免分枝）
        if (degree[u] >= 2 || degree[v] >= 2) continue;

        // 检查是否连通，避免回路
        int rootU = uf_find(&uf, u);
        int rootV = uf_find(&uf, v);
        if (rootU != rootV) {
            tspLength += c;  
            edgesSelected++;
            uf_union(&uf, u, v);  // 合并集合
            degree[u]++;  
            degree[v]++;
            // 更新邻接表
            if (adj[u][0] == -1) adj[u][0] = v;
            else adj[u][1] = v;
            if (adj[v][0] == -1) adj[v][0] = u;
            else adj[v][1] = u;
        }
    }
    // 找到生成树的两个端点（度数为1的顶点）
    int u = -1, v_end = -1;
    for (int i = 1; i <= n; i++) {
        if (degree[i] == 1) {
            if (u == -1) u = i;
            else v_end = i;
        }
    }

    // 闭合回路，累加最后一条边的代价
    tspLength += cost[u - 1][v_end - 1];
    int *path = (int *)malloc((n + 1) * sizeof(int));  
    construct_path(n, adj, u, path);  // 构造路径
    path[n] = u;  // 闭合回路

    // 输出路径和总代价
    printf("Path: ");
    for (int i = 0; i <= n; i++) {
        printf("%d", path[i]);
        if (i < n) printf(" -> ");
    }
    printf("\nTotal cost: %d\n", tspLength);
    return 0;
}