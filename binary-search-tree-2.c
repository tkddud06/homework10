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
int front = 0; // ��Ȱ�� ���� ť�� ������ ����, front�� �� ��������, rear�� ���Ҹ� ����Ű���� �θ�, ��� 0 �ε������� ����
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

	do { // �޴� �������� �۵�
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");
		printf("[--------------  [�ֻ�]  	[2022041062]  --------------]\n");

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

int initializeBST(Node** h) { // �ʱ�ȭ �ϴ� �Լ�

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
	if (ptr) { // �� Ʈ���� �ƴ� ���
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	} // ������ȸ�̹Ƿ�, ���� �̵�, �׸��� �ڱ��ڽ� Ž��, ������ �̵� ������ ȣ��
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)
{
	Node* ptr = node;
	while (ptr != NULL || top != -1) // ������ Ž���Ͽ���, ���ÿ��� ���� ��尡 ���� ������ ������ �� ( �� Ʈ���� �ƴ� ��츸 Ž�� ���� ����)
	{
		if (ptr == NULL) // ���� ���� ��忡 ������ ��� (�����̵�, �������̵�)
		{
			ptr = pop(); // ���� ���� ��忡 ������ ���, �ٽ� �θ� ���� �ö󰣴�. ( ���� ������ ���� ��忡 �����Ͽ� �� if���� ���� ���, �θ��� �θ� ���� �ٷ� �ö󰡰� �ȴ�.)
			printf(" [%d] ", ptr->key); // �ش� ��带 Ž���ϰ�, ( ������ȸ�� ������, �� ���������)
			ptr = ptr->right; // �� �θ� ����� ������ ���� ��������.
			continue; // ���� ������ �ݺ��Ͽ�, ������ȸ�� �����Ѵ�.
		}
		push(ptr); // �߰��� ��带 ���ÿ� �״´�.
		ptr = ptr->left; // �켱������ ���� ���� ��������.
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	Node* node = ptr;
	if (node == NULL) // �� Ʈ�����
	{
		return;
	}

	do
	{
		printf(" [%d] ", node->key); // �ش� ��带 Ž���� ��,
		if(node->left != NULL) enQueue(node->left); // �ش� ����� ���� ����
		if (node->right != NULL) enQueue(node->right); // �ش� ����� ������ ��带 ť�� ������� �״´�
		node = deQueue(); // ������� ť���� ������ Ž���Ѵ�. �׷��� �Ǹ�, ���� ������� ���ʿ��� ���������� Ž���� �� �ִ�.
	} while (node); // ��� ��带 Ž���� ������ ������ ��.
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

	if(head == NULL) // �ʱ�ȭ ��ü�� �ȵ� �����
	{
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) // �� Ʈ�����
	{
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left->key == key) // ù��° ��尡 ���� ����� ���
	{
		Node* del = head->left; // ���� ��� ����

		if (del->left == NULL && del->right == NULL) // ���� ����� ���
		{
			free(del);
			head->left = NULL; 
			return 0; // �����, ���� ����� left ��ũ(����� left ��ũ)�� NULL�� ����
		}
		else if (del->left != NULL && del->right != NULL) // �ڽ� ��带 2�� ������ ���
		{

			// ������ ����Ʈ���� ���� ���� ��� ã�� ( �ش� ���� ���� ����, ���� ����̰ų�, ������ �ڽ��� �ϳ� ������ ��찡 ���� ��.
			Node* temp = del->right;
			while (temp->left != NULL)
			{
				temp = temp->left;
			}

			// �ش� ���� ���� ��带 �����ϴ� �Լ��� �� ȣ���Ͽ� �����ϰ�, ���� �Լ����� ������ �� ����� Ű�� �ش� ���� ���� ����� Ű�� �ٲ������ν�, ���������δ� ����� ��� �۵���Ŵ
			int temp_key = temp->key;
			deleteNode(head, temp_key);
			del->key = temp_key;
			return 0;
		}
		else // �ڽ� ��带 1���� ������ ���
		{
			if (del->left != NULL) // ���� �ڽ� ��尡 �ִ� ���
			{
				head->left = del->left; // �����ִ� �ڽ� ���� ���� �ڸ� ��ü
				free(del); // ���� ��� free
				
			}
			else // ������ �ڽ� ��尡 �ִ� ���
			{
				head->left = del->right; // �����ִ� �ڽ� ���� ���� �ڸ� ��ü
				free(del); // ���� ��� free
			}
			return 0;
		}
	}

	Node* prev = head->left; // ��Ʈ ��带 ����Ű�� prev ����

	while (prev != NULL) // ������ Ž���� ������
	{
		if (prev->key > key) // ���� Ű�� ��尡 �θ� ����� Ű���� ���ʿ� ���� ���
		{
			if (prev->left == NULL) // ���̻� ��ġ�� �� �ִ� �ش� �ڽ��� ���� ���
			{
				break;
			}
			if (prev->left->key == key) // �ٷ� ���� �ڽ��� ���� ����� ���
			{
				Node* del = prev->left; // ���� ��� ����

				if (del->left == NULL && del->right == NULL) // ���� ����� ���
				{
					free(del);
					prev->left = NULL;
					return 0; // �����, ���� ����� left ��ũ�� NULL�� ����
				}
				else if (del->left != NULL && del->right != NULL) // �ڽ� ��带 2�� ������ ���
				{

					// ������ ����Ʈ���� ���� ���� ��� ã�� ( �ش� ���� ���� ����, ���� ����̰ų�, ������ �ڽ��� �ϳ� ������ ��찡 ���� ��.
					Node* temp = del->right;
					while (temp->left != NULL)
					{
						temp = temp->left;
					}
					// �ش� ���� ���� ��带 ��� ȣ���� ���� �����ϰ�, ���� �Լ����� ������ �� ����� Ű�� �ش� ���� ���� ����� Ű�� �ٲ������ν�, ���������δ� ���ʿ� ��û���� ��带 ����� ��� �۵���Ŵ
					int temp_key = temp->key;
					deleteNode(head, temp_key);
					del->key = temp_key;
					return 0;
				}
				else // �ڽ� ��带 1���� ������ ���
				{
					if (del->left != NULL) // ���� �ڽ� ��尡 �ִ� ���
					{
						prev->left = del->left; // �����ִ� �ڽ� ���� ���� �ڸ� ��ü
						free(del); // ���� ��� free
					}
					else // ������ �ڽ� ��尡 �ִ� ���
					{
						prev->left = del->right; // �����ִ� �ڽ� ���� ���� �ڸ� ��ü
						free(del); // ���� ��� free
					}
					return 0;
				}
			}
			else // �ش� �ڽ��� ���� ��尡 �ƴ� ��� �ش� ���� ���� ������
			{
				prev = prev->left;
			}
		}
		else // �ݴ� ���(���� Ű�� ��尡 �θ� ����� Ű���� �����ʿ� ���� ���)
		{
			if (prev->right == NULL) // ���̻� ��ġ�� �� �ִ� �ش� �ڽ��� ���� ���
			{
				break;
			}

			if (prev->right->key == key) // �ٷ� ������ �ڽ��� ���� ����� ���
			{
				Node* del = prev->right; // ���� ��� ����
				if (del->left == NULL && del->right == NULL) // ���� ����� ���
				{
					free(del);
					prev->right = NULL;
					return 0; // �����, ���� ����� right ��ũ�� NULL�� ����
				}
				else if (del->left != NULL && del->right != NULL) // �ڽ� ��带 2�� ������ ���
				{


					// ������ ����Ʈ���� ���� ���� ��� ã�� ( �ش� ���� ���� ����, ���� ����̰ų�, ������ �ڽ��� �ϳ� ������ ��찡 ���� ��.
					Node* temp = del->right;
					while (temp->left != NULL)
					{
						temp = temp->left;
					}
					// �ش� ���� ���� ��带 ��� ȣ���� ���� �����ϰ�, ���� �Լ����� ������ �� ����� Ű�� �ش� ���� ���� ����� Ű�� �ٲ������ν�, ���������δ� ���ʿ� ��û���� ��带 ����� ��� �۵���Ŵ
					int temp_key = temp->key;
					deleteNode(head, temp_key);
					del->key = temp_key;
					return 0;
				}
				else // �ڽ� ��带 1���� ������ ���
				{
					if (del->left != NULL) // ���� �ڽ� ��尡 �ִ� ���
					{
						prev->right = del->left; // �����ִ� �ڽ� ���� ���� �ڸ� ��ü
						free(del); // ���� ��� free
					}
					else // ������ �ڽ� ��尡 �ִ� ���
					{
						prev->right = del->right; // �����ִ� �ڽ� ���� ���� �ڸ� ��ü
						free(del); // ���� ��� free
					}
					return 0;
				}
			}
			else // �ش� �ڽ��� ���� ��尡 �ƴ� ��� �ش� ������ ���� ������
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
	if (top == -1) // ���Ҵ� 0�� �ε������� ���Ƿ�, top�� -1�� ���� �� ����
	{
		// printf("stack is empty\n");
		return NULL;
	}
	// ������� �ʴٸ�, ž�� ���Ҹ� temp�� ������ ��, top�� �Ʒ� ���Ҹ� ����Ű�� �ϰ�, temp ����
	Node* temp = stack[top];
	top--;
	return temp;
}

void push(Node* aNode)
{
	if (top + 1 == 20) // ������ 20���� ���Ҹ� ���� �����ϹǷ�, �ε����� 19������ ����� �� �ִ�. �׷��Ƿ�, ���� ���Ҹ� ������ ������ 19���� �Ѿ, 20���� ��찡 �� �� ����
	{
		printf("stack is full\n");
		return;
	}
	// �� ���� �ʾҴٸ�, ���� �ε����� ���ο� ���Ҹ� �ִ´�
	top++;
	stack[top] = aNode;
}



Node* deQueue()
{
	if (front == rear) // front�� rear�� ��ġ�ϴ�, �� ��� ���Ҹ� �̾Ƴ°ų�, front == -1 && rear == -1�� ����
	{
		// printf("queue is empty\n");
		return NULL;
	}
	// �׷��� ������
	front = (front + 1) % MAX_QUEUE_SIZE; // front�� ���Ҹ� ����Ű�� �ϰ�,
	// front�� ���Ҹ� temp�� ������ ��
	Node* temp = queue[front];
	return temp; // temp�� ����
}

void enQueue(Node* aNode)
{
	if ((rear + 1) % MAX_QUEUE_SIZE == front) // ť�� �� ����, �ٷ� ���� �ε����� ù��° ����(��, front)�� �Ǹ� ���� ����
	{
		printf("queue is full\n");
		return;
	}
	rear = (rear + 1) % MAX_QUEUE_SIZE; // �׷��� �ʴ� ���� rear�� �ڷ� �̵��ϰ�,
	queue[rear] = aNode; // �ش� ��ġ�� ���Ҹ� �ִ´�
}