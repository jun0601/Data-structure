/*
컴퓨터정보통신학과 2019253018 이준형
visual studio이용해 구현함.

*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <Windows.h>  //gotoxy()용 라이브러리

#define m 12
#define p 12        
#define Max_dir  8   // 총 가능한 이동방향의 수
#define Timestep 100 // 시간단위는 ms 이므로 1000 이면 1 초를 멈추게 됨 but, 100으로 시간 조정

typedef struct Aoff {
    short int vert;
    short int horiz;
} offsets;

typedef struct  St_element {    // 스택에 저장할 데이터
    short int row;
    short int col;
    short int dir;
} ty_element;

typedef struct ListNode* listPointer;
typedef struct ListNode {       // 연결스택의 노드
    ty_element data;
    listPointer link;
} listNode;

//                                           N      NE        E      SE       S       SW        W       NW     
offsets move[Max_dir] = { {-1,0}, {-1,1},  {0,1},  {1,1},   {1,0},  {1,-1},  {0,-1}, {-1,-1} };

int maze[m + 2][p + 2];     // maze 행렬.
int mark[m + 2][p + 2];     // mark 행렬

void gotoxy(int x, int y) 
{
    COORD Pos = { x ,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void push(listPointer* top, ty_element item) {
    listPointer temp;
    temp = (listPointer)malloc(sizeof(listNode));
    temp->data = item;
    temp->link = *top;
    *top = temp;
}

ty_element pop(listPointer* top) {
    // 확인사항 -> 호출전에  top 이  NULL 이 아닌 것을 확인하고 호출해야 함
    listPointer temp = *top;
    ty_element item;

    item = temp->data;
    *top = temp->link;
    free(temp);
    return item;
}

//  top (i.e. *ptr_top)이 가리키는  연결리스트를 reversing 한다.
void reverse(listPointer* ptr_top) {
    listPointer curr;
    listPointer temp;
    listPointer next;
    curr = *ptr_top; // the first node pointed to by top.

    if (!curr || curr->link == NULL) // 0 개 또는 1 개만 가짐. 이 경우 더 이상 할 일이 없다.
        return;

    temp = curr; // 첫 노드.
    curr = curr->link; // curr 은 두번째 노드를 가리킨다.
    temp->link = NULL;  // 첫노드의 link 에 NULL 을 넣는다. (이것이 마지막 노드가 될 것임.)

    while (curr) {
        next = curr->link;
        // curr 를 맨 앞 노드가 되게 넣는다.
        curr->link = *ptr_top;  // 기존의 맨 앞이었던 노드가 2 번째가 됨.
        *ptr_top = curr;  // 헤더가 첫 노드를 가리킴
        curr = next;  // 다음 노드로 감.
    }
} // reverse

//  미로 경로 탐색 함수.
//  [sy,sx] 에서 시작하여 [dy, dx] 에 도달하는 경로를 찾는다.
int path(int sy, int sx, int dy, int dx)
{
    listPointer top = NULL;  // 스택의 top 변수. 초기에는 NULL.
    int i, j, row, col, nextRow, nextCol, dir, basex, basey, ocount;
    int found, num_bktrack = 0, path_length = 0;
    int EXIT_ROW = dy, EXIT_COL = dx;  // 목표점.
    ty_element position;

    if (maze[sy][sx] == 1 || maze[dy][dx] == 1) {
        printf("입력오류: 출발점이나 목표점이 막힌 셀입니다.\n");
        return 0;
    }

    // 미로를 먼저 화면에 출력
    CONSOLE_SCREEN_BUFFER_INFO presentCur;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
    basex = presentCur.dwCursorPosition.X; // 베이스 좌표의 x.
    basey = presentCur.dwCursorPosition.Y; // 베이스 좌표의 y.

    for (i = 0; i < m + 2; i++) {
        for (j = 0; j < p + 2; j++) {
            gotoxy(j + basex, i + basey);
            printf("%1d", maze[i][j]);
        }
    }

    // 빈 스택에 출발점을 넣는다.(초기화)
    position.row = sy; position.col = sx;  position.dir = 0;
    push(&top, position);

    // mark 정보초기화. 바운더리를 제외한 내부 셀에 모두 0 (방문안함)으로 초기화한다.
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            mark[1 + i][1 + j] = 0;
        }
    }
    mark[sy][sx] = 1;  // 출발점을 방문했음으로 표시.

    gotoxy(sx + basex, sy + basey);
    printf(">");    // 출발점을 표시한다
    Sleep(Timestep);

    found = 0;
    while (top != NULL) { // 스택이 비어 있거나(탐색실패 의미함), 탐색성공이면 이 루프를 종료함.
        position = pop(&top);
        row = position.row;  col = position.col;  dir = position.dir;
        while (dir < Max_dir) {  //현재셀의 dir 이 소진되었거나, 탐색성공이면 이 while 종료!
            nextRow = row + move[dir].vert;  // dir에 따른 다음셀을 준비.
            nextCol = col + move[dir].horiz;
            if (nextRow == EXIT_ROW && nextCol == EXIT_COL) {
                found = 1; // 다음셀이 목표점임. 경로탐색 성공!
                gotoxy(nextCol + basex, nextRow + basey);
                printf("<");
                break;
            }
            else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {  // 다음셀로 갈 수 있음.
                // 현재셀과 다음방향을 스택에 저장하고 다음셀로 이동.
                position.row = row; position.col = col;   position.dir = ++dir;
                push(&top, position);
                row = nextRow; col = nextCol; // 현재셀을 다음셀로 변경함 (즉 이동함).
                gotoxy(col + basex, row + basey);
                printf("*");
                Sleep(Timestep);
                dir = 0;   // 새로 이동한 셀에서는 dir = 0 부터 시도.
                mark[row][col] = 1;  // 이동한 곳을  "갔음"으로 표시.
            }
            else
                ++dir;
        }//while

        if (found)
            break;

        // 여기에 오면 [row, col] 의 모든 direction 이 실패한 것임. 따라서 backtracking  해야 함.
        // 이 작업의 구현은 while body의 첫 문장에서 함(pop 하여 스택의 이전 셀로 돌아감).
        gotoxy(col + basex, row + basey);  // 백트래킹 전에 경로표시를 지우고 0 을 넣음.
        printf("%c", '$');
        num_bktrack++;
        Sleep(Timestep);
    }//while

    gotoxy(0, basey + m + 2);  // 미로와 겹치지 않게 아래에 출력이 되게 커서를 옮김.
    printf("\n");

    if (found) { // 경로를 찾았으므로 출력함.
        printf("찾은 경로(row,col):\n");
        reverse(&top);   // 스택 연결리스트의 노드들을 역순으로 함.
        ocount = 0;
        while (top)
        {
            position = top->data;
            if ((ocount != 0) && (ocount % 6 == 0))
                printf("\n");
            printf("[%2d, %2d] ", position.row, position.col);
            ocount++;
            top = top->link;

        }

        printf("[%2d, %2d] ", row, col);
        printf("[%2d, %2d]\n\n", EXIT_ROW, EXIT_COL);
        path_length = ocount + 2;
        printf("경로길이= %d,  백트래킹수= %d\n\n", path_length, num_bktrack);
    }
    else
        printf("가능 경로가 없음.\n");
    return 1;
}  // path

void main() {
    int i, j, res;
    int starty, startx, endy, endx;
    FILE* fp;
    fp = fopen("maze_14_2.txt", "r");
    if (!fp) {
        printf("file open failed.\n");
        getchar();
    }

    // 파일에서 미로 읽기
    for (i = 0; i < m + 2; i++) {
        for (j = 0; j < p + 2; j++) {
            res = fscanf(fp, "%d", &maze[i][j]);
            if (res != 1) {
                printf("파일에서 maze 읽기 실패.\n");
                getchar();
            }
        }
    }

    while (1) {
        printf("출발점과 목표점을 입력하시오.(4개의 정수; 종료는 Control+D Enter): ");
        res = scanf("%d%d%d%d", &starty, &startx, &endy, &endx);
        if (res != 4)
            break;

        path(starty, startx, endy, endx);
    }

} // main
