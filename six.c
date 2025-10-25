#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#define VM_PAGE 7      /*假设每个页面可以存放10条指令,则共有32个虚页*/
#define PM_PAGE 4         /*分配给作业的内存块数为4*/
#define TOTAL_INSERT 18
typedef struct {
    int vmn;
    int pmn;
    int exist;
    int time;
} vpage_item;

vpage_item page_table[VM_PAGE];
vpage_item* ppage_bitmap[PM_PAGE];
int vpage_arr[TOTAL_INSERT] = {1,2,3,4,2,6,2,1,2,3,7,6,3,2,1,2,3,6};

void init_data() {
    for (int i = 0; i < VM_PAGE; i++) {
        page_table[i].vmn = i + 1;
        page_table[i].pmn = -1;
        page_table[i].exist = 0;
        page_table[i].time = -1;
    }
    for (int i = 0; i < PM_PAGE; i++) {
        ppage_bitmap[i] = NULL;
    }
}

void FIFO() {
    int k = 0;
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    while (sum < TOTAL_INSERT) {
        if (page_table[vpage_arr[sum] - 1].exist == 0) {
            missing_page_count++;
            if (k < PM_PAGE) {
                ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[k]->exist = 1;
                ppage_bitmap[k]->pmn = k;
                ppage_bitmap[k]->time = current_time;
                k++;
            } else {
                int oldest_time = ppage_bitmap[0]->time;
                int replace_index = 0;
                for (int i = 1; i < PM_PAGE; i++) {
                    if (ppage_bitmap[i]->time < oldest_time) {
                        oldest_time = ppage_bitmap[i]->time;
                        replace_index = i;
                    }
                }
                ppage_bitmap[replace_index]->exist = 0;
                ppage_bitmap[replace_index] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[replace_index]->exist = 1;
                ppage_bitmap[replace_index]->pmn = replace_index;
                ppage_bitmap[replace_index]->time = current_time;
            }
        }
        current_time++;
        sum++;
    }
    printf("FIFO算法缺页次数为:%d\t缺页率为:%f\t置换次数为:%d\t置换率为:%f\n", 
           missing_page_count, missing_page_count / (float)TOTAL_INSERT, 
           missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

void LRU() {
    int k = 0;
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    while (sum < TOTAL_INSERT) {
        if (page_table[vpage_arr[sum] - 1].exist == 0) {
            missing_page_count++;
            if (k < PM_PAGE) {
                ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[k]->exist = 1;
                ppage_bitmap[k]->pmn = k;
                ppage_bitmap[k]->time = current_time;
                k++;
            } else {
                int lru_time = ppage_bitmap[0]->time;
                int replace_index = 0;
                for (int i = 1; i < PM_PAGE; i++) {
                    if (ppage_bitmap[i]->time < lru_time) {
                        lru_time = ppage_bitmap[i]->time;
                        replace_index = i;
                    }
                }
                ppage_bitmap[replace_index]->exist = 0;
                ppage_bitmap[replace_index] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[replace_index]->exist = 1;
                ppage_bitmap[replace_index]->pmn = replace_index;
                ppage_bitmap[replace_index]->time = current_time;
            }
        } else {
            // 更新最近使用时间
            for (int i = 0; i < PM_PAGE; i++) {
                if (ppage_bitmap[i]->vmn == vpage_arr[sum]) {
                    ppage_bitmap[i]->time = current_time;
                    break;
                }
            }
        }
        current_time++;
        sum++;
    }
    printf("LRU算法缺页次数为:%d\t缺页率为:%f\t置换次数为:%d\t置换率为:%f\n", 
           missing_page_count, missing_page_count / (float)TOTAL_INSERT, 
           missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

void OPT() {
    int k = 0;
    int sum = 0;
    int missing_page_count = 0;
    while (sum < TOTAL_INSERT) {
        if (page_table[vpage_arr[sum] - 1].exist == 0) {
            missing_page_count++;
            if (k < PM_PAGE) {
                ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[k]->exist = 1;
                ppage_bitmap[k]->pmn = k;
                k++;
            } else {
                int replace_index = 0;
                int farthest = -1;
                for (int i = 0; i < PM_PAGE; i++) {
                    int next_use = TOTAL_INSERT;
                    for (int j = sum + 1; j < TOTAL_INSERT; j++) {
                        if (ppage_bitmap[i]->vmn == vpage_arr[j]) {
                            next_use = j;
                            break;
                        }
                    }
                    if (next_use > farthest) {
                        farthest = next_use;
                        replace_index = i;
                    }
                }
                ppage_bitmap[replace_index]->exist = 0;
                ppage_bitmap[replace_index] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[replace_index]->exist = 1;
                ppage_bitmap[replace_index]->pmn = replace_index;
            }
        }
        sum++;
    }
    printf("OPT算法缺页次数为:%d\t缺页率为:%f\t置换次数为:%d\t置换率为:%f\n", 
           missing_page_count, missing_page_count / (float)TOTAL_INSERT, 
           missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

int main() {
    int a;
    printf("请输入需要选择的页面置换算法：1.FIFO\t2.LRU\t3.OPT\t输入0结束\n");
    do {
        scanf("%d", &a);
        switch (a) {
            case 1:
                init_data();
                FIFO();
                break;
            case 2:
                init_data();
                LRU();
                break;
            case 3:
                init_data();
                OPT();
                break;
            case 0:
                printf("程序结束\n");
                break;
            default:
                printf("无效输入，请重新选择\n");
        }
    } while (a != 0);
    return 0;
}    