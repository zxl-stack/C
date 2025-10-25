#include <stdio.h>     
#include <string.h>     
#include <ctype.h>      // 字符处理函数
#include <stdlib.h>     // 标准库函数
#include <math.h>      
#define MAX1 10     // 最大变量数量
#define MAX2 100    // 最大表达式长度

// 全局变量
char vars[MAX1];    // 存储变量名的数组
int count = 0;      // 变量计数器
char expr[MAX2];    // 存储用户输入的表达式

// 函数声明
void get_variables();   // 提取表达式中的变量
void print_header();    // 打印真值表表头
void print_table();     // 打印真值表内容
int evaluate(int values[]); // 计算表达式的值
int parse_expression(int values[], int *pos); // 解析表达式
int precedence(char op);    // 获取运算符优先级
int apply_op(int a, int b, char op); // 应用运算符计算

int main() {
    printf("请输入命题公式(使用 & | ! > = 作为运算符):\n");
    fgets(expr, MAX2, stdin);
    expr[strcspn(expr, "\n")] = '\0';
    get_variables();
    print_header();
    print_table();
    return 0;  
}
// 从表达式中提取所有不同的变量名
void get_variables() {
    int i;
    for (i = 0; expr[i] != '\0'; i++) {
        if (isalpha(expr[i])) {
            int j, exists = 0;
            for (j = 0; j < count; j++) {
                if (vars[j] == expr[i]) {
                    exists = 1;  
                    break;
                }
            }
            if (!exists) {
                vars[count++] = expr[i];
                if (count >= MAX1) break;
            }
        }
    }
}

// 打印真值表的表头
void print_header() {
    int i;
    for (i = 0; i < count; i++) {
        printf("%c\t", vars[i]);
    }
    printf("%s\n", expr);
    for (i = 0; i < count; i++) {
        printf("--------");
    }
    printf("--------\n");
}

// 打印完整的真值表
void print_table() {
    int i, j;
    int total_rows = (int)pow(2, count);
    int values[MAX1];  
    for (i = 0; i < total_rows; i++) {
        for (j = 0; j < count; j++) {
            values[j] = (i >> (count - 1 - j)) & 1;
            printf("%d\t", values[j]);
        }
        printf("%d\n", evaluate(values));
    }
}

// 计算表达式在给定变量值下的结果
int evaluate(int values[]) {
    int pos = 0;  
    return parse_expression(values, &pos);
}
int parse_expression(int values[], int *pos) {
    int left = parse_term(values, pos);
    while (1) {
        char op = expr[*pos];
        if (op == '&' || op == '|' || op == '>' || op == '=') {
            (*pos)++;  
            int right = parse_term(values, pos);  
            left = apply_op(left, right, op);
        } else {
            break;  
        }
    } 
    return left;  
}

// 解析表达式中的项(变量、带括号的表达式或带!的表达式)
int parse_term(int values[], int *pos) {
    while (expr[*pos] == ' ') (*pos)++;
    if (expr[*pos] == '(') {
        (*pos)++;  
        int val = parse_expression(values, pos);  
        while (expr[*pos] != ')') (*pos)++;
        (*pos)++; 
        return val;
    } 
    else if (expr[*pos] == '!') {
        (*pos)++;  
        int val = parse_term(values, pos);  
        return !val; 
    }
    else if (isalpha(expr[*pos])) {
        char var = expr[*pos];
        (*pos)++;
        int i;
        for (i = 0; i < count; i++) {
            if (vars[i] == var) {
                return values[i];  
            }
        }
        return 0; 
    }  
    return 0; 
}

// 应用运算符进行计算
int apply_op(int a, int b, char op) {
    switch (op) {
        case '&': return a && b;  // 逻辑与
        case '|': return a || b;  // 逻辑或
        case '>': return !a || b; // 逻辑蕴含(等价于!a ∨ b)
        case '=': return a == b;  // 逻辑等价
        default: return 0;        // 未知运算符，默认返回0
    }
}