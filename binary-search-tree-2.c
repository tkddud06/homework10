/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * School of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node* left;
	struct node* right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;


/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = 0; // 원활한 원형 큐의 동작을 위해, front를 빈 공간으로, rear는 원소를 가르키도록 두며, 모두 0 인덱스에서 시작
int rear = 0;



int initializeBST(Node** h);
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* functions that you have to implement */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int deleteNode(Node* head, int key);  /* delete the node for the key */
Node* pop();
void push(Node* aNode);
Node* deQueue();
void enQueue(Node* aNode);



int main()
{
	char command;
	int key;
	Node* head = NULL;

	do { // 메뉴 형식으로 작동
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");
		printf("[--------------  [최상영]  	[2022041062]  --------------]\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) { // 초기화 하는 함수

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if (*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node)); 
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
	if (ptr) { // 빈 트리가 아닌 경우
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	} // 중위순회이므로, 왼쪽 이동, 그리고 자기자신 탐색, 오른쪽 이동 순으로 호출
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)
{
	Node* ptr = node;
	while (ptr != NULL || top != -1) // 끝까지 탐색하였고, 스택에도 남은 노드가 없을 때까지 진행할 것 ( 빈 트리가 아닌 경우만 탐색 시작 가능)
	{
		if (ptr == NULL) // 만약 리프 노드에 도달한 경우 (왼쪽이든, 오른쪽이든)
		{
			ptr = pop(); // 왼쪽 리프 노드에 도달한 경우, 다시 부모 노드로 올라간다. ( 만약 오른쪽 리프 노드에 도달하여 이 if문에 들어온 경우, 부모의 부모 노드로 바로 올라가게 된다.)
			printf(" [%d] ", ptr->key); // 해당 노드를 탐색하고, ( 중위순회기 때문에, 이 순서대로임)
			ptr = ptr->right; // 그 부모 노드의 오른쪽 노드로 내려간다.
			continue; // 기존 과정을 반복하여, 중위순회를 진행한다.
		}
		push(ptr); // 발견한 노드를 스택에 쌓는다.
		ptr = ptr->left; // 우선적으로 왼쪽 노드로 내려간다.
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	Node* node = ptr;
	if (node == NULL) // 빈 트리라면
	{
		return;
	}

	do
	{
		printf(" [%d] ", node->key); // 해당 노드를 탐색한 뒤,
		if(node->left != NULL) enQueue(node->left); // 해당 노드의 왼쪽 노드와
		if (node->right != NULL) enQueue(node->right); // 해당 노드의 오른쪽 노드를 큐에 순서대로 쌓는다
		node = deQueue(); // 순서대로 큐에서 꺼내와 탐색한다. 그렇게 되면, 레벨 순서대로 왼쪽에서 오른쪽으로 탐색할 수 있다.
	} while (node); // 모든 노드를 탐색할 때까지 진행할 것.
}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while (ptr != NULL) {

		/* if there is a node for the key, then just return */
		if (ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if (ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if (parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{

	if(head == NULL) // 초기화 자체가 안된 경우라면
	{
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) // 빈 트리라면
	{
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left->key == key) // 첫번째 노드가 지울 노드인 경우
	{
		Node* del = head->left; // 지울 노드 지정

		if (del->left == NULL && del->right == NULL) // 리프 노드인 경우
		{
			free(del);
			head->left = NULL; 
			return 0; // 지우고, 이전 노드의 left 링크(헤더의 left 링크)를 NULL로 설정
		}
		else if (del->left != NULL && del->right != NULL) // 자식 노드를 2개 가지는 경우
		{

			// 오른쪽 서브트리의 가장 작은 노드 찾기 ( 해당 가장 작은 노드는, 리프 노드이거나, 오른쪽 자식을 하나 가지는 경우가 있을 것.
			Node* temp = del->right;
			while (temp->left != NULL)
			{
				temp = temp->left;
			}

			// 해당 가장 작은 노드를 삭제하는 함수를 재 호출하여 삭제하고, 원래 함수에서 지워야 할 노드의 키를 해당 가장 작은 노드의 키로 바꿔줌으로써, 실질적으로는 지우는 기능 작동시킴
			int temp_key = temp->key;
			deleteNode(head, temp_key);
			del->key = temp_key;
			return 0;
		}
		else // 자식 노드를 1개만 가지는 경우
		{
			if (del->left != NULL) // 왼쪽 자식 노드가 있는 경우
			{
				head->left = del->left; // 남아있는 자식 노드로 원래 자리 교체
				free(del); // 지울 노드 free
				
			}
			else // 오른쪽 자식 노드가 있는 경우
			{
				head->left = del->right; // 남아있는 자식 노드로 원래 자리 교체
				free(del); // 지울 노드 free
			}
			return 0;
		}
	}

	Node* prev = head->left; // 루트 노드를 가리키는 prev 생성

	while (prev != NULL) // 끝까지 탐색할 때까지
	{
		if (prev->key > key) // 지울 키의 노드가 부모 노드의 키보다 왼쪽에 있을 경우
		{
			if (prev->left == NULL) // 더이상 서치할 수 있는 해당 자식이 없을 경우
			{
				break;
			}
			if (prev->left->key == key) // 바로 왼쪽 자식이 지울 노드일 경우
			{
				Node* del = prev->left; // 지울 노드 지정

				if (del->left == NULL && del->right == NULL) // 리프 노드인 경우
				{
					free(del);
					prev->left = NULL;
					return 0; // 지우고, 이전 노드의 left 링크를 NULL로 설정
				}
				else if (del->left != NULL && del->right != NULL) // 자식 노드를 2개 가지는 경우
				{

					// 오른쪽 서브트리의 가장 작은 노드 찾기 ( 해당 가장 작은 노드는, 리프 노드이거나, 오른쪽 자식을 하나 가지는 경우가 있을 것.
					Node* temp = del->right;
					while (temp->left != NULL)
					{
						temp = temp->left;
					}
					// 해당 가장 작은 노드를 재귀 호출을 통해 삭제하고, 원래 함수에서 지워야 할 노드의 키를 해당 가장 작은 노드의 키로 바꿔줌으로써, 실질적으로는 최초에 요청받은 노드를 지우는 기능 작동시킴
					int temp_key = temp->key;
					deleteNode(head, temp_key);
					del->key = temp_key;
					return 0;
				}
				else // 자식 노드를 1개만 가지는 경우
				{
					if (del->left != NULL) // 왼쪽 자식 노드가 있는 경우
					{
						prev->left = del->left; // 남아있는 자식 노드로 원래 자리 교체
						free(del); // 지울 노드 free
					}
					else // 오른쪽 자식 노드가 있는 경우
					{
						prev->left = del->right; // 남아있는 자식 노드로 원래 자리 교체
						free(del); // 지울 노드 free
					}
					return 0;
				}
			}
			else // 해당 자식이 지울 노드가 아닌 경우 해당 왼쪽 노드로 내려감
			{
				prev = prev->left;
			}
		}
		else // 반대 경우(지울 키의 노드가 부모 노드의 키보다 오른쪽에 있을 경우)
		{
			if (prev->right == NULL) // 더이상 서치할 수 있는 해당 자식이 없을 경우
			{
				break;
			}

			if (prev->right->key == key) // 바로 오른쪽 자식이 지울 노드일 경우
			{
				Node* del = prev->right; // 지울 노드 지정
				if (del->left == NULL && del->right == NULL) // 리프 노드인 경우
				{
					free(del);
					prev->right = NULL;
					return 0; // 지우고, 이전 노드의 right 링크를 NULL로 설정
				}
				else if (del->left != NULL && del->right != NULL) // 자식 노드를 2개 가지는 경우
				{


					// 오른쪽 서브트리의 가장 작은 노드 찾기 ( 해당 가장 작은 노드는, 리프 노드이거나, 오른쪽 자식을 하나 가지는 경우가 있을 것.
					Node* temp = del->right;
					while (temp->left != NULL)
					{
						temp = temp->left;
					}
					// 해당 가장 작은 노드를 재귀 호출을 통해 삭제하고, 원래 함수에서 지워야 할 노드의 키를 해당 가장 작은 노드의 키로 바꿔줌으로써, 실질적으로는 최초에 요청받은 노드를 지우는 기능 작동시킴
					int temp_key = temp->key;
					deleteNode(head, temp_key);
					del->key = temp_key;
					return 0;
				}
				else // 자식 노드를 1개만 가지는 경우
				{
					if (del->left != NULL) // 왼쪽 자식 노드가 있는 경우
					{
						prev->right = del->left; // 남아있는 자식 노드로 원래 자리 교체
						free(del); // 지울 노드 free
					}
					else // 오른쪽 자식 노드가 있는 경우
					{
						prev->right = del->right; // 남아있는 자식 노드로 원래 자리 교체
						free(del); // 지울 노드 free
					}
					return 0;
				}
			}
			else // 해당 자식이 지울 노드가 아닌 경우 해당 오른쪽 노드로 내려감
			{
				prev = prev->right;
			}
		}
	}

	printf("No node for key [%d]\n", key);
	return -1;
}


void freeNode(Node* ptr)
{
	if (ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if (head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	if (top == -1) // 원소는 0번 인덱스부터 들어가므로, top이 -1일 때는 빈 상태
	{
		// printf("stack is empty\n");
		return NULL;
	}
	// 비어있지 않다면, 탑의 원소를 temp에 저장한 뒤, top은 아래 원소를 가르키게 하고, temp 리턴
	Node* temp = stack[top];
	top--;
	return temp;
}

void push(Node* aNode)
{
	if (top + 1 == 20) // 스택은 20개의 원소를 저장 가능하므로, 인덱스상 19번까지 사용할 수 있다. 그러므로, 다음 원소를 저장할 공간이 19번을 넘어선, 20번인 경우가 꽉 찬 경우다
	{
		printf("stack is full\n");
		return;
	}
	// 꽉 차지 않았다면, 다음 인덱스에 새로운 원소를 넣는다
	top++;
	stack[top] = aNode;
}



Node* deQueue()
{
	if (front == rear) // front와 rear가 일치하는, 즉 모든 원소를 뽑아냈거나, front == -1 && rear == -1인 상태
	{
		// printf("queue is empty\n");
		return NULL;
	}
	// 그렇지 않으면
	front = (front + 1) % MAX_QUEUE_SIZE; // front는 원소를 가르키게 하고,
	// front의 원소를 temp에 저장한 뒤
	Node* temp = queue[front];
	return temp; // temp를 리턴
}

void enQueue(Node* aNode)
{
	if ((rear + 1) % MAX_QUEUE_SIZE == front) // 큐가 꽉 차서, 바로 다음 인덱스가 첫번째 원소(즉, front)가 되면 강제 리턴
	{
		printf("queue is full\n");
		return;
	}
	rear = (rear + 1) % MAX_QUEUE_SIZE; // 그렇지 않는 경우는 rear는 뒤로 이동하고,
	queue[rear] = aNode; // 해당 위치에 원소를 넣는다
}