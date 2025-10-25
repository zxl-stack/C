#include <stdio.h>
#include <math.h>
int main() {
    int a, s[100][100], i = 0, j = 0, m = 0, n = 0, p = 0, k = 0; 
    scanf("%d", &a);
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            scanf("%d", &s[i][j]);
        }
    }
    int t1[100][100];
    int t2[100][100];
    int t3[100][100];
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            t1[i][j] = s[i][j];
            t2[i][j] = s[i][j];
            t3[i][j] = s[i][j];
        }
    }  
    printf("R的自反闭包为:\n");
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (i == j) {
                printf("1");
                m++;
            } else {
                printf("%d", t1[i][j]);
                m++;
            }
            if (m % a == 0) {
                printf("\n");
            }
        }
    }
    printf("R的对称闭包为:\n");
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (t2[i][j] != t2[j][i]) {
                t2[i][j] = t2[j][i] = 1;
            }
            printf("%d", t2[i][j]);
            n++;
            if (n % a == 0) {
                printf("\n");
            }
        }
    }
    printf("R的传递闭包为:\n");
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (t3[j][i] == 1) {
                for (k = 0; k < a; k++) {
                    t3[j][k] = t3[j][k] || t3[i][k];
                }
            }
        }
    }
    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            printf("%d", t3[i][j]);
            p++;
            if (p % a == 0) {
                printf("\n");
            }
        }
    }
    return 0;
}