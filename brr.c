#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define MAP_WIDTH 50
#define MAP_HEIGHT 50
#define MAX_NODES 10000
#define STEP_SIZE 1.0
#define GOAL_THRESHOLD 1.0
#define PATH_DENSITY 0.5  // 路径点密度

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point position;
    int parent;
} Node;

typedef struct {
    Node nodes[MAX_NODES];
    int node_count;
} Tree;

typedef struct {
    double min_x, max_x;
    double min_y, max_y;
} MapBounds;

typedef struct {
    Point position;
    double radius;
} Obstacle;

// 函数前向声明
double distance_to_obstacles(Point p);
bool collision_free(Point a, Point b);
double distance(Point a, Point b);
bool line_circle_intersection(Point a, Point b, Obstacle obs);
void densify_path(Point *path, int *path_length);
bool verify_path(Point *path, int path_length);

// 全局变量
Tree tree_start, tree_goal;
MapBounds map_bounds = {0, MAP_WIDTH, 0, MAP_HEIGHT};
Obstacle obstacles[100];
int obstacle_count = 0;
Point start, goal;

// 计算两点间距离
double distance(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// 计算点到最近障碍物的距离
double distance_to_obstacles(Point p) {
    double min_dist = INFINITY;
    for (int i = 0; i < obstacle_count; i++) {
        double dist = distance(p, obstacles[i].position) - obstacles[i].radius;
        if (dist < min_dist) {
            min_dist = dist;
        }
    }
    return min_dist;
}

// 检查线段与圆是否相交（改进版碰撞检测）
bool line_circle_intersection(Point a, Point b, Obstacle obs) {
    // 向量AB和AO
    double ab_x = b.x - a.x;
    double ab_y = b.y - a.y;
    double ao_x = obs.position.x - a.x;
    double ao_y = obs.position.y - a.y;
    
    // 计算投影比例t
    double ab_length_sq = ab_x * ab_x + ab_y * ab_y;
    if (ab_length_sq < 1e-10) {
        // A和B是同一点，直接检查点到圆心距离
        return distance(a, obs.position) < obs.radius;
    }
    
    double t = (ao_x * ab_x + ao_y * ab_y) / ab_length_sq;
    t = fmax(0, fmin(1, t));  // 限制t在[0,1]范围内
    
    // 计算投影点
    double proj_x = a.x + t * ab_x;
    double proj_y = a.y + t * ab_y;
    
    // 计算投影点到圆心的距离
    double dist = distance((Point){proj_x, proj_y}, obs.position);
    
    return dist < obs.radius;
}

// 检查两点间直线是否与障碍物碰撞（使用改进的碰撞检测）
bool collision_free(Point a, Point b) {
    for (int i = 0; i < obstacle_count; i++) {
        if (line_circle_intersection(a, b, obstacles[i])) {
            printf("Collision detected between (%.2f, %.2f) and (%.2f, %.2f) at obstacle %d\n", 
                   a.x, a.y, b.x, b.y, i);
            return false;
        }
    }
    return true;
}

// 初始化树
void init_tree(Tree *tree, Point root) {
    tree->nodes[0].position = root;
    tree->nodes[0].parent = -1;
    tree->node_count = 1;
}

// 找到树上距离点最近的节点
int nearest_neighbor(Tree *tree, Point point) {
    int nearest = 0;
    double min_dist = distance(tree->nodes[0].position, point);
    
    for (int i = 1; i < tree->node_count; i++) {
        double dist = distance(tree->nodes[i].position, point);
        if (dist < min_dist) {
            min_dist = dist;
            nearest = i;
        }
    }
    return nearest;
}

// 从最近节点向随机点扩展
Point steer(Point from, Point to, double step_size) {
    Point new_point;
    double dist = distance(from, to);
    
    if (dist <= step_size) {
        new_point = to;
    } else {
        double ratio = step_size / dist;
        new_point.x = from.x + (to.x - from.x) * ratio;
        new_point.y = from.y + (to.y - from.y) * ratio;
    }
    return new_point;
}

// 双向自适应扩展策略
Point bidirectional_steer(Point from, Point to, Point goal, double step_random, double step_goal) {
    Point direction_random, direction_goal, new_point;
    double dist_random = distance(from, to);
    double dist_goal = distance(from, goal);
    
    if (dist_random > 0) {
        direction_random.x = (to.x - from.x) / dist_random;
        direction_random.y = (to.y - from.y) / dist_random;
    } else {
        direction_random.x = 0;
        direction_random.y = 0;
    }
    
    if (dist_goal > 0) {
        direction_goal.x = (goal.x - from.x) / dist_goal;
        direction_goal.y = (goal.y - from.y) / dist_goal;
    } else {
        direction_goal.x = 0;
        direction_goal.y = 0;
    }
    
    new_point.x = from.x + step_random * direction_random.x + step_goal * direction_goal.x;
    new_point.y = from.y + step_random * direction_random.y + step_goal * direction_goal.y;
    
    // 确保新点在地图范围内
    if (new_point.x < map_bounds.min_x) new_point.x = map_bounds.min_x;
    if (new_point.x > map_bounds.max_x) new_point.x = map_bounds.max_x;
    if (new_point.y < map_bounds.min_y) new_point.y = map_bounds.min_y;
    if (new_point.y > map_bounds.max_y) new_point.y = map_bounds.max_y;
    
    return new_point;
}

// 添加新节点到树
void add_node(Tree *tree, Point new_point, int parent) {
    if (tree->node_count >= MAX_NODES) return;
    
    tree->nodes[tree->node_count].position = new_point;
    tree->nodes[tree->node_count].parent = parent;
    tree->node_count++;
    
    printf("Added node %d: (%.2f, %.2f), parent=%d\n", 
           tree->node_count-1, new_point.x, new_point.y, parent);
}

// 增加路径点密度
void densify_path(Point *path, int *path_length) {
    Point new_path[MAX_NODES];
    int new_length = 0;
    
    for (int i = 0; i < *path_length - 1; i++) {
        Point a = path[i];
        Point b = path[i + 1];
        double dist = distance(a, b);
        
        // 添加起点
        new_path[new_length++] = a;
        
        // 沿线段均匀插入点
        int steps = (int)(dist / PATH_DENSITY) + 1;
        for (int j = 1; j < steps; j++) {
            double t = (double)j / steps;
            new_path[new_length++] = (Point){
                a.x + t * (b.x - a.x),
                a.y + t * (b.y - a.y)
            };
        }
    }
    
    // 添加终点
    new_path[new_length++] = path[*path_length - 1];
    
    // 复制回原数组
    memcpy(path, new_path, new_length * sizeof(Point));
    *path_length = new_length;
}

// 验证路径安全性
bool verify_path(Point *path, int path_length) {
    for (int i = 0; i < path_length - 1; i++) {
        if (!collision_free(path[i], path[i + 1])) {
            printf("Path verification failed between points %d and %d\n", i, i + 1);
            return false;
        }
    }
    return true;
}

// 路径优化(改进版)
void optimize_path(Point *path, int *path_length) {
    if (*path_length <= 2) return;
    
    int i = 0;
    while (i < *path_length - 1) {
        int j = *path_length - 1;
        bool found = false;
        
        while (j > i + 1) {
            if (collision_free(path[i], path[j])) {
                // 可以移除i+1到j-1之间的点
                for (int k = i + 1; k < j; k++) {
                    path[k] = path[j];
                }
                // 移动i到j的位置
                i = j;
                found = true;
                break;
            }
            j--;
        }
        
        if (!found) {
            i++;
        }
    }
    
    // 压缩数组，移除重复点
    int new_length = 1;
    for (int i = 1; i < *path_length; i++) {
        if (fabs(path[i].x - path[i-1].x) > 1e-6 || 
            fabs(path[i].y - path[i-1].y) > 1e-6) {
            path[new_length++] = path[i];
        }
    }
    
    *path_length = new_length;
    printf("Path optimized from %d to %d points\n", *path_length + (*path_length - new_length), *path_length);
}

// 改进的Bi-RRT算法
bool improved_bi_rrt(Point start, Point goal, int max_iter) {
    init_tree(&tree_start, start);
    init_tree(&tree_goal, goal);
    
    printf("Starting Bi-RRT with max iterations: %d\n", max_iter);
    
    for (int iter = 0; iter < max_iter; iter++) {
        // 随机采样点
        Point rand_point;
        if (rand() % 100 < 10) { // 10%的概率直接采样目标点
            rand_point = goal;
            printf("Iteration %d: Sampling goal point directly\n", iter);
        } else {
            rand_point.x = map_bounds.min_x + (double)rand() / RAND_MAX * (map_bounds.max_x - map_bounds.min_x);
            rand_point.y = map_bounds.min_y + (double)rand() / RAND_MAX * (map_bounds.max_y - map_bounds.min_y);
            printf("Iteration %d: Sampling random point (%.2f, %.2f)\n", iter, rand_point.x, rand_point.y);
        }
        
        // 从起始树扩展
        int nearest_start = nearest_neighbor(&tree_start, rand_point);
        Point new_start = bidirectional_steer(tree_start.nodes[nearest_start].position, 
                                            rand_point, goal, STEP_SIZE * 0.3, STEP_SIZE * 0.7);
        
        printf("Start tree: nearest=%d, new=(%.2f, %.2f)\n", 
               nearest_start, new_start.x, new_start.y);
        
        if (collision_free(tree_start.nodes[nearest_start].position, new_start)) {
            add_node(&tree_start, new_start, nearest_start);
            
            // 检查是否到达目标
            if (distance(new_start, goal) < GOAL_THRESHOLD) {
                printf("Goal reached from start tree after %d iterations!\n", iter);
                return true;
            }
        } else {
            printf("Collision detected in start tree extension\n");
        }
        
        // 从目标树扩展
        int nearest_goal = nearest_neighbor(&tree_goal, rand_point);
        Point new_goal = bidirectional_steer(tree_goal.nodes[nearest_goal].position, 
                                            rand_point, start, STEP_SIZE * 0.3, STEP_SIZE * 0.7);
        
        printf("Goal tree: nearest=%d, new=(%.2f, %.2f)\n", 
               nearest_goal, new_goal.x, new_goal.y);
        
        if (collision_free(tree_goal.nodes[nearest_goal].position, new_goal)) {
            add_node(&tree_goal, new_goal, nearest_goal);
            
            // 检查两棵树是否连接
            for (int i = 0; i < tree_start.node_count; i++) {
                if (distance(tree_start.nodes[i].position, new_goal) < GOAL_THRESHOLD && 
                    collision_free(tree_start.nodes[i].position, new_goal)) {
                    printf("Trees connected after %d iterations!\n", iter);
                    printf("Start tree node %d: (%.2f, %.2f)\n", 
                           i, tree_start.nodes[i].position.x, tree_start.nodes[i].position.y);
                    printf("Goal tree node %d: (%.2f, %.2f)\n", 
                           tree_goal.node_count-1, new_goal.x, new_goal.y);
                    return true;
                }
            }
        } else {
            printf("Collision detected in goal tree extension\n");
        }
        
        // 每100次迭代打印进度
        if ((iter + 1) % 100 == 0) {
            printf("Progress: %d/%d iterations, start tree: %d nodes, goal tree: %d nodes\n", 
                   iter + 1, max_iter, tree_start.node_count, tree_goal.node_count);
        }
    }
    
    printf("Failed to find path after %d iterations\n", max_iter);
    return false;
}

// 提取并合并两棵树的路径
void extract_combined_path(Point *path, int *path_length) {
    *path_length = 0;
    
    // 找到两棵树之间的连接点
    int connect_start_idx = -1;
    int connect_goal_idx = -1;
    double min_dist = INFINITY;
    
    for (int i = 0; i < tree_start.node_count; i++) {
        for (int j = 0; j < tree_goal.node_count; j++) {
            double dist = distance(tree_start.nodes[i].position, tree_goal.nodes[j].position);
            if (dist < min_dist) {
                min_dist = dist;
                connect_start_idx = i;
                connect_goal_idx = j;
            }
        }
    }
    
    if (connect_start_idx == -1 || connect_goal_idx == -1) {
        printf("Error: Could not find connection between trees\n");
        return;
    }
    
    printf("Connecting start tree node %d to goal tree node %d\n", 
           connect_start_idx, connect_goal_idx);
    
    // 从连接点回溯到起点
    int current = connect_start_idx;
    while (current != -1) {
        path[(*path_length)++] = tree_start.nodes[current].position;
        current = tree_start.nodes[current].parent;
    }
    
    // 反转路径
    for (int i = 0; i < *path_length / 2; i++) {
        Point temp = path[i];
        path[i] = path[*path_length - 1 - i];
        path[*path_length - 1 - i] = temp;
    }
    
    // 从连接点回溯到终点
    current = connect_goal_idx;
    while (current != -1) {
        path[(*path_length)++] = tree_goal.nodes[current].position;
        current = tree_goal.nodes[current].parent;
    }
    
    printf("Extracted combined path with %d points\n", *path_length);
}

// 主函数
int main() {
    srand(time(NULL));
    
    // 用户输入
    printf("Enter start point (x y): ");
    scanf("%lf %lf", &start.x, &start.y);
    
    printf("Enter goal point (x y): ");
    scanf("%lf %lf", &goal.x, &goal.y);
    
    printf("Enter number of obstacles: ");
    scanf("%d", &obstacle_count);
    
    for (int i = 0; i < obstacle_count; i++) {
        printf("Enter obstacle %d (x y radius): ", i+1);
        scanf("%lf %lf %lf", &obstacles[i].position.x, &obstacles[i].position.y, &obstacles[i].radius);
    }
    
    // 运行改进的Bi-RRT算法
    if (improved_bi_rrt(start, goal, 5000)) {
        printf("Path found!\n");
        
        // 提取合并路径
        Point path[MAX_NODES];
        int path_length = 0;
        extract_combined_path(path, &path_length);
        
        // 增加路径点密度
        densify_path(path, &path_length);
        printf("Path densified to %d points\n", path_length);
        
        // 验证路径安全性
        if (!verify_path(path, path_length)) {
            printf("Warning: Path contains collisions after densification\n");
            // 可选：尝试重新规划
        }
        
        // 优化路径
        optimize_path(path, &path_length);
        
        printf("Optimized path (%d points):\n", path_length);
        for (int i = 0; i < path_length; i++) {
            printf("(%.2f, %.2f)\n", path[i].x, path[i].y);
        }
        
        // 再次验证优化后的路径
        if (verify_path(path, path_length)) {
            printf("Path verification successful: all segments are collision-free\n");
        } else {
            printf("Error: Optimized path contains collisions\n");
        }
        
        printf("Reached goal!\n");
    } else {
        printf("Failed to find path.\n");
    }
    
    return 0;
}