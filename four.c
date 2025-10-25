#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_CITIES 20

int costMatrix[MAX_CITIES][MAX_CITIES]; 
int path[MAX_CITIES];                  // 存储当前路径
bool visited[MAX_CITIES];              
int n;                                
int min_cost = INT_MAX;               
int best_path[MAX_CITIES];             // 最优路径

// 打印最优路径
void printSolution() {
    printf("最优路径: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_path[i]); // 城市编号从0开始显示
    }
    printf("%d\n", best_path[0]); // 回到起点形成回路
    printf("最小代价: %d\n", min_cost);
}

// 检查城市v是否可以加入当前路径
bool isSafe(int v, int pos, int current_cost) {
    if (visited[v]) {
        return false;
    }
    
    // 检查前一个城市到当前城市是否有路径
    if (costMatrix[path[pos-1]][v] == INT_MAX) {
        return false;
    }
    
   
    if (current_cost + costMatrix[path[pos-1]][v] >= min_cost) {
        return false;
    }
    
    return true;
}

// 回溯法求解TSP问题
void tspBacktrack(int pos, int current_cost) {
    if (pos == n) {
        // 检查是否能回到起点
        if(costMatrix[path[pos-1]][path[0]]!=INT_MAX){
            int total_cost = current_cost + costMatrix[path[pos-1]][path[0]];
            if (total_cost < min_cost) {
                min_cost = total_cost;
                for (int i = 0; i < n; i++) {
                    best_path[i] = path[i];
                }
            
        }
    }
        return;
    }
    
    // 尝试不同的城市作为路径中的下一个城市
    for (int v = 0; v < n; v++) {
        if (isSafe(v, pos, current_cost)) {
            path[pos] = v;
            visited[v] = true;
            
            // 递归构造剩下的路径
            tspBacktrack(pos + 1, current_cost + costMatrix[path[pos-1]][v]);
            
            // 回溯
            visited[v] = false;
            path[pos] = -1;
        }
    }
    return;

}

int main() {
    printf("请输入城市数量: ");
    scanf("%d", &n);
    printf("请输入代价矩阵(%d×%d):\n", n, n);
    printf("(注意: 输入0表示无路径，将被视为∞)\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &costMatrix[i][j]);
            if (i==j) {
                costMatrix[i][j] = INT_MAX; 
            }
        }
    }
    
    // 初始化
    for (int i = 0; i < n; i++) {
        path[i] = -1;
        visited[i] = false;
    }
    
    // 从城市0开始构建路径
    path[0] = 0;
    visited[0] = true;
    
    printf("\n正在计算最优路径...\n");
    tspBacktrack(1, 0);
    
    if (min_cost == INT_MAX) {
        printf("该图不存在可行的TSP路径。\n");
    } else {
        printSolution();
    }
    
    return 0;
}