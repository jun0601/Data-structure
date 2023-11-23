/*
컴퓨터정보통신학과 2019253018 이준형 
BST구현
visual stdio로 구현함

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) (((a)>(b))?(a):(b))

typedef struct anod* TreePointer;

typedef struct anod {
	int sno;
	char name[50];
	TreePointer leftChild, rightChild;
} Ty_Node;

TreePointer insertBST(TreePointer root, Ty_Node data, int* lv)
{
	(*lv)++;
	if (!root)
	{
		TreePointer new = (TreePointer)malloc(sizeof(Ty_Node));
		new->sno = data.sno;
		strcpy(new->name, data.name);
		new->leftChild = NULL;
		new->rightChild = NULL;
		return new;
	}
	else
	{
		if (strcmp(root->name, data.name) > 0)
			root->leftChild = insertBST(root->leftChild, data, lv);
		else if (strcmp(root->name, data.name) < 0)
			root->rightChild = insertBST(root->rightChild, data, lv);
		else
			printf("데이터가 중복되었습니다.\n");
		return root;
	}
}

TreePointer searchBST(TreePointer root, char* name, int* lv)
{
	(*lv)++;
	if (!root)
		return NULL;
	if (!strcmp(root->name, name))
		return root;
	else
	{
		if (strcmp(root->name, name) > 0)
			return searchBST(root->leftChild, name, lv);
		else
			return searchBST(root->rightChild, name, lv);
	}
}

int leafBST(TreePointer root)
{
	int cnt = 0;
	if (root != NULL)
	{
		if (root->leftChild == NULL && root->rightChild == NULL)
		{
			return 1;
		}
		else
		{
			cnt = leafBST(root->leftChild) + leafBST(root->rightChild);
		}
	}
	return cnt;
}

int heightBST(TreePointer root)
{
	int height = 0;
	if (root != NULL)
	{
		height = 1 + max(heightBST(root->leftChild), heightBST(root->rightChild));
	}
	return height;
}

void deleteNode(TreePointer root, char* name)
{
	TreePointer parent, node, succ, succ_parent;
	TreePointer child;

	parent = NULL;
	node = root;

	//삭제할 노드의 위치 탐색
	while ((node != NULL) && strcmp(node->name, name))
	{
		parent = node;
		if (strcmp(node->name, name) > 0)
			node = node->leftChild;
		else
			node = node->rightChild;
	}

	//삭제할 노드가 없는 경우
	if (node == NULL)
	{
		printf("그런 학생 없습니다.\n");
		return;
	}

	//삭제할 노드가 단말 노드인 경우
	if ((node->leftChild == NULL) && (node->rightChild == NULL))
	{
		if (parent != NULL)
		{
			if (parent->leftChild == node)
				parent->leftChild = NULL;
			else
				parent->rightChild = NULL;
		}
		else root = NULL;
	}

	//삭제할 노드가 자식 노드를 한 개 가진 경우
	else if ((node->leftChild == NULL) || (node->rightChild == NULL))
	{
		if (node->leftChild != NULL)
			child = node->leftChild;
		else
			child = node->rightChild;
		if (parent != NULL)
		{
			if (parent->leftChild == node)
				parent->leftChild = child;
			else
				parent->rightChild = child;
		}
		else root = child;
	}

	//삭제할 노드가 자식 노드를 두 개 가진 경우
	else
	{
		succ_parent = node;
		succ = node->leftChild;

		//왼쪽 서브 트리에서 후계자 찾기
		while (succ->rightChild != NULL)
		{
			succ_parent = succ;
			succ = succ->rightChild;
		}

		if (succ_parent->leftChild == succ)
			succ_parent->leftChild = succ->leftChild;
		else
			succ_parent->rightChild = succ->leftChild;
		strcpy(node->name, succ->name);
		node = succ;
	}
	printf("성공적으로 삭제되었습니다.\n");
	free(node);
}

void inorder_traversal(TreePointer root)
{
	if (!root)
		return;
	inorder_traversal(root->leftChild);
	printf("%s %d\n", root->name, root->sno);
	inorder_traversal(root->rightChild);
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

void    all_free(char* cmd, char* av1, char* av2)
{
	free(cmd);
	free(av1);
	free(av2);
}

int main()
{
	FILE* fp;
	TreePointer root = NULL;
	Ty_Node data;
	char parse[255];
	char* cmd = NULL;
	char* av1 = NULL;
	char* av2 = NULL;
	int idx;
	int level = 0;

	fp = fopen("sdata.txt", "r");
	if (!fp)
	{
		printf("Failed to read file.\n");
		return 0;
	}
	while (!feof(fp))
	{
		fscanf(fp, "%d %s", &data.sno, data.name);
		root = insertBST(root, data, &level);
	}
	fclose(fp);

	while (1)
	{
		printf("========================================\n in  => 삽입\n sp =>이름순으로 출력\n de => 삭제\n se => 해당노드 레벨 출력\n ht => 트리 높이 출력\n ch => 해당노드 좌/우 자식 출력\n le =>총 leaf노드 개수 출력\n ex => 프로그램 종료\n수행할 작업을 선택하시오 :  ");

		if (!gets(parse))
			break;

		idx = 0;
		cmd = parsing(parse, &idx);
		av1 = parsing(parse, &idx);
		av2 = parsing(parse, &idx);

		if (!strcmp(cmd, "in"))
		{
			data.sno = atoi(av1);
			strcpy(data.name, av2);
			level = 0;
			root = insertBST(root, data, &level);
			printf("입력 성공! level = %d\n", level);
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "sp"))
		{
			level = 0;
			inorder_traversal(root);
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "de"))
		{
			deleteNode(root, av1);
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "se"))
		{
			level = 0;
			TreePointer node = searchBST(root, av1, &level);
			if (!node)
			{
				printf("그런 학생은 없습니다.\n");
				all_free(cmd, av1, av2);
				continue;
			}
			printf("이름: %s, 학번: %d, 레벨위치 = %d\n", node->name, node->sno, level);
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "ht"))
		{
			printf("height = %d\n", heightBST(root));
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "ch"))
		{
			level = 0;
			TreePointer node = searchBST(root, av1, &level);
			if (!node)
			{
				printf("그런 학생은 없습니다.\n");
				all_free(cmd, av1, av2);
				continue;
			}
			if (node->leftChild && node->rightChild)
				printf("left child = %s, right child = %s\n", node->leftChild->name, node->rightChild->name);
			else if (node->leftChild && !node->rightChild)
				printf("left child = %s, right child = NULL\n", node->leftChild->name);
			else if (!node->leftChild && node->rightChild)
				printf("left child = NULL, right child = %s\n", node->rightChild->name);
			else
				printf("left child = NULL, right child = NULL\n");
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "le"))
		{
			printf("number of leaf nodes = %d\n", leafBST(root));
			all_free(cmd, av1, av2);
		}
		else if (!strcmp(cmd, "ex"))
		{
			all_free(cmd, av1, av2);
			exit(1);
		}
		else
		{
			printf("명령어를 다시 입력하시오.\n");
			all_free(cmd, av1, av2);
		}
	}
}
