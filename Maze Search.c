/*
��ǻ����������а� 2019253018 ������
visual studio�̿��� ������.

*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <Windows.h>  //gotoxy()�� ���̺귯��

#define m 12
#define p 12        
#define Max_dir  8   // �� ������ �̵������� ��
#define Timestep 100 // �ð������� ms �̹Ƿ� 1000 �̸� 1 �ʸ� ���߰� �� but, 100���� �ð� ����

typedef struct Aoff {
    short int vert;
    short int horiz;
} offsets;

typedef struct  St_element {    // ���ÿ� ������ ������
    short int row;
    short int col;
    short int dir;
} ty_element;

typedef struct ListNode* listPointer;
typedef struct ListNode {       // ���ὺ���� ���
    ty_element data;
    listPointer link;
} listNode;

//                                           N      NE        E      SE       S       SW        W       NW     
offsets move[Max_dir] = { {-1,0}, {-1,1},  {0,1},  {1,1},   {1,0},  {1,-1},  {0,-1}, {-1,-1} };

int maze[m + 2][p + 2];     // maze ���.
int mark[m + 2][p + 2];     // mark ���

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
    // Ȯ�λ��� -> ȣ������  top ��  NULL �� �ƴ� ���� Ȯ���ϰ� ȣ���ؾ� ��
    listPointer temp = *top;
    ty_element item;

    item = temp->data;
    *top = temp->link;
    free(temp);
    return item;
}

//  top (i.e. *ptr_top)�� ����Ű��  ���Ḯ��Ʈ�� reversing �Ѵ�.
void reverse(listPointer* ptr_top) {
    listPointer curr;
    listPointer temp;
    listPointer next;
    curr = *ptr_top; // the first node pointed to by top.

    if (!curr || curr->link == NULL) // 0 �� �Ǵ� 1 ���� ����. �� ��� �� �̻� �� ���� ����.
        return;

    temp = curr; // ù ���.
    curr = curr->link; // curr �� �ι�° ��带 ����Ų��.
    temp->link = NULL;  // ù����� link �� NULL �� �ִ´�. (�̰��� ������ ��尡 �� ����.)

    while (curr) {
        next = curr->link;
        // curr �� �� �� ��尡 �ǰ� �ִ´�.
        curr->link = *ptr_top;  // ������ �� ���̾��� ��尡 2 ��°�� ��.
        *ptr_top = curr;  // ����� ù ��带 ����Ŵ
        curr = next;  // ���� ���� ��.
    }
} // reverse

//  �̷� ��� Ž�� �Լ�.
//  [sy,sx] ���� �����Ͽ� [dy, dx] �� �����ϴ� ��θ� ã�´�.
int path(int sy, int sx, int dy, int dx)
{
    listPointer top = NULL;  // ������ top ����. �ʱ⿡�� NULL.
    int i, j, row, col, nextRow, nextCol, dir, basex, basey, ocount;
    int found, num_bktrack = 0, path_length = 0;
    int EXIT_ROW = dy, EXIT_COL = dx;  // ��ǥ��.
    ty_element position;

    if (maze[sy][sx] == 1 || maze[dy][dx] == 1) {
        printf("�Է¿���: ������̳� ��ǥ���� ���� ���Դϴ�.\n");
        return 0;
    }

    // �̷θ� ���� ȭ�鿡 ���
    CONSOLE_SCREEN_BUFFER_INFO presentCur;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
    basex = presentCur.dwCursorPosition.X; // ���̽� ��ǥ�� x.
    basey = presentCur.dwCursorPosition.Y; // ���̽� ��ǥ�� y.

    for (i = 0; i < m + 2; i++) {
        for (j = 0; j < p + 2; j++) {
            gotoxy(j + basex, i + basey);
            printf("%1d", maze[i][j]);
        }
    }

    // �� ���ÿ� ������� �ִ´�.(�ʱ�ȭ)
    position.row = sy; position.col = sx;  position.dir = 0;
    push(&top, position);

    // mark �����ʱ�ȭ. �ٿ������ ������ ���� ���� ��� 0 (�湮����)���� �ʱ�ȭ�Ѵ�.
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            mark[1 + i][1 + j] = 0;
        }
    }
    mark[sy][sx] = 1;  // ������� �湮�������� ǥ��.

    gotoxy(sx + basex, sy + basey);
    printf(">");    // ������� ǥ���Ѵ�
    Sleep(Timestep);

    found = 0;
    while (top != NULL) { // ������ ��� �ְų�(Ž������ �ǹ���), Ž�������̸� �� ������ ������.
        position = pop(&top);
        row = position.row;  col = position.col;  dir = position.dir;
        while (dir < Max_dir) {  //���缿�� dir �� �����Ǿ��ų�, Ž�������̸� �� while ����!
            nextRow = row + move[dir].vert;  // dir�� ���� �������� �غ�.
            nextCol = col + move[dir].horiz;
            if (nextRow == EXIT_ROW && nextCol == EXIT_COL) {
                found = 1; // �������� ��ǥ����. ���Ž�� ����!
                gotoxy(nextCol + basex, nextRow + basey);
                printf("<");
                break;
            }
            else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {  // �������� �� �� ����.
                // ���缿�� ���������� ���ÿ� �����ϰ� �������� �̵�.
                position.row = row; position.col = col;   position.dir = ++dir;
                push(&top, position);
                row = nextRow; col = nextCol; // ���缿�� �������� ������ (�� �̵���).
                gotoxy(col + basex, row + basey);
                printf("*");
                Sleep(Timestep);
                dir = 0;   // ���� �̵��� �������� dir = 0 ���� �õ�.
                mark[row][col] = 1;  // �̵��� ����  "����"���� ǥ��.
            }
            else
                ++dir;
        }//while

        if (found)
            break;

        // ���⿡ ���� [row, col] �� ��� direction �� ������ ����. ���� backtracking  �ؾ� ��.
        // �� �۾��� ������ while body�� ù ���忡�� ��(pop �Ͽ� ������ ���� ���� ���ư�).
        gotoxy(col + basex, row + basey);  // ��Ʈ��ŷ ���� ���ǥ�ø� ����� 0 �� ����.
        printf("%c", '$');
        num_bktrack++;
        Sleep(Timestep);
    }//while

    gotoxy(0, basey + m + 2);  // �̷ο� ��ġ�� �ʰ� �Ʒ��� ����� �ǰ� Ŀ���� �ű�.
    printf("\n");

    if (found) { // ��θ� ã�����Ƿ� �����.
        printf("ã�� ���(row,col):\n");
        reverse(&top);   // ���� ���Ḯ��Ʈ�� ������ �������� ��.
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
        printf("��α���= %d,  ��Ʈ��ŷ��= %d\n\n", path_length, num_bktrack);
    }
    else
        printf("���� ��ΰ� ����.\n");
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

    // ���Ͽ��� �̷� �б�
    for (i = 0; i < m + 2; i++) {
        for (j = 0; j < p + 2; j++) {
            res = fscanf(fp, "%d", &maze[i][j]);
            if (res != 1) {
                printf("���Ͽ��� maze �б� ����.\n");
                getchar();
            }
        }
    }

    while (1) {
        printf("������� ��ǥ���� �Է��Ͻÿ�.(4���� ����; ����� Control+D Enter): ");
        res = scanf("%d%d%d%d", &starty, &startx, &endy, &endx);
        if (res != 4)
            break;

        path(starty, startx, endy, endx);
    }

} // main
