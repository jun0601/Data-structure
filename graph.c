#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Max_vertex 100 // 최대 가능 정점 개수. 이는 작업할 그래프의 정점의 수보다 많이 큰 수임.
#define INFINITE_VAL 10000 // 무한대 수.(그래프의 모든 아크들의 비용의 총합보다 훨씬 큰 수.)
#define NUM_IMPOSSIBLE -99999 // 절대 등장하지 않을 것 같은 수.
typedef struct stnode* type_node_ptr; // 인접리스트의 노드 선언.
typedef struct stnode {
    int no; //정점번호
    double length; // 아크의 길이
    type_node_ptr link; // 다음 노드에 대한 포인터
} type_node;
int Num_vertices; // 작업할 그래프의 총 정점 개수.
double Cost_mat[Max_vertex][Max_vertex]; // 인접행렬. 모든 셀의 값을 INFINITE_VAL 로 초기화.
type_node_ptr Adj_list[Max_vertex]; // 인접리스트. 처음에는 NULL 로 초기화.
double dist[Max_vertex]; // Dijkstra 알고리즘이 사용하는 거리정보.
int visited[Max_vertex]; // 탐색함수들이 사용하는 방문여부 정보.(1:방문함; 0:방문 안함)
int pred[Max_vertex]; // 탐색함수들이 사용할 선행자 정보. (-1 이면 선행자 없음).
int S[Max_vertex]; // Dijkstra 알고리즘이 사용하는 처리완료정점 집합(1:들어있음; 0:안들어있음).


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
    int ib; //이 변수는 buffer 에서 다음 읽을 글자의 위치를 가진다.
    char buffer[100], command[30], sub[10], filename[100]; // 명령문 버퍼, 명령어, 서브명령어.
    FILE* fps; // 그래프 데이터 파일의 포인터.

    printf("\n그래프 데이터 파일 이름은? ");
    gets_s(filename, sizeof(filename));
    fps = fopen(filename, "r");
    if (!fps) {
        printf("파일 열기 실패.\n");
        return;
    }
    // 초기화: 인접행렬, 인접리스트.
    for (i = 0; i < Max_vertex; i++) {
        Adj_list[i] = NULL;
        for (j = 0; j < Max_vertex; j++) {
            Cost_mat[i][j] = INFINITE_VAL; // 이 값은 아크가 없음을 나타냄.
        }
    }
    read_and_construct_graph(fps); // 그래프 데이터 파일을 읽어 그래프표현을 만든다.
    //명령문 수행 루프.
    while (1) {
        // 한 명령의 수행 전에 먼저 visited, pred, S 를 초기화한다.
        //for (i = 0; i < Num_vertices; i++) {
        for (i = 0; i < Max_vertex; i++) {
            visited[i] = 0; pred[i] = -1; S[i] = 0;
        }
        printf("\n명령문종류: dfs a 0 / dfs s 0 7 / bfs a 0 / bfs s 0 7 / dij a 0 / dij s 0 7 /exit.\n");
        printf("명령문을 넣으시오> ");
        gets_s(buffer, sizeof(buffer)); // 명령문 한 줄(line)을 읽어 온다. 문자열 buf 로 읽어 옴.
        ib = 0; // ib는 다음 처리할 글자를 가리킨다. 여기서는 첫 글자를 가리키게 초기화.
        read_word(buffer, &ib, command); // 먼저 명령단어를 읽는다.
        if (strlen(command) == 0) {
            printf("명령오류. \n");
            continue; // 명령단어가 없으면 다시 명령 요청.
        }
        if (strcmp(command, "dij") == 0) { // Dijkstra algorithm 에 의해 모든 노드로의 경로 찾기
            read_word(buffer, &ib, sub); // sub 명령 단어를 읽는다.
            if (strlen(command) == 0)
                continue; // 서브 명령단어가 없으면 다시 명령 요청.
            if (sub[0] == 'a') {
                start_vertex = read_num(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                dijkstra(start_vertex); // Dijkstra 알고리즘 수행.
                print_paths_dijkstra(start_vertex, -1); // 모든 정점으로의 경로 출력.
            }
            else if (sub[0] == 's') {
                start_vertex = read_num(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                destination_vertex = read_num(buffer, &ib); // 종착정점 읽기.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                dijkstra(start_vertex); // Dijkstra 알고리즘 수행.
                print_paths_dijkstra(start_vertex, destination_vertex); // 한 경로 출력.
            }
            else {
                printf("명령 오류.\n");
                continue;
            }
        } // dij
        else if (strcmp(command, "dfs") == 0) { // dfs에 의해 모든 노드로의 경로 찾기
            read_word(buffer, &ib, sub); // sub 명령 단어를 읽는다.
            if (strlen(command) == 0)
                continue; // 서브 명령단어가 없으면 다시 명령 요청.
            if (sub[0] == 'a') {
                start_vertex = read_num(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                pred[start_vertex] = -1; // 시작정점의 선행 정점은 없는 것으로 함.
                depth_first(start_vertex); // depth-first searach 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, -1); // 모든 정점으로의 경로 출력.
            }
            else if (sub[0] == 's') {
                start_vertex = read_num(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                destination_vertex = read_num(buffer, &ib); // 종착정점 읽기.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                pred[start_vertex] = -1; // 시작정점의 선행 정점은 없다.
                depth_first(start_vertex); // depth-first searach 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // 한 정점 경로 출력.
            }
            else {
                printf("sub 명령 오류.\n");
                continue;
            }
        } // dfs
        else if (strcmp(command, "bfs") == 0) { // bfs에 의해 모든 노드로의 경로 찾기
            read_word(buffer, &ib, sub); // sub 명령 단어를 읽는다.
            if (strlen(command) == 0)
                continue; // 서브 명령단어가 없으면 다시 명령 요청.
            if (sub[0] == 'a') {
                start_vertex = read_num(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                breadth_first(start_vertex); // breadth-first search 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, -1); // 모든 정점으로의 경로 출력.
            }
            else if (sub[0] == 's') {
                start_vertex = read_num(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                destination_vertex = read_num(buffer, &ib); // 종착정점 읽기.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                breadth_first(start_vertex); // breadth-first search 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // 한 정점 경로 출력.
            }
            else {
                printf("sub 명령 오류.\n");
                continue;
            }
        } // bfs
        else if (strcmp(command, "exit") == 0) {
            break; // 루프 종료
        }
        else {
            printf("명령오류. \n"); continue; // 잘못된 명령은 무시하고 다시 요청.
        }
    } // while(명령문 수행 루프)
    printf("\n프로그램을 종료합니다.\n");
} // main

// S 에 없는 것 중 dist 가 최소인 정점을 찾는다. 이 정점번호를 반환.
// S 에 안들어 있는 정점이 하나도 없으면 -1 을 반환.
int choose(int number_of_vertex) { // 입력 number_of_vertex 은 정점의 총 개수임.
    int i, j, min, minpos;

    //S에 없는 첫 정점을 찾자.
    for (i = 0; i < number_of_vertex; i++) {
        if (S[i] == 0)
            break;
    }
    if (i == number_of_vertex)
        return -1; // 정점들이 모두 S 에 들어 있다.
    minpos = i; // S에 없는 첫 정점번호.
    min = dist[i]; // 그 정점의 dist.
    for (j = i + 1; j < number_of_vertex; j++)
        if (S[j] == 0 && dist[j] < min) { // S 에 없는 정점이며 distance 가 min보다 더 작으면, 변경.
            min = dist[j];
            minpos = j;
        }
    return minpos;
} // choose
// 아래 큐 관련 함수들은 큐가 선형큐(linear queue)라 가정하여 작성됨.
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


// 정점 v 가 queue 안에 이미 존재하는지 체크한다. 
int exist_in_queue(int queue[], int front, int rear, int v) {
    int i, answer = 0;
    for (i = front + 1; i <= rear; i++) {
        if (queue[i] == v)
            answer = 1;
    }
    return answer; // 존재하면 1, 안하면 0 을 반환.
} // exist_in_queue


void read_word(char buf[], int* pos, char name[]) {
    int i = 0;
    // 공백문자나 컴마는 지나간다.
    while (buf[*pos] == ' ' || buf[*pos] == ',') (*pos)++;
    // 우측에 공백문자나 컴마나 널문자가 아니면 읽는다.
    while (buf[*pos] != ' ' && buf[*pos] != ',' && buf[*pos] != '\0') {
        name[i] = buf[*pos];
        i++; (*pos)++;
    }
    name[i] = '\0';
    return;
} // read_word


// 파일에 준비한 문법을 읽어 인접행렬(Cost_mat), 인접리스트(Adj_list)를 만든다.
// 총 정점수 Num_vertices 도 채운다.
void read_and_construct_graph(FILE* fps) {
    char line[500]; // line buffer
    char str[10];
    char* ret;
    int i, k, m, leng, nr;
    int v, dv; // 정점들
    double co; // 아크의 비용
    type_node_ptr nptr, curr; // 인접리스트 노드에 대한 포인터
    ret = fgets(line, 500, fps);
    if (!ret) {
        printf("Read a line error: no line for the number of vertices.\n");
        getchar();
    }
    // 총 정점수 를 읽어 전역변수 Num_vertices 에 넣는다.
    nr = sscanf(line, "%d", &Num_vertices);
    if (nr != 1) {
        printf("Read error: number of vertices.\n");
        getchar(); // 실행 중지.
    }
    int num_adj_lines = 0;
    do {
        // read a line of the list of arcs.
        ret = fgets(line, 500, fps); // read nonterminals line.
        if (!ret) { // no more line in the file
            break;
        }
        leng = strlen(line); // get the length of the line.
        if (leng == 0) break; // 줄에 한 글자도 없다면 읽기 종료.
        // 간선들의 출발정점 읽기.
        i = 0;
        while (i < leng && !isdigit(line[i])) // move to a digit.
            i++;
        k = 0;
        while (i < leng && isdigit(line[i])) { // collect digits.
            str[k] = line[i];
            i++; k++;
        }
        str[k] = '\0'; // finish the string.
        if (k == 0) break; // 한개의 숫자도 없음.
        v = atoi(str); // 간선의 출발정점.
        num_adj_lines++; // 인접정보 줄 수 증가.
        // 출발정점에 대한 인접정점들에 대한 정보를 읽자: 인접정점번호, 아크비용.
        do {
            while (i < leng && !isdigit(line[i])) // move to a digit.
                i++;
            if (i >= leng) { // 이 줄에 더 이상 인접정점이 없다.
                break;
            }
            // 인접 정점을 읽자.
            k = 0;
            while (i < leng && isdigit(line[i])) { // collect digits.
                str[k] = line[i];
                i++; k++;
            }
            str[k] = '\0'; // finish the string.
            dv = atoi(str); // 인접정점의 번호.
            // 아크비용을 읽자.
            k = 0;
            while (i < leng && !isdigit(line[i])) // move to a digit.
                i++;
            while (i < leng && (isdigit(line[i]) || line[i] == '.')) { // collect digits.
                str[k] = line[i];
                i++; k++;
            }
            str[k] = '\0'; // finish the string.
            if (k == 0) {
                printf("오류: 있어야 할 간선의 비용이 안나옴.\n");
                getchar(); // 실행 중지.
            }
            co = atof(str); // 이것이 아크비용이다.

            // store the arc Cost_mat to the Cost_mat matrix.
            Cost_mat[v][dv] = co;

            // 인접리스트의 노드 하나를 생성하여 붙인다.
            nptr = (type_node_ptr)malloc(sizeof(type_node));
            nptr->no = dv; nptr->length = co; nptr->link = NULL;

            // 해당 인접리스트의 맨뒤에 붙인다.
            curr = Adj_list[v];
            if (curr == NULL)
                Adj_list[v] = nptr; // 리스트의 첫 노드가 됨.
            else {
                while (curr->link) // 맨 뒤의 노드로 간다.
                    curr = curr->link;
                curr->link = nptr; // 새 노드를 맨 뒤에 붙인다.
            }
        } while (1);
    } while (1);
    if (Num_vertices != num_adj_lines) {
        printf("인접정보의 줄 수가 총 정점수 와 다르다. (강제종료 하시오.) \n");
        getchar(); // 실행중지.
    }
    fclose(fps);
} // read_and_construct_graph

//------------------------여기서부터 함수 작성
int read_num(char buf[], int* pos) {
    int a = 0, i = 0;
    char numstring[30];
    while (buf[*pos] == ' ' || buf[*pos] == ',') (*pos)
        ++; // 공백문자, 컴마는 무시
    while (isdigit(buf[*pos])) {
        numstring[i] = buf[*pos]; i++; (*pos)++;
    }
    numstring[i] = '\0';
    if (i == 0) {
        printf("수를 읽을 수 없습니다. \n");
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
                printf("\n%d에서 %d로의 최단경로 : ", start, a);
                b = a;
                if (pred[b] == NUM_IMPOSSIBLE) {
                    printf("경로가 없습니다.");
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
        printf("\n%d에서 %d로의 경로 : ", start, desti);
        b = desti;
        if (pred[b] == NUM_IMPOSSIBLE) {
            printf("경로가 없습니다.\n");
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
                printf("%d 에서 %d 로의 최단경로: ", start, x);
                y = x;
                if (pred[y] == -1) {
                    printf("경로가 없음\n");
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
                    printf(". 경로길이 = %.2lf\n", dist[x]);
                }
            }
        }
    }
    else {
        printf("%d 에서 %d 로의 최단경로: ", start, desti);
        y = desti;
        if (pred[y] == -1) {
            printf("경로가 없습니다.");
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
            printf(". 경로길이 =  %.2lf\n", dist[desti]);
        }
    }
}