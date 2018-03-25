# 前言：
这次，将进行介绍的是一个特殊而有趣的队列——二项队列。在二项队列中你会发现人们是怎么把二进制的思想体现在数据结构中的；同时二项队列也是一种很高效的队列，对于合并、插入以及`DeleteMin`操作，它的最坏运行时间为`O(LogN)`，而插入平均花费常数时间进行。那么接下来开始我们的讲解吧~~  

首先还是写一个[我的CSDN博客](https://blog.csdn.net/weixin_41427400/article/details/79686215)传送门~~

# 原理：
首先我们得知道，二项队列并不是一颗堆序的树，而是堆序树的一个集合，也可以成为**森林**。而在森林中的每棵树，我们称之为**二项树**。二项树有如下的性质：  
1. 高度为0的二项树是一颗单节点的树；  
2. 高度为n的二项树`Bn`，通过将一颗二项树`B(n-1)`接到另一颗二项树`B(n-1)`上构成；
换句话来将，对于高度为n的树它的节点数为2的n次方；  
![](https://img-blog.csdn.net/20180325145223696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTQyNzQwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如上图所示，这就是二项树深林，而二项队列与普通的堆相同，都支持合并，插入以及DeleteMin操作；  

## 表现方式：
对于二项树，我们不使用一般的树的形式来对其进行储存，我们要求二项树的每一个节点包含数据，第一个儿子以及其右兄弟，同时保证其子节点元素一定小于父节点的元素。在离散数学中，这是一种很普遍的表达树的形式。
![](https://img-blog.csdn.net/20180325154011903?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTQyNzQwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 合并：  
对于二项队列来说合并可以说是最重要的操作，合并的目的是将两个二项队列进行合并，其中涉及到了单独两科二项树之间的合并以及森林(二项队列)之间的合并；对于二项树之间的合并是非常简单的，即先判断两颗二项树的头结点的大小，将元素较大的作为另外一颗树的新儿子，旧儿子则变为新儿子的右兄弟。  
![](https://img-blog.csdn.net/20180325154206467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTQyNzQwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码如下：  

		BinTree B_queue::CombineTrees(BinTree T1, BinTree T2) {  
		    // 判断是否需要反接  
		    if (T1->Element > T2->Element)  
		        return CombineTrees(T2, T1);  
		      
		    // 将T2接在T1上  
		    T2->NextSibling = T1->LeftChild;  
		    T1->LeftChild = T2;  
		    return T1;  
		}
可以看到合并两颗二项树所需要的操作是非常少的。那么如何进行森林的合并呢？这里，我们需要一点二进制的思想。我们先回忆一下，两个二进制数相加的情况：比如`00101+00011`的结果用二进制表示将会是`01000`，我们将从最低位开始运算，满二进一并向高位进位。而在二项树的合并中，我们也保留相同的思路，将有树看为`1`将没有树看为`0`，那么从高度最小的二项树之间开始合并，**树的合并也就等效成了二进制的加法**。
![](https://img-blog.csdn.net/20180325155844693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTQyNzQwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们可以发现上图中的二项队列合并可以看做二进制的加法`0011 + 0111 = 1010`；也就是说，**我们运用二进制的思想，就可以很好的解决二项队列的合并问题**！ 
代码如下：  

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
可以看到，在具体的代码中，**我们用了一个新的二项树节点来保存进位，以此来达到我们向二进制加法的转换**。同时巧妙的通过`switch`将8中情况给完美的进行了对应。(8种情况很好解释，应为2的三次方等于8)  

## 插入操作：
在我们完成了合并以后，插入操作就显得非常显而易见了。当我们想要插入某个元素的时候，我们只需重新生成一个只有目标元素的二项队列，然后将它与原来的二项队列进行合并，即可达到插入的目的！  
代码如下：
	
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

## 删除操作：
删除操作与插入相比要复杂一些，不过我们已经掌握了合并操作，所以对我们来说也不是什么难事了。删除要求我们删除队列中的最小元素并返回它的值，我们只需要遍历队列中每个树的树根，找到其中最小的元素，将该节点删除并返回其值即可。不过当删除一个树根节点后，会使其不满足二项队列的性质，因此，我们采取如下措施：  
1. 找到最小元素所在的二项树后，在一个新的队列H2中加入该二项树，同时在原队列中删除该二项树；
2. 在H2中删除最小的节点后，将二项树的每一个儿子作为新的二项树的树根；(如下图所示)
![](https://img-blog.csdn.net/20180325161439191?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTQyNzQwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3. 将H2队列与H1队列进行合并，即完成删除操作；  
代码如下：

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
那么整个二项队列到这里也就讲完啦，如果大家有什么疑问，或者我说的不好的地方欢迎指出，我一定会改正的~~

参考文献：《数据结构与算法分析——C语言描述》