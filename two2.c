#include <stdio.h>      
#include <stdlib.h>     
#include <limits.h>     // 定义整数类型极限值（如INT_MAX）


// 动态分配二维数组
int** allocate_2d_array(int rows, int cols) {
    int **arr = (int **)malloc(rows * sizeof(int *));  
    for (int i = 0; i < rows; i++) {
        arr[i] = (int *)malloc(cols * sizeof(int));    
    }
     return arr;
}


// 打印最短路径及其长度
void print_path(int n, int **parent, int **arc) {
    int *path = (int *)malloc((n + 1) * sizeof(int));  // 分配路径存储空间
    int current = 0;       // 从起点0开始
    int mask = (1 << (n - 1)) - 1;  
    path[0] = current;     
    for (int i = 1; i <= n; i++) {
        int next = parent[current][mask];  // 获取下一个城市
        path[i] = next;                  
        mask ^= (1 << (next - 1));       
        current = next;                   // 移动到下一个城市
    }

    // 打印路径
    printf("\n最短路径: ");
    for (int i = 0; i <= n; i++) {
        printf("%d", path[i]);
        if (i < n) printf(" -> ");
    }
}

// 动态规划求解TSP问题
int tsp(int n, int **arc, int **dp, int **parent) {
    int subset_size = 1 << (n - 1); 

    // 初始化：从各城市直接返回起点的成本
    for (int i = 1; i < n; i++) {
        dp[i][0] = arc[i][0];   
        parent[i][0] = 0;     
    }

    // 动态规划填表
    for (int mask = 1; mask < subset_size; mask++) {  
        for (int i = 1; i < n; i++) {              
            dp[i][mask] = INT_MAX;                   // 初始化为无穷大
            if(!(mask & (1 << (i - 1)))) {       
                for (int k = 1; k < n; k++) {      
                    if (mask & (1 << (k - 1))) {     // 如果k在子集中
                        int new_mask = mask ^ (1 << (k - 1));  // 移除k的子集
                        int new_cost = arc[i][k] + dp[k][new_mask];  
                        if (new_cost < dp[i][mask]) {
                            dp[i][mask] = new_cost;  // 更新最小成本
                            parent[i][mask] = k;     
                        }
                    }
                }   
            }
        }
    }

    // 计算最终结果：从起点出发，经过所有城市后返回
    int final_mask = subset_size - 1;  
    int min_cost = INT_MAX;
    int start_k = -1;
    for (int k = 1; k < n; k++) {
        int cost = arc[0][k] + dp[k][final_mask ^ (1 << (k - 1))];  // 计算总成本
        if (cost < min_cost) {
            min_cost = cost;  // 更新最小总成本
            start_k = k;       // 记录起始城市
            parent[0][final_mask] = start_k;  // 记录路径起点
    }
}

    return min_cost;  // 返回最短路径长度
}

int main() {
    int n;
    printf("请输入城市数量 n: ");
    scanf("%d", &n);  
    int **arc = allocate_2d_array(n, n);
    printf("请输入 %d x %d 的代价矩阵（用 -1 表示不可达）:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &arc[i][j]);
            if (arc[i][j] == -1) arc[i][j] = INT_MAX;   // 将-1替换为无穷大
        }
    }
    int size = 1 << (n - 1);  // 计算子集数量
    int **dp = allocate_2d_array(n, size);//用于存储从起点出发，经过某些城市后到达某个城市时的最小路径成本
    int **parent = allocate_2d_array(n, size);//用于记录路径信息，以便在动态规划完成后能够回溯出完整的最短路径
    int shortest_path = tsp(n, arc, dp, parent);
    print_path(n, parent, arc);  
    printf("最短路径总长度: %d\n", shortest_path);
    return 0;
}