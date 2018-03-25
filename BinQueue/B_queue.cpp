#include "stdafx.h"
#include "B_queue.h"

/* ���캯��:��ʼ������
 * ����ֵ:��
 * ����:size:������еĽ���
 */
B_queue::B_queue(int size)
: MaxSize(size){
	// ��ʼ���������
	CurrentSize = 0;
	TheTrees = new BinTree[MaxSize];
	for (int i = 0; i < MaxSize; i++)
		TheTrees[i] = NULL;
}

/* ��������:��������ʱ���մ洢�ռ�
 * ����ֵ:��
 * ����:��
 */
B_queue::~B_queue()
{
	MakeEmpty(); // �ÿն������

	// ɾ��ɭ��
	delete TheTrees;
	TheTrees = NULL;
}

/* �ÿպ���:����������ÿ�
 * ����ֵ:��
 * ����:��
 */
void B_queue::MakeEmpty() {
	// ��ɭ���е�ÿ�����ÿ�
	for (int i = 0; i < MaxSize; i++) 
		MakeEmpty(TheTrees[i]);
}

/* �ÿպ���:���������ÿ�
 * ����ֵ:��
 * ����:��
 */
void B_queue::MakeEmpty(BinTree Tree) {
	if (Tree == NULL)
		return;

	MakeEmpty(Tree->LeftChild); // �ݹ��ÿ�����
	MakeEmpty(Tree->NextSibling); // �ݹ��ÿ��ֵܽڵ�

	// ɾ����ǰ�ڵ�
	delete Tree;
	Tree = NULL;
}

/* չʾ����:չʾ��������е���Ϣ
 * ����ֵ:��
 * ����:��
 */
void B_queue::Display() {
	// չʾɭ���е�������Ϣ
	for (int i = 0; i < MaxSize; i++) {
		cout << "��" << i << "��:" << endl;
		Display(TheTrees[i], 0);
	}
}

/* չʾ����:չʾ�������е���Ϣ
 * ����ֵ:��
 * ����:Tree:��Ҫչʾ��Ϣ������index:�����±꣬1Ϊ������2Ϊ�ֵܽڵ�
 */
void B_queue::Display(BinTree Tree, int index) {
	if (Tree == NULL)
		return;

	if (index == 1)
		cout << "��";
	if (index == 2)
		cout << "��";
	cout << Tree->Element << endl;
	Display(Tree->LeftChild, 1); // �ݹ�չʾ�ӽڵ�
	Display(Tree->NextSibling, 2); // �ݹ�չʾ�ֵܽڵ�
}

/* ��Ϻ���:�����������������һ��
 * ����ֵ:BinTree:��Ϻ�Ķ�����
 * ����:T1:������ϵĶ�������T2:������ϵĶ�����
 */
BinTree B_queue::CombineTrees(BinTree T1, BinTree T2) {
	// �ж��Ƿ���Ҫ����
	if (T1->Element > T2->Element)
		return CombineTrees(T2, T1);
	
	// ��T2����T1��
	T2->NextSibling = T1->LeftChild;
	T1->LeftChild = T2;
	return T1;
}

/* �ϲ�����:��һ�����������ö��������һ��
 * ����ֵ:��
 * ����:other:������ϵĶ������
 */
void B_queue::Merge(B_queue &other) {
	// T1:�ö�����еĶ�����
	// T2:��һ��������еĶ�����
	// Carry:����ϳɵĶ�����
	BinTree T1, T2, Carry = NULL;
	int i, j;

	// �ж��Ƿ���Խ��кϲ�
	if (CurrentSize + other.CurrentSize > 2 * (1 << MaxSize) - 1) {
		cout << "���кϲ��󳬳�����!" << endl;
		return;
	}

	CurrentSize += other.CurrentSize; // ����Ԫ�ظ���
	// �������еĶ�����
	for (i = 0, j = 1; j <= CurrentSize; i++, j *= 2) {
		T1 = TheTrees[i], T2 = other.TheTrees[i];

		switch (!!T1 + 2 * !!T2 + 4 * !! Carry)
		{
			// ��û��
			case 0:
			// ֻ��T1
			case 1:
				break;
			// ֻ��T2
			case 2:
				TheTrees[i] = other.TheTrees[i];
				other.TheTrees[i] = NULL;
				break;
			// ��T1��T2
			case 3:
				Carry = CombineTrees(T1, T2);
				TheTrees[i] = other.TheTrees[i] = NULL;
				break;
			// ֻ��Carry
			case 4:
				TheTrees[i] = Carry;
				Carry = NULL;
				break;
			// ��T1��Carry
			case 5:
				Carry = CombineTrees(T1, Carry);
				TheTrees[i] = NULL;
				break;
			// ��T2��Carry
			case 6:
				Carry = CombineTrees(T2, Carry);
				other.TheTrees[i] = NULL;
				break;
			// ��T1��T2��Carry
			case 7:
				TheTrees[i] = Carry;
				Carry = CombineTrees(T1, T2);
				other.TheTrees[i] = NULL;
				break;
			default:
				break;
		}
	}
}

/* ���뺯��:���������в���ָ��Ԫ��
 * ����ֵ:��
 * ����:key:��Ҫ�����Ԫ��
 */
void B_queue::Insert(int key) {
	// �����µĶ������
	B_queue other(MaxSize);

	// ��ʼ��һ���µĶ��У�������һ��ָ��Ԫ��
	other.TheTrees[0] = new BinNode();
	other.TheTrees[0]->Element = key;
	other.TheTrees[0]->LeftChild = other.TheTrees[0]->NextSibling = NULL;
	other.CurrentSize = 1;

	// �ϲ��¶���
	Merge(other);
}

/* ɾ������:ɾ�������е���СԪ�أ���������ֵ
 * ����ֵ:int:�����е���Сֵ
 * ����:��
 */
int B_queue::DeleteMin() {
	// MinTree:��Сֵ���ڵ���
	// MinElement:�����е���Сֵ
	// DeletedQueue:��ɾ��������ɵ��¶���
	// DeletedTree:��ɾ����
	int MinTree;
	int MinElement;
	B_queue DeletedQueue;
	Position DeletedTree, oldRoot;

	MinElement = INT_MAX;
	// ��ȡ�����е���Сֵ�Լ������ڶ�����
	for (int i = 0; i < MaxSize; i++) {
		if (TheTrees[i] && TheTrees[i]->Element < MinElement) {
			MinTree = i;
			MinElement = TheTrees[i]->Element;
		}
	}

	// ɾ����СԪ�����ڵĽڵ�
	DeletedTree = TheTrees[MinTree];
	oldRoot = DeletedTree;
	DeletedTree = DeletedTree->LeftChild;
	delete oldRoot;
	oldRoot = NULL;

	// �����¶���
	DeletedQueue.CurrentSize = (1 << MinTree) - 1; // �����¶��е�Ԫ�ظ���
	for (int i = MinTree - 1; i >= 0; i--) {
		DeletedQueue.TheTrees[i] = DeletedTree;
		DeletedTree = DeletedTree->NextSibling;
		DeletedQueue.TheTrees[i]->NextSibling = NULL;
	}

	// �ö�����еĶ��ڶ������ÿ�
	TheTrees[MinTree] = NULL;
	CurrentSize -= DeletedQueue.CurrentSize + 1; // ���´�С

	// ���µĶ�����кϲ�
	Merge(DeletedQueue);

	// ������Сֵ
	return MinElement;
}