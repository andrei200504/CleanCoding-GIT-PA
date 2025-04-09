#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Graph {
    int num_vertices;       // Numarul de varfuri
    int* visited;           // Vector de vizitare
    Node** adjacency_list;  // Liste de adiacenta
} Graph;

typedef struct Stack {
    int top;
    int capacity;           // Capacitatea stivei
    int* array;             // Array-ul stivei
} Stack;

Node* create_node(int vertex) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = vertex;
    new_node->next = NULL;
    return new_node;
}

void add_edge(Graph* graph, int src, int dest) {
    // Adauga muchie de la src la dest
    Node* new_node = create_node(dest);
    new_node->next = graph->adjacency_list[src];
    graph->adjacency_list[src] = new_node;

    // Adauga muchie de la dest la src (graful este neorientat)
    new_node = create_node(src);
    new_node->next = graph->adjacency_list[dest];
    graph->adjacency_list[dest] = new_node;
}

Graph* create_graph(int num_vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->adjacency_list = (Node**)malloc(num_vertices * sizeof(Node*));
    graph->visited = (int*)malloc(num_vertices * sizeof(int));

    for (int i = 0; i < num_vertices; i++) {
        graph->adjacency_list[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

Stack* create_stack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->array = (int*)malloc(capacity * sizeof(int));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void push(Stack* stack, int value) {
    if (stack->top < stack->capacity - 1) {
        stack->array[++stack->top] = value;
    }
}

void dfs(Graph* graph, Stack* stack, int vertex) {
    graph->visited[vertex] = 1;
    printf("%d ", vertex);
    push(stack, vertex);

    Node* adj_node = graph->adjacency_list[vertex];
    while (adj_node != NULL) {
        int neighbor = adj_node->data;
        if (!graph->visited[neighbor]) {
            dfs(graph, stack, neighbor);
        }
        adj_node = adj_node->next;
    }
}

void reset_visited(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->visited[i] = 0;
    }
}

void insert_edges(Graph* graph, int num_edges) {
    int src, dest;
    printf("Adauga %d muchii (de la 0 la %d):\n", num_edges, graph->num_vertices - 1);
    for (int i = 0; i < num_edges; i++) {
        scanf("%d %d", &src, &dest);
        if (src >= 0 && src < graph->num_vertices && dest >= 0 && dest < graph->num_vertices) {
            add_edge(graph, src, dest);
        } else {
            printf("Muchie invalida: %d %d\n", src, dest);
        }
    }
}

int is_reachable(Graph* graph, int start, int end) {
    Stack* stack = create_stack(graph->num_vertices);
    reset_visited(graph);
    dfs(graph, stack, start);

    int reachable = graph->visited[end];

    free(stack->array);
    free(stack);
    return reachable;
}

int main() {
    int num_vertices, num_edges;
    int start, end;

    printf("Cate noduri are graful? ");
    scanf("%d", &num_vertices);

    printf("Cate muchii are graful? ");
    scanf("%d", &num_edges);

    Graph* graph = create_graph(num_vertices);
    insert_edges(graph, num_edges);

    printf("Introdu nodul de start si nodul de sfarsit: ");
    scanf("%d %d", &start, &end);

    if (is_reachable(graph, start, end)) {
        printf("Exista drum intre nodurile %d si %d\n", start, end);
    } else {
        printf("Nu exista drum intre nodurile %d si %d\n", start, end);
    }

    // Elibereaza memoria alocata pentru listele de adiacenta
    for (int i = 0; i < num_vertices; i++) {
        Node* current = graph->adjacency_list[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjacency_list);
    free(graph->visited);
    free(graph);

    return 0;
}

