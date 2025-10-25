#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <graphics.h> // 需要安装BGI图形库

// 定义点的结构体
typedef struct {
    double x;
    double y;
} point;

// 函数声明
void RandomPoints(point P[], int n);
void QuickSort_X(point s[], int low, int high);
double Closest(point S[], point *Q, int low, int high);
void DrawPoints(point S[], int n, point Q[], double distance);

// 窗口大小和缩放因子
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SCALE_FACTOR 60.0  // 将坐标值放大以便在屏幕上更好显示

int main() {
    int size;
    printf("随机生成横纵坐标值均在1-10的点数个数:");
    scanf("%d", &size);
    
    int low = 0;
    int high = size - 1;
    point *S = malloc(sizeof(point) * size);
    point Q[2];
    
    // 生成随机点
    RandomPoints(S, size);
    QuickSort_X(S, low, high);
    
    double MAX = Closest(S, Q, low, high);
    
    printf("\n");
    printf("一对最近点对分别为:(%.2f,%.2f),(%.2f,%.2f)\n", Q[0].x, Q[0].y, Q[1].x, Q[1].y);
    printf("最短的距离为:%.2f\n", MAX);
    
    // 初始化图形模式
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    
    // 绘制点集和最近点对
    DrawPoints(S, size, Q, MAX);
    
    // 等待用户按键关闭图形窗口
    getch();
    closegraph();
    
    free(S);
    return 0;
}

// 生成随机点
void RandomPoints(point P[], int n) {
    srand(time(NULL));
    printf("生成的随机点:\n");
    for (int i = 0; i < n; i++) {
        P[i].x = (double)rand() / RAND_MAX * 9 + 1;
        P[i].y = (double)rand() / RAND_MAX * 9 + 1;
        printf("(%.2f,%.2f) ", P[i].x, P[i].y);
        if ((i + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
}

// 计算两点之间的距离
double Distance(point a, point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// 快排一趟排序(X轴)
int Partition_X(point S[], int low, int high) {
    int i = low, j = high;
    while (i < j) {
        while (i < j && S[i].x <= S[j].x) j--;
        if (i < j) {
            point temp = S[i]; S[i] = S[j]; S[j] = temp;
            i++;
        }
        while (i < j && S[i].x <= S[j].x) i++;
        if (i < j) {
            point temp = S[i]; S[i] = S[j]; S[j] = temp;
            j--;
        }
    }
    return i;
}

void QuickSort_X(point s[], int low, int high) {
    int pivot;
    if (low < high) {
        pivot = Partition_X(s, low, high);
        QuickSort_X(s, low, pivot - 1);
        QuickSort_X(s, pivot + 1, high);
    }
}

// 快排一趟排序(Y轴)
int Partition_Y(point S[], int low, int high) {
    int i = low, j = high;
    while (i < j) {
        while (i < j && S[i].y <= S[j].y) j--;
        if (i < j) {
            point temp = S[i]; S[i] = S[j]; S[j] = temp;
            i++;
        }
        while (i < j && S[i].y <= S[j].y) i++;
        if (i < j) {
            point temp = S[i]; S[i] = S[j]; S[j] = temp;
            j--;
        }
    }
    return i;
}

void QuickSort_Y(point s[], int low, int high) {
    int pivot;
    if (low < high) {
        pivot = Partition_Y(s, low, high);
        QuickSort_Y(s, low, pivot - 1);
        QuickSort_Y(s, pivot + 1, high);
    }
}

// 找出最近点对
double Closest(point S[], point *Q, int low, int high) {
    double d1, d2, d3, d;
    int mid, i, j, index;
    point P[3000]; // 存放点集合P1和P2
    point Q1[2], Q2[2];
    
    if (high - low == 1) {
        Q[0] = S[low];
        Q[1] = S[high];
        return Distance(S[low], S[high]);
    }
    
    if (high - low == 2) {
        d1 = Distance(S[low], S[low + 1]);
        d2 = Distance(S[low + 1], S[high]);
        d3 = Distance(S[low], S[high]);
        
        if ((d1 < d2) && (d1 < d3)) {
            Q[0] = S[low];
            Q[1] = S[low + 1];
            return d1;
        } else if (d2 < d3) {
            Q[0] = S[low + 1];
            Q[1] = S[high];
            return d2;
        } else {
            Q[0] = S[low];
            Q[1] = S[high];
            return d3;
        }
    }
    
    mid = (low + high) / 2;
    d1 = Closest(S, Q1, low, mid);
    d2 = Closest(S, Q2, mid + 1, high);
    
    if (d1 <= d2) {
        d = d1;
        Q[0] = Q1[0];
        Q[1] = Q1[1];
    } else {
        d = d2;
        Q[0] = Q2[0];
        Q[1] = Q2[1];
    }
    
    index = 0;
    for (i = mid; (i >= low) && (S[mid].x - S[i].x < d); i--)
        P[index++] = S[i];
    for (i = mid + 1; (i <= high) && (S[i].x - S[mid].x < d); i++)
        P[index++] = S[i];
    
    QuickSort_Y(P, 0, index - 1);
    
    for (i = 0; i < index; i++) {
        for (j = i + 1; j < index; j++) {
            if (P[j].y - P[i].y >= d)
                break;
            else {
                d3 = Distance(P[i], P[j]);
                if (d3 < d) {
                    Q[0] = P[i];
                    Q[1] = P[j];
                    d = d3;
                }
            }
        }
    }
    
    return d;
}

// 绘制点集和最近点对
void DrawPoints(point S[], int n, point Q[], double distance) {
    // 设置背景色为白色
    setbkcolor(WHITE);
    cleardevice();
    
    // 绘制坐标轴
    setcolor(BLACK);
    line(50, WINDOW_HEIGHT - 50, WINDOW_WIDTH - 50, WINDOW_HEIGHT - 50); // X轴
    line(50, 50, 50, WINDOW_HEIGHT - 50); // Y轴
    
    // 绘制刻度
    for (int i = 1; i <= 10; i++) {
        // X轴刻度
        char label[10];
        sprintf(label, "%d", i);
        outtextxy(50 + i * SCALE_FACTOR - 5, WINDOW_HEIGHT - 40, label);
        line(50 + i * SCALE_FACTOR, WINDOW_HEIGHT - 50, 50 + i * SCALE_FACTOR, WINDOW_HEIGHT - 45);
        
        // Y轴刻度
        outtextxy(30, WINDOW_HEIGHT - 50 - i * SCALE_FACTOR + 5, label);
        line(50, WINDOW_HEIGHT - 50 - i * SCALE_FACTOR, 55, WINDOW_HEIGHT - 50 - i * SCALE_FACTOR);
    }
    
    // 绘制所有点（蓝色）
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    for (int i = 0; i < n; i++) {
        int x = 50 + (int)(S[i].x * SCALE_FACTOR);
        int y = WINDOW_HEIGHT - 50 - (int)(S[i].y * SCALE_FACTOR);
        fillellipse(x, y, 3, 3);
    }
    
    // 绘制最近点对（红色）
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    int x1 = 50 + (int)(Q[0].x * SCALE_FACTOR);
    int y1 = WINDOW_HEIGHT - 50 - (int)(Q[0].y * SCALE_FACTOR);
    int x2 = 50 + (int)(Q[1].x * SCALE_FACTOR);
    int y2 = WINDOW_HEIGHT - 50 - (int)(Q[1].y * SCALE_FACTOR);
    
    fillellipse(x1, y1, 5, 5);
    fillellipse(x2, y2, 5, 5);
    
    // 连接最近点对
    line(x1, y1, x2, y2);
    
    // 显示距离
    char distText[50];
    sprintf(distText, "Distance: %.2f", distance);
    outtextxy((x1 + x2) / 2, (y1 + y2) / 2 - 20, distText);
    
    // 添加标题
    setcolor(BLACK);
    outtextxy(WINDOW_WIDTH / 2 - 100, 20, "Closest Pair of Points");
    
    // 添加图例
    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(100, WINDOW_HEIGHT - 30, 3, 3);
    outtextxy(110, WINDOW_HEIGHT - 35, "Points");
    
    setfillstyle(SOLID_FILL, RED);
    fillellipse(200, WINDOW_HEIGHT - 30, 5, 5);
    outtextxy(210, WINDOW_HEIGHT - 35, "Closest Pair");
}