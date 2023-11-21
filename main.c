#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Listdata {
    char name[30];
    int id;
    char phone[20];
    char province[30];
} listData;  //������ �����ۿ� ���� ����


typedef struct Listnode* type_listNode_pointer; //�ϳ��� ����Ʈ ����� ���� ����

typedef struct Listnode {
    listData data;
    type_listNode_pointer link;
} listNode;


typedef struct LinkedList {
    type_listNode_pointer head;
    int length;
} linkedList; // ù��° ��带 ����Ű�� head�� ����

type_listNode_pointer find_last_node(type_listNode_pointer node)
{
    while (node->link != NULL)
        node = node->link;
    return node;
}

void insertLast(linkedList* L, listData item)
{
    type_listNode_pointer new, node;

    new = (type_listNode_pointer)malloc(sizeof(listNode));
    new->data = item;
    node = L->head;
    if (!node) // empty
        L->head = new;
    else // node exist
        find_last_node(node)->link = new;
    new->link = NULL;
    L->length++;
}

void printList(linkedList* L)
{
    type_listNode_pointer node = L->head;

    while (node != NULL) {
        printf("[ %s %d %s %s] -> ", node->data.name, node->data.id, node->data.phone, node->data.province);
        node = node->link;
    }
    printf("NULL\n");
}

type_listNode_pointer search(linkedList* L, int x)
{
    type_listNode_pointer target;

    target = L->head;
    while (target)
    {
        if (target->data.id == x)
            return target;
        target = target->link;
    }
    return NULL;
}

void insert(linkedList* L, listNode* pre, listData x)
{
    type_listNode_pointer new;

    new = (type_listNode_pointer)malloc(sizeof(listNode));
    new->data = x;
    if (L->head)
    {
        if (pre)
        {
            new->link = pre->link;
            pre->link = new;
        }
        else
        {
            new->link = L->head;
            L->head = new;
        }
    }
    else
    {
        new->link = NULL;
        L->head = new;
    }
    (L->length)++;
}

type_listNode_pointer search_prev_delete(linkedList* L, int x, int* ptr_found)
{
    type_listNode_pointer pre, cur;

    pre = NULL;
    cur = L->head;
    while (cur)
    {
        if (cur->data.id == x)
        {
            *ptr_found = 1;
            return pre;
        }
        else
        {
            pre = cur;
            cur = cur->link;
        }
    }
    *ptr_found = 0;
    return NULL;
}

void delete(linkedList* L, int x)
{
    type_listNode_pointer   pre, del;
    int found;

    pre = search_prev_delete(L, x, &found);
    if (!found)
        printf("student number does not exist\n");
    else
    {
        if (!pre) // ù ��带 ����
        {
            del = L->head;
            L->head = del->link;
            free(del);
        }
        else // ù ��尡 �ƴ� ��� ����
        {
            del = pre->link;
            pre->link = del->link;
            free(del);
        }
    }
    (L->length)--;
}

void reverse(linkedList* L)
{
    type_listNode_pointer node, new_head, tmp;

    node = L->head;
    new_head = NULL;
    while (node)
    {
        tmp = new_head;
        new_head = node;
        node = node->link;
        new_head->link = tmp;
    }
    L->head = new_head;
}

int getLength(linkedList* L)
{
    return (L->length);
}

char* parsing(char* parse, int* j)
{
    char* buf;
    int     i = 0;

    buf = (char*)malloc(sizeof(parse) * 15);
    while (*(parse + *j) == ',' || *(parse + *j) == ' ')
        (*j)++;
    while (*(parse + *j) != ',' && *(parse + *j) != ' ' && *(parse + *j) != '\0')
    {
        buf[i] = *(parse + *j);
        i++, (*j)++;
    }
    buf[i] = '\0';
    return buf;
}

void    all_free(char* cmd, char* av1, char* av2, char* av3, char* av4, char* av5)
{
    free(cmd);
    free(av1);
    free(av2);
    free(av3);
    free(av4);
    free(av5);
}

int main()
{
    linkedList  LL; // LL: NULL, length: 0
    LL.head = NULL;
    LL.length = 0;
    listData    data;

    FILE* fd;
    char        parse[100]; // ��ɹ�(����� �Է¹�)
    char* cmd = NULL;
    char* av1 = NULL;
    char* av2 = NULL;
    char* av3 = NULL;
    char* av4 = NULL;
    char* av5 = NULL;
    type_listNode_pointer   nptr; // search�Լ� return��
    int         idx;

    fd = fopen("student_data.txt", "r");
    if (!fd)
    {
        printf("Failed to read file.\n");
        return 0;
    }
    while (!feof(fd))
    {
        fscanf(fd, "%s %d %s %s", data.name, &data.id, data.phone, data.province);
        insertLast(&LL, data);
    }
    fclose(fd);

   
    printf("\t\t\t""REFERENCE\n��. print\n��. search 2151\n��. insert 5006, 3390, ������, 010-3390-4302, ����\n��. delete 3088\n��. reverse\n��. getLength\n��. exit\n");
    printf("===================================================\n");
    while (1)
    {
        printf("��ɾ �Է��Ͻÿ� :  ");

        if (!gets(parse))
            break;
        idx = 0;
        cmd = parsing(parse, &idx);
        av1 = parsing(parse, &idx);
        av2 = parsing(parse, &idx);
        av3 = parsing(parse, &idx);
        av4 = parsing(parse, &idx);
        av5 = parsing(parse, &idx);

        if (!strcmp(cmd, "print"))
        {
            printList(&LL);
            all_free(cmd, av1, av2, av3, av4, av5);
        }
        else if (!strcmp(cmd, "search"))
        {
            if (!(nptr = search(&LL, atoi(av1))))
                printf("������ ���� �й��� ���� �л��� �����ϴ�.\n");
            else
                printf("�̸�: %s, �й�: %d, ��ȭ��ȣ: %s, ��������: %s\n", nptr->data.name, nptr->data.id, nptr->data.phone, nptr->data.province);
            all_free(cmd, av1, av2, av3, av4, av5);
        }
        else if (!strcmp(cmd, "insert"))
        {
            if (!(nptr = search(&LL, atoi(av1))))
                printf("������ ���� �й��� ���� �л��� �����ϴ�.\n");
            else
            {
                // if (!*av5)
                // {
                //     printf("input is incorrectly, plz re-enter\n");
                //     continue;
                // }
                listData tmp;
                tmp.id = atoi(av2);
                strcpy(tmp.name, av3);
                strcpy(tmp.phone, av4);
                strcpy(tmp.province, av5);
                insert(&LL, nptr, tmp);
                printList(&LL);
            }
            all_free(cmd, av1, av2, av3, av4, av5);
        }
        else if (!strcmp(cmd, "delete"))
        {
            if (!(nptr = search(&LL, atoi(av1))))
                printf("������ ���� �й��� ���� �л��� �����ϴ�.\n");
            else
            {
                delete(&LL, nptr->data.id);
                printList(&LL);
            }
            all_free(cmd, av1, av2, av3, av4, av5);
        }
        else if (!strcmp(cmd, "reverse"))
        {
            reverse(&LL);
            printList(&LL);
            all_free(cmd, av1, av2, av3, av4, av5);
        }
        else if (!strcmp(cmd, "getLength"))
        {
            printf("list's length: [%d]\n", getLength(&LL));
            all_free(cmd, av1, av2, av3, av4, av5);
        }
        else if (!strcmp(cmd, "exit"))
        {
            // free �� �ʿ� X -> �ڵ����� ���α׷��� �޸� ȸ��
            printf("���α׷��� ����Ǿ����ϴ�.\n");
            return 0;
        }
        else
        {
            all_free(cmd, av1, av2, av3, av4, av5);
            printf("�ٽ� �Է��Ͻÿ�\n");
        }
    }
    return 0;
}