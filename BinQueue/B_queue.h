#ifndef B_QUEUE_H
#define B_QUEUE_H

#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// �������������ڵ�
typedef struct BinNode *Position;
typedef struct BinNode *BinTree;

/* �������ڵ�
 * ����Ԫ��:
 * Element:�ڵ��д洢��Ԫ��
 * LeftChild:�ڵ��������
 * NextSibling:�ڵ���ֵܽڵ�
 */
struct BinNode {
	int Element;
	Position LeftChild;
	Position NextSibling;
};

/* B_queue��(�������)
 * �ӿ�:
 * B_queue(int):���캯�����ö��е�������
 * Insert:���빦�ܣ����������в���һ��ָ��Ԫ��
 * DeleteMin:ɾ�����ܣ�ɾ�������е���СԪ����������ֵ
 * MakeEmpty:�ÿչ��ܣ�����������ÿ�
 * Display:չʾ���ܣ�չʾ��������е�Ԫ��
 */
class B_queue
{
public:
	// ���캯��
	B_queue(int = 10);
	//��������
	~B_queue();

	// �ӿں���
	void Insert(int);
	int DeleteMin();
	void MakeEmpty();
	void Display();
	
private:
	// �ϳɺ���
	void Merge(B_queue &);
	BinTree CombineTrees(BinTree, BinTree);

	// ��������
	void MakeEmpty(BinTree);
	void Display(BinTree, int);

	int MaxSize; // ���������еĽ���
	int CurrentSize; //���������д���Ԫ�ظ���
	BinTree *TheTrees; // ������ɭ��
};

#endif