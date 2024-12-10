#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_NODES 100
#define INF INT_MAX


typedef struct Node {
    int dest;   
    int weight; 
    struct Node* next;
} Node;

Node* graph[MAX_NODES];
int dist[MAX_NODES];
int prev[MAX_NODES];
int visited[MAX_NODES];
int n; 


void init_graph(int nodes) {
    n = nodes;
    for (int i = 0; i < n; i++) {
        graph[i] = NULL;
    }
}

void add_edge(int src, int dest, int weight) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->dest = dest;
    new_node->weight = weight;
    new_node->next = graph[src];
    graph[src] = new_node;

    // 因为是无向图，也需要加上反向边
    new_node = (Node*)malloc(sizeof(Node));
    new_node->dest = src;
    new_node->weight = weight;
    new_node->next = graph[dest];
    graph[dest] = new_node;
}

void dijkstra(int start) {
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        int min_dist = INF;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                u = j;
                min_dist = dist[j];
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        for (Node* node = graph[u]; node != NULL; node = node->next) {
            int v = node->dest;
            int weight = node->weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }
}

typedef struct {
    char name[15];
    char intro[100];
} NodeInfo;

NodeInfo node_info[MAX_NODES];

void print_path(int start, int end) {
    if (dist[end] == INF) {
        printf("没有 %s 到 %s 的路径\n", node_info[start].name, node_info[end].name);
        return;
    }

    printf(" %s 到 %s 的最短路径长度为 %d\n", node_info[start].name, node_info[end].name, dist[end]);
    int path[MAX_NODES];
    int path_length = 0;

    for (int v = end; v != -1; v = prev[v]) {
        path[path_length++] = v;
    }

    printf("路径为: \n");
    for (int i = path_length - 1; i >= 0; i--) {
        printf("%s ", node_info[path[i]].name);
        if (i > 0) printf("-> ");
    }
    printf("\n");
}

int getInfo_byName(char* name) {
    for(int i = 0; i < n; i++) {
        if(strcmp(node_info[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void printInfo_byIndex(int index)
{
    printf("景点名称: %s\n", node_info[index].name);
    printf("景点编号: %d\n", index);
    printf("景点简介: %s\n\n", node_info[index].intro);
}

int main() {
    int nodes, edges, src, dest, weight;
    
    printf("输入景点数: ");
    scanf("%d", &nodes);
    init_graph(nodes);

    for(int i = 0 ; i < nodes; i++) {
        printf("输入景点 %d 的名字: ", i);
        scanf("%s", node_info[i].name);
        while(getchar() != '\n');
        printf("输入景点 %d 的简介: ", i);
        fgets(node_info[i].intro, 100, stdin);
        node_info[i].intro[strcspn(node_info[i].intro, "\n")] = 0;
    }

    printf("输入边的数目: ");
    scanf("%d", &edges);

    printf("输入边的信息 (起点 终点 长度):\n");
    for (int i = 0; i < edges; i++) {
        char src_name[15], dest_name[15];
        scanf("%s %s %d", src_name, dest_name, &weight);
        src = getInfo_byName(src_name);
        dest = getInfo_byName(dest_name);
        if(src == -1 || dest == -1) {
            printf("景点不存在\n");
            i--;
            continue;
        }
        add_edge(src, dest, weight);
        printf("添加边 %d -> %d, 长度 %d\n", src, dest, weight);
    }

    while (1)
    {
        char type[10];
        printf("请输入查询类型 (path/info/exit): ");
        scanf("%s", type);
        if(strcmp(type, "path") == 0) {
            char src_name[15], dest_name[15];
            printf("请输入起点和终点: ");
            scanf("%s %s", src_name, dest_name);
            src = getInfo_byName(src_name);
            dest = getInfo_byName(dest_name);
            if(src == -1 || dest == -1) {
                printf("景点不存在\n");
                continue;
            }
            dijkstra(src);
            print_path(src, dest);
            printf("\n");
        }
        else if(strcmp(type, "info") == 0) {
            char name[15];
            printf("请输入景点名字: ");
            scanf("%s", name);
            int index = getInfo_byName(name);
            if(index == -1) {
                printf("景点不存在\n");
                continue;
            }
            printInfo_byIndex(index);
            printf("\n");
        }
        else if(strcmp(type, "exit") == 0) {
            printf("退出程序\n");
            break;
        }
    }

    return 0;
}
