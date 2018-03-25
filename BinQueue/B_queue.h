#ifndef B_QUEUE_H
#define B_QUEUE_H

#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// 重命名二项树节点
typedef struct BinNode *Position;
typedef struct BinNode *BinTree;

/* 二项树节点
 * 储存元素:
 * Element:节点中存储的元素
 * LeftChild:节点的左子树
 * NextSibling:节点的兄弟节点
 */
struct BinNode {
	int Element;
	Position LeftChild;
	Position NextSibling;
};

/* B_queue类(二项队列)
 * 接口:
 * B_queue(int):构造函数，该队列的最大阶数
 * Insert:插入功能，向二项队列中插入一个指定元素
 * DeleteMin:删除功能，删除队列中的最小元，并返回其值
 * MakeEmpty:置空功能，将二项队列置空
 * Display:展示功能，展示二项队列中的元素
 */
class B_queue
{
public:
	// 构造函数
	B_queue(int = 10);
	//析构函数
	~B_queue();

	// 接口函数
	void Insert(int);
	int DeleteMin();
	void MakeEmpty();
	void Display();
	
private:
	// 合成函数
	void Merge(B_queue &);
	BinTree CombineTrees(BinTree, BinTree);

	// 辅助函数
	void MakeEmpty(BinTree);
	void Display(BinTree, int);

	int MaxSize; // 储存二项队列的阶数
	int CurrentSize; //储存二项队列储存元素个数
	BinTree *TheTrees; // 二项树森林
};

#endif