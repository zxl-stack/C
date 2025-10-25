#include <stdio.h>
#include <stdlib.h>

int MAX(int a, int b) {
    return (a >= b) ? a : b;
}

// 声明 BagSack 函数
int BagSack(int w[], int v[], int *x, int n, int C);

int main() {
    int n, C;
    
    printf("请输入物品数量: ");
    scanf("%d", &n);
    
    int *w = (int *)malloc(n* sizeof(int));
    int *v = (int *)malloc(n * sizeof(int));
    int *x = (int *)malloc(n * sizeof(int));
    
    printf("请输入每个物品的重量:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &w[i]);
    }
    
    printf("请输入每个物品的价值:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
    
    printf("请输入背包容量: ");
    scanf("%d", &C);
    
    int max_value = BagSack(w, v, x, n, C);
    printf("最大价值是：%d\n", max_value);
    
    // 输出每个物品是否被选择的情况
    printf("选择的物品(1表示选择,0表示未选择):\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
    
    return 0;
}

// 实现 0-1 背包问题的动态规划算法
int BagSack(int w[], int v[], int x[], int n, int C) {
    // 动态分配动态规划表
    int **V = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        V[i] = (int *)malloc((C + 1) * sizeof(int));
    }
    
    // 初始化动态规划表
    for (int i = 0; i <= n; i++) {
        V[i][0] = 0; // 当背包容量为 0 时，价值为 0
    }
    for (int j = 0; j <= C; j++) {
        V[0][j] = 0; // 当没有物品时，价值为 0
    }
    
    // 填充动态规划表
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= C; j++) {
            if (j < w[i-1]) { 
                V[i][j] = V[i - 1][j];
            } else {
                V[i][j] = MAX(V[i - 1][j], V[i - 1][j - w[i-1]] + v[i-1]);
            }
        }
    }
    
    // 回溯找出选择的物品
    int j = C;
    for (int i = n; i > 0; i--) {
        if (V[i][j] != V[i - 1][j]) {
            x[i-1] = 1; // 数组从0开始)
            j = j - w[i-1];
        } else {
            x[i-1] = 0; // 不选择物品 i-1
        }
    }
    
    // 返回背包所装物品的最大价值
    return V[n][C];
}