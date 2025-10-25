#include <stdio.h>
#include<string.h>
int main() 
{
    int a, s[100][100], i = 0, j = 0, m = 0, n = 0, p = 0, q = 0, k = 0;
    scanf("%d", &a);
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            scanf("%d", &s[i][j]);
        }
        getchar();
    }
    // 检查自反性
    for (m = 0; m < a; m++) {
        if (s[m][m] == 1) {
            n++;
        }
    }
    if (m == n) {
        printf("该矩阵是自反的\n");
    }
    else printf("该矩阵不是自反的\n");   
    // 检查反自反性
    for (m = 0, n = 0; m < a; m++) {
        if (s[m][m] == 0) {
            n++;
        }
    }
    if (m == n) {
        printf("该矩阵是反自反的\n");
    }
    else printf("该矩阵不是反自反的\n");
    
    // 检查对称性和反对称性
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (i == j) {
                continue;
            }
            if (s[i][j] != s[j][i]) {
                p++;
            }
            if (s[i][j] == s[j][i]) {
                q++;
            }
        }
    }
    if (p == a * a - a) {
        printf("该矩阵是反对称的\n");
    }
    else printf("该矩阵不是反对称的\n");
    if (q == a * a - a) {
        printf("该矩阵是对称的\n");
    }
    else printf("该矩阵不是对称的\n");
    // 检查传递性
    int t[100][100];
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            t[i][j] = s[i][j];
        }
    }
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (t[j][i] == 1) {
                for (k = 0; k < a; k++) {
                    t[j][k] = t[j][k] || t[i][k];
                }
            }
        }
    }
    int v = 0;
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (t[i][j] != s[i][j]) {
                v++;
            }
        }
    }
    if (v == 0) {
        printf("该矩阵是传递的\n");
    }
    else printf("该矩阵不是传递的\n");
    return 0;
}
