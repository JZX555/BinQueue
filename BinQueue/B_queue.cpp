#include "stdafx.h"
#include "B_queue.h"

/* 构造函数:初始化对象
 * 返回值:无
 * 参数:size:二项队列的阶数
 */
B_queue::B_queue(int size)
: MaxSize(size){
	// 初始化二项队列
	CurrentSize = 0;
	TheTrees = new BinTree[MaxSize];
	for (int i = 0; i < MaxSize; i++)
		TheTrees[i] = NULL;
}

/* 析构函数:对象消亡时回收存储空间
 * 返回值:无
 * 参数:无
 */
B_queue::~B_queue()
{
	MakeEmpty(); // 置空二项队列

	// 删除森林
	delete TheTrees;
	TheTrees = NULL;
}

/* 置空函数:将二项队列置空
 * 返回值:无
 * 参数:无
 */
void B_queue::MakeEmpty() {
	// 将森林中的每棵树置空
	for (int i = 0; i < MaxSize; i++) 
		MakeEmpty(TheTrees[i]);
}

/* 置空函数:将二项树置空
 * 返回值:无
 * 参数:无
 */
void B_queue::MakeEmpty(BinTree Tree) {
	if (Tree == NULL)
		return;

	MakeEmpty(Tree->LeftChild); // 递归置空子树
	MakeEmpty(Tree->NextSibling); // 递归置空兄弟节点

	// 删除当前节点
	delete Tree;
	Tree = NULL;
}

/* 展示函数:展示二项队列中的信息
 * 返回值:无
 * 参数:无
 */
void B_queue::Display() {
	// 展示森林中的所以信息
	for (int i = 0; i < MaxSize; i++) {
		cout << "第" << i << "阶:" << endl;
		Display(TheTrees[i], 0);
	}
}

/* 展示函数:展示二项树中的信息
 * 返回值:无
 * 参数:Tree:想要展示信息的树；index:辅助下标，1为子树，2为兄弟节点
 */
void B_queue::Display(BinTree Tree, int index) {
	if (Tree == NULL)
		return;

	if (index == 1)
		cout << "子";
	if (index == 2)
		cout << "兄";
	cout << Tree->Element << endl;
	Display(Tree->LeftChild, 1); // 递归展示子节点
	Display(Tree->NextSibling, 2); // 递归展示兄弟节点
}

/* 组合函数:将两个二项树组合在一起
 * 返回值:BinTree:组合后的二项树
 * 参数:T1:参与组合的二项树；T2:参与组合的二项树
 */
BinTree B_queue::CombineTrees(BinTree T1, BinTree T2) {
	// 判断是否需要反接
	if (T1->Element > T2->Element)
		return CombineTrees(T2, T1);
	
	// 将T2接在T1上
	T2->NextSibling = T1->LeftChild;
	T1->LeftChild = T2;
	return T1;
}

/* 合并函数:另一个二项队列与该队列组合在一起
 * 返回值:无
 * 参数:other:参与组合的二项队列
 */
void B_queue::Merge(B_queue &other) {
	// T1:该二项队列的二项树
	// T2:另一个二项队列的二项树
	// Carry:额外合成的二项树
	BinTree T1, T2, Carry = NULL;
	int i, j;

	// 判断是否可以进行合并
	if (CurrentSize + other.CurrentSize > 2 * (1 << MaxSize) - 1) {
		cout << "队列合并后超出长度!" << endl;
		return;
	}

	CurrentSize += other.CurrentSize; // 更新元素个数
	// 检索所有的二项树
	for (i = 0, j = 1; j <= CurrentSize; i++, j *= 2) {
		T1 = TheTrees[i], T2 = other.TheTrees[i];

		switch (!!T1 + 2 * !!T2 + 4 * !! Carry)
		{
			// 都没有
			case 0:
			// 只有T1
			case 1:
				break;
			// 只有T2
			case 2:
				TheTrees[i] = other.TheTrees[i];
				other.TheTrees[i] = NULL;
				break;
			// 有T1和T2
			case 3:
				Carry = CombineTrees(T1, T2);
				TheTrees[i] = other.TheTrees[i] = NULL;
				break;
			// 只有Carry
			case 4:
				TheTrees[i] = Carry;
				Carry = NULL;
				break;
			// 有T1和Carry
			case 5:
				Carry = CombineTrees(T1, Carry);
				TheTrees[i] = NULL;
				break;
			// 有T2和Carry
			case 6:
				Carry = CombineTrees(T2, Carry);
				other.TheTrees[i] = NULL;
				break;
			// 有T1，T2和Carry
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

/* 插入函数:向二项队列中插入指定元素
 * 返回值:无
 * 参数:key:想要插入的元素
 */
void B_queue::Insert(int key) {
	// 申请新的二项队列
	B_queue other(MaxSize);

	// 初始化一个新的队列，并加入一个指定元素
	other.TheTrees[0] = new BinNode();
	other.TheTrees[0]->Element = key;
	other.TheTrees[0]->LeftChild = other.TheTrees[0]->NextSibling = NULL;
	other.CurrentSize = 1;

	// 合并新队列
	Merge(other);
}

/* 删除函数:删除队列中的最小元素，并返回其值
 * 返回值:int:队列中的最小值
 * 参数:无
 */
int B_queue::DeleteMin() {
	// MinTree:最小值所在的树
	// MinElement:队列中的最小值
	// DeletedQueue:被删除树所组成的新队列
	// DeletedTree:被删除树
	int MinTree;
	int MinElement;
	B_queue DeletedQueue;
	Position DeletedTree, oldRoot;

	MinElement = INT_MAX;
	// 获取队列中的最小值以及其所在二项树
	for (int i = 0; i < MaxSize; i++) {
		if (TheTrees[i] && TheTrees[i]->Element < MinElement) {
			MinTree = i;
			MinElement = TheTrees[i]->Element;
		}
	}

	// 删除最小元素所在的节点
	DeletedTree = TheTrees[MinTree];
	oldRoot = DeletedTree;
	DeletedTree = DeletedTree->LeftChild;
	delete oldRoot;
	oldRoot = NULL;

	// 更新新队列
	DeletedQueue.CurrentSize = (1 << MinTree) - 1; // 设置新队列的元素个数
	for (int i = MinTree - 1; i >= 0; i--) {
		DeletedQueue.TheTrees[i] = DeletedTree;
		DeletedTree = DeletedTree->NextSibling;
		DeletedQueue.TheTrees[i]->NextSibling = NULL;
	}

	// 该二项队列的对于二项树置空
	TheTrees[MinTree] = NULL;
	CurrentSize -= DeletedQueue.CurrentSize + 1; // 更新大小

	// 与新的二项队列合并
	Merge(DeletedQueue);

	// 返回最小值
	return MinElement;
}