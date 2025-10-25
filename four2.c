#include <stdio.h>  
#define MAX_ITEMS 100  
int n;               
int W;               
int weights[MAX_ITEMS]; 
int values[MAX_ITEMS];  
int max_value = 0;    
int selected_items[MAX_ITEMS]; // 临时数组，记录当前递归路径选中的物品（0未选，1选中）
int best_selection[MAX_ITEMS]; // 最终结果数组，保存最优解的物品选择状态


void backtrack(int k, int current_value, int current_weight) {
    if (k == n) {
        if (current_value > max_value) {
            max_value = current_value; // 更新最大价值
            for (int i = 0; i < n; i++) {
                best_selection[i] = selected_items[i]; 
            }
        }
        return; // 回溯返回上一层
    }

    selected_items[k] = 0; 
    backtrack(k + 1, current_value, current_weight);


    if (current_weight + weights[k] <= W) {
        selected_items[k] = 1; 
    
        backtrack(k + 1, current_value + values[k], current_weight + weights[k]);
    }
    return;// 回溯返回上一层
}


int main() {
    printf("请输入物品数量: ");
    scanf("%d", &n); 
    printf("请输入背包容量: ");
    scanf("%d", &W); 
    printf("请输入%d个物品的重量: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &weights[i]); 
    }
    printf("请输入%d个物品的价值: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &values[i]); 
    }

    // 启动回溯搜索：从第0个物品开始，初始总价值和重量均为0
    backtrack(0, 0, 0);
    printf("最大价值: %d\n", max_value); // 打印最优解的总价值
    printf("选择的物品(0=未选, 1=已选): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_selection[i]); 
    }
    printf("\n"); 

    return 0; 
}