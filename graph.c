#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Max_vertex 100 // �ִ� ���� ���� ����. �̴� �۾��� �׷����� ������ ������ ���� ū ����.
#define INFINITE_VAL 10000 // ���Ѵ� ��.(�׷����� ��� ��ũ���� ����� ���պ��� �ξ� ū ��.)
#define NUM_IMPOSSIBLE -99999 // ���� �������� ���� �� ���� ��.
typedef struct stnode* type_node_ptr; // ��������Ʈ�� ��� ����.
typedef struct stnode {
    int no; //������ȣ
    double length; // ��ũ�� ����
    type_node_ptr link; // ���� ��忡 ���� ������
} type_node;
int Num_vertices; // �۾��� �׷����� �� ���� ����.
double Cost_mat[Max_vertex][Max_vertex]; // �������. ��� ���� ���� INFINITE_VAL �� �ʱ�ȭ.
type_node_ptr Adj_list[Max_vertex]; // ��������Ʈ. ó������ NULL �� �ʱ�ȭ.
double dist[Max_vertex]; // Dijkstra �˰����� ����ϴ� �Ÿ�����.
int visited[Max_vertex]; // Ž���Լ����� ����ϴ� �湮���� ����.(1:�湮��; 0:�湮 ����)
int pred[Max_vertex]; // Ž���Լ����� ����� ������ ����. (-1 �̸� ������ ����).
int S[Max_vertex]; // Dijkstra �˰����� ����ϴ� ó���Ϸ����� ����(1:�������; 0:�ȵ������).


void addq(int queue[], int* rear, int item);
int choose(int number_of_vertex);
int deleteq(int queue[], int* front, int rear);
int empty_queue(int front, int rear);
int exist_in_queue(int queue[], int front, int rear, int v);
void main();
void read_and_construct_graph(FILE* fps);


void read_and_construct_graph(char* fileName);
void depth_first(int current);
void breadth_first(int start);
void dijkstra(int v0);
void print_paths_dfs_bfs(int start, int desti);
void print_paths_dijkstra(int start, int desti);

int read_num(char buf[], int* pos);
void read_word(char buf[], int* pos, char name[]);

void main() {
    int i, j, start_vertex, destination_vertex;
    int ib; //�� ������ buffer ���� ���� ���� ������ ��ġ�� ������.
    char buffer[100], command[30], sub[10], filename[100]; // ��ɹ� ����, ��ɾ�, �����ɾ�.
    FILE* fps; // �׷��� ������ ������ ������.

    printf("\n�׷��� ������ ���� �̸���? ");
    gets_s(filename, sizeof(filename));
    fps = fopen(filename, "r");
    if (!fps) {
        printf("���� ���� ����.\n");
        return;
    }
    // �ʱ�ȭ: �������, ��������Ʈ.
    for (i = 0; i < Max_vertex; i++) {
        Adj_list[i] = NULL;
        for (j = 0; j < Max_vertex; j++) {
            Cost_mat[i][j] = INFINITE_VAL; // �� ���� ��ũ�� ������ ��Ÿ��.
        }
    }
    read_and_construct_graph(fps); // �׷��� ������ ������ �о� �׷���ǥ���� �����.
    //��ɹ� ���� ����.
    while (1) {
        // �� ����� ���� ���� ���� visited, pred, S �� �ʱ�ȭ�Ѵ�.
        //for (i = 0; i < Num_vertices; i++) {
        for (i = 0; i < Max_vertex; i++) {
            visited[i] = 0; pred[i] = -1; S[i] = 0;
        }
        printf("\n��ɹ�����: dfs a 0 / dfs s 0 7 / bfs a 0 / bfs s 0 7 / dij a 0 / dij s 0 7 /exit.\n");
        printf("��ɹ��� �����ÿ�> ");
        gets_s(buffer, sizeof(buffer)); // ��ɹ� �� ��(line)�� �о� �´�. ���ڿ� buf �� �о� ��.
        ib = 0; // ib�� ���� ó���� ���ڸ� ����Ų��. ���⼭�� ù ���ڸ� ����Ű�� �ʱ�ȭ.
        read_word(buffer, &ib, command); // ���� ��ɴܾ �д´�.
        if (strlen(command) == 0) {
            printf("��ɿ���. \n");
            continue; // ��ɴܾ ������ �ٽ� ��� ��û.
        }
        if (strcmp(command, "dij") == 0) { // Dijkstra algorithm �� ���� ��� ������ ��� ã��
            read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
            if (strlen(command) == 0)
                continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
            if (sub[0] == 'a') {
                start_vertex = read_num(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                dijkstra(start_vertex); // Dijkstra �˰��� ����.
                print_paths_dijkstra(start_vertex, -1); // ��� ���������� ��� ���.
            }
            else if (sub[0] == 's') {
                start_vertex = read_num(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                destination_vertex = read_num(buffer, &ib); // �������� �б�.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                dijkstra(start_vertex); // Dijkstra �˰��� ����.
                print_paths_dijkstra(start_vertex, destination_vertex); // �� ��� ���.
            }
            else {
                printf("��� ����.\n");
                continue;
            }
        } // dij
        else if (strcmp(command, "dfs") == 0) { // dfs�� ���� ��� ������ ��� ã��
            read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
            if (strlen(command) == 0)
                continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
            if (sub[0] == 'a') {
                start_vertex = read_num(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                pred[start_vertex] = -1; // ���������� ���� ������ ���� ������ ��.
                depth_first(start_vertex); // depth-first searach �˰��� ����.
                print_paths_dfs_bfs(start_vertex, -1); // ��� ���������� ��� ���.
            }
            else if (sub[0] == 's') {
                start_vertex = read_num(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                destination_vertex = read_num(buffer, &ib); // �������� �б�.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                pred[start_vertex] = -1; // ���������� ���� ������ ����.
                depth_first(start_vertex); // depth-first searach �˰��� ����.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // �� ���� ��� ���.
            }
            else {
                printf("sub ��� ����.\n");
                continue;
            }
        } // dfs
        else if (strcmp(command, "bfs") == 0) { // bfs�� ���� ��� ������ ��� ã��
            read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
            if (strlen(command) == 0)
                continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
            if (sub[0] == 'a') {
                start_vertex = read_num(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                breadth_first(start_vertex); // breadth-first search �˰��� ����.
                print_paths_dfs_bfs(start_vertex, -1); // ��� ���������� ��� ���.
            }
            else if (sub[0] == 's') {
                start_vertex = read_num(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                destination_vertex = read_num(buffer, &ib); // �������� �б�.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                breadth_first(start_vertex); // breadth-first search �˰��� ����.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // �� ���� ��� ���.
            }
            else {
                printf("sub ��� ����.\n");
                continue;
            }
        } // bfs
        else if (strcmp(command, "exit") == 0) {
            break; // ���� ����
        }
        else {
            printf("��ɿ���. \n"); continue; // �߸��� ����� �����ϰ� �ٽ� ��û.
        }
    } // while(��ɹ� ���� ����)
    printf("\n���α׷��� �����մϴ�.\n");
} // main

// S �� ���� �� �� dist �� �ּ��� ������ ã�´�. �� ������ȣ�� ��ȯ.
// S �� �ȵ�� �ִ� ������ �ϳ��� ������ -1 �� ��ȯ.
int choose(int number_of_vertex) { // �Է� number_of_vertex �� ������ �� ������.
    int i, j, min, minpos;

    //S�� ���� ù ������ ã��.
    for (i = 0; i < number_of_vertex; i++) {
        if (S[i] == 0)
            break;
    }
    if (i == number_of_vertex)
        return -1; // �������� ��� S �� ��� �ִ�.
    minpos = i; // S�� ���� ù ������ȣ.
    min = dist[i]; // �� ������ dist.
    for (j = i + 1; j < number_of_vertex; j++)
        if (S[j] == 0 && dist[j] < min) { // S �� ���� �����̸� distance �� min���� �� ������, ����.
            min = dist[j];
            minpos = j;
        }
    return minpos;
} // choose
// �Ʒ� ť ���� �Լ����� ť�� ����ť(linear queue)�� �����Ͽ� �ۼ���.
void addq(int queue[], int* rear, int item) {
    if (*rear == Max_vertex - 1) {
        printf("adding to queue has failed since queue is full.\n");
        return;
    }
    else
        queue[++ * rear] = item;
} // addq

int deleteq(int queue[], int* front, int rear) {
    if (*front == rear) {
        printf("delete failed since queue is empty.\n");
        return -999;
    }
    else
        return queue[++ * front];
} //deleteq
int empty_queue(int front, int rear) {
    if (front == rear) return 1;
    else return 0;
} // empty_queue


// ���� v �� queue �ȿ� �̹� �����ϴ��� üũ�Ѵ�. 
int exist_in_queue(int queue[], int front, int rear, int v) {
    int i, answer = 0;
    for (i = front + 1; i <= rear; i++) {
        if (queue[i] == v)
            answer = 1;
    }
    return answer; // �����ϸ� 1, ���ϸ� 0 �� ��ȯ.
} // exist_in_queue


void read_word(char buf[], int* pos, char name[]) {
    int i = 0;
    // ���鹮�ڳ� �ĸ��� ��������.
    while (buf[*pos] == ' ' || buf[*pos] == ',') (*pos)++;
    // ������ ���鹮�ڳ� �ĸ��� �ι��ڰ� �ƴϸ� �д´�.
    while (buf[*pos] != ' ' && buf[*pos] != ',' && buf[*pos] != '\0') {
        name[i] = buf[*pos];
        i++; (*pos)++;
    }
    name[i] = '\0';
    return;
} // read_word


// ���Ͽ� �غ��� ������ �о� �������(Cost_mat), ��������Ʈ(Adj_list)�� �����.
// �� ������ Num_vertices �� ä���.
void read_and_construct_graph(FILE* fps) {
    char line[500]; // line buffer
    char str[10];
    char* ret;
    int i, k, m, leng, nr;
    int v, dv; // ������
    double co; // ��ũ�� ���
    type_node_ptr nptr, curr; // ��������Ʈ ��忡 ���� ������
    ret = fgets(line, 500, fps);
    if (!ret) {
        printf("Read a line error: no line for the number of vertices.\n");
        getchar();
    }
    // �� ������ �� �о� �������� Num_vertices �� �ִ´�.
    nr = sscanf(line, "%d", &Num_vertices);
    if (nr != 1) {
        printf("Read error: number of vertices.\n");
        getchar(); // ���� ����.
    }
    int num_adj_lines = 0;
    do {
        // read a line of the list of arcs.
        ret = fgets(line, 500, fps); // read nonterminals line.
        if (!ret) { // no more line in the file
            break;
        }
        leng = strlen(line); // get the length of the line.
        if (leng == 0) break; // �ٿ� �� ���ڵ� ���ٸ� �б� ����.
        // �������� ������� �б�.
        i = 0;
        while (i < leng && !isdigit(line[i])) // move to a digit.
            i++;
        k = 0;
        while (i < leng && isdigit(line[i])) { // collect digits.
            str[k] = line[i];
            i++; k++;
        }
        str[k] = '\0'; // finish the string.
        if (k == 0) break; // �Ѱ��� ���ڵ� ����.
        v = atoi(str); // ������ �������.
        num_adj_lines++; // �������� �� �� ����.
        // ��������� ���� ���������鿡 ���� ������ ����: ����������ȣ, ��ũ���.
        do {
            while (i < leng && !isdigit(line[i])) // move to a digit.
                i++;
            if (i >= leng) { // �� �ٿ� �� �̻� ���������� ����.
                break;
            }
            // ���� ������ ����.
            k = 0;
            while (i < leng && isdigit(line[i])) { // collect digits.
                str[k] = line[i];
                i++; k++;
            }
            str[k] = '\0'; // finish the string.
            dv = atoi(str); // ���������� ��ȣ.
            // ��ũ����� ����.
            k = 0;
            while (i < leng && !isdigit(line[i])) // move to a digit.
                i++;
            while (i < leng && (isdigit(line[i]) || line[i] == '.')) { // collect digits.
                str[k] = line[i];
                i++; k++;
            }
            str[k] = '\0'; // finish the string.
            if (k == 0) {
                printf("����: �־�� �� ������ ����� �ȳ���.\n");
                getchar(); // ���� ����.
            }
            co = atof(str); // �̰��� ��ũ����̴�.

            // store the arc Cost_mat to the Cost_mat matrix.
            Cost_mat[v][dv] = co;

            // ��������Ʈ�� ��� �ϳ��� �����Ͽ� ���δ�.
            nptr = (type_node_ptr)malloc(sizeof(type_node));
            nptr->no = dv; nptr->length = co; nptr->link = NULL;

            // �ش� ��������Ʈ�� �ǵڿ� ���δ�.
            curr = Adj_list[v];
            if (curr == NULL)
                Adj_list[v] = nptr; // ����Ʈ�� ù ��尡 ��.
            else {
                while (curr->link) // �� ���� ���� ����.
                    curr = curr->link;
                curr->link = nptr; // �� ��带 �� �ڿ� ���δ�.
            }
        } while (1);
    } while (1);
    if (Num_vertices != num_adj_lines) {
        printf("���������� �� ���� �� ������ �� �ٸ���. (�������� �Ͻÿ�.) \n");
        getchar(); // ��������.
    }
    fclose(fps);
} // read_and_construct_graph

//------------------------���⼭���� �Լ� �ۼ�
int read_num(char buf[], int* pos) {
    int a = 0, i = 0;
    char numstring[30];
    while (buf[*pos] == ' ' || buf[*pos] == ',') (*pos)
        ++; // ���鹮��, �ĸ��� ����
    while (isdigit(buf[*pos])) {
        numstring[i] = buf[*pos]; i++; (*pos)++;
    }
    numstring[i] = '\0';
    if (i == 0) {
        printf("���� ���� �� �����ϴ�. \n");
        return NUM_IMPOSSIBLE;
    }
    else {
        a = atoi(numstring);
        return a;
    }
} // read_num

void depth_fir_re(int v) {
    type_node_ptr curr;
    visited[v] = 1;

    for (curr = Adj_list[v]; curr != NULL; curr = curr->link) {
        int adj_node = curr->no;
        if (!visited[adj_node]) {
            pred[adj_node] = v;
            depth_fir_re(adj_node);
        }
    }
}

void depth_first(int v0) {
    int k;
    for (k = 0; k < Num_vertices; k++) {
        visited[k] = 0;
        pred[k] = NUM_IMPOSSIBLE;
    }
    depth_fir_re(v0);
}

void breadth_first(int start) {
    int queue_a[Max_vertex];
    int front = 0, rear = -1;
    int i, j, t;
    type_node_ptr curr;

    for (i = 0; i < Num_vertices; i++) {
        visited[i] = 0;
        pred[i] = NUM_IMPOSSIBLE;
    }

    queue_a[++rear] = start;
    visited[start] = 1;
    pred[start] = -1;

    while (front <= rear) {
        t = queue_a[front++];

        for (curr = Adj_list[t]; curr != NULL; curr = curr->link) {
            int adj_node = curr->no;
            if (!visited[adj_node]) {
                queue_a[++rear] = adj_node;
                visited[adj_node] = 1;
                pred[adj_node] = t;
            }
        }
    }
}

void dijkstra(int v0) {
    int i, j, q;
    int count;
    double min_dis, dis;

    for (j = 0; j < Num_vertices; j++) {
        visited[j] = 0;
        dist[j] = Cost_mat[v0][j];
        if (dist[j] < INFINITE_VAL) {
            pred[j] = v0;
        }
        else {
            pred[j] = -1;
        }
    }

    visited[v0] = 1;
    dist[v0] = 0;
    count = 1;

    while (count < Num_vertices - 1) {
        min_dis = INFINITE_VAL;

        for (q = 0; q < Num_vertices; q++) {
            if (dist[q] < min_dis && !visited[q]) {
                min_dis = dist[q];
                j = q;
            }
        }

        visited[j] = 1;

        for (q = 0; q < Num_vertices; q++) {
            if (!visited[q]) {
                dis = min_dis + Cost_mat[j][q];
                if (dis < dist[q]) {
                    dist[q] = dis;
                    pred[q] = j;
                }
            }
        }

        count++;
    }
}

void print_paths_dfs_bfs(int start, int desti) {
    int a, b;
    if (desti == -1) {
        for (a = 0; a < Num_vertices; a++) {
            if (a != start) {
                printf("\n%d���� %d���� �ִܰ�� : ", start, a);
                b = a;
                if (pred[b] == NUM_IMPOSSIBLE) {
                    printf("��ΰ� �����ϴ�.");
                }
                else {
                    int road[Max_vertex];
                    int road_length = 0;
                    while (b != start) {
                        road[road_length] = b;
                        road_length++;
                        b = pred[b];
                    }

                    printf(" %d,", start);
                    for (int k = road_length - 1; k >= 0; k--) {
                        printf(" %d,", road[k]);
                    }
                }
                printf(".");
            }
        }
    }
    else {
        printf("\n%d���� %d���� ��� : ", start, desti);
        b = desti;
        if (pred[b] == NUM_IMPOSSIBLE) {
            printf("��ΰ� �����ϴ�.\n");
        }
        else {
            int road[Max_vertex];
            int road_length = 0;
            while (b != start) {
                road[road_length] = b;
                road_length++;
                b = pred[b];
            }

            printf(" %d,", start);
            for (int k = road_length - 1; k > 0; k--) {
                printf(" %d,", road[k]);
            }
            printf(" %d.", desti);
        }
    }
}

void print_paths_dijkstra(int start, int desti) {
    int x, y;
    if (desti == -1) {
        for (x = 0; x < Num_vertices; x++) {
            if (x != start) {
                printf("%d ���� %d ���� �ִܰ��: ", start, x);
                y = x;
                if (pred[y] == -1) {
                    printf("��ΰ� ����\n");
                }
                else {
                    int road[Max_vertex];
                    int road_length = 0;
                    while (y != start) {
                        road[road_length] = y;
                        road_length++;
                        y = pred[y];
                    }

                    printf(" %d,", start);
                    for (int k = road_length - 1; k >= 0; k--) {
                        printf(" %d,", road[k]);
                    }
                    printf(". ��α��� = %.2lf\n", dist[x]);
                }
            }
        }
    }
    else {
        printf("%d ���� %d ���� �ִܰ��: ", start, desti);
        y = desti;
        if (pred[y] == -1) {
            printf("��ΰ� �����ϴ�.");
        }
        else {
            int road[Max_vertex];
            int road_length = 0;
            while (y != start) {
                road[road_length] = y;
                road_length++;
                y = pred[y];
            }

            printf(" %d,", start);
            for (int k = (road_length - 1); k >= 0; k--) {
                printf(" %d,", road[k]);
            }
            printf(". ��α��� =  %.2lf\n", dist[desti]);
        }
    }
}