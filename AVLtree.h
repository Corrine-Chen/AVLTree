

#include<iostream>

using namespace std;


#include<iostream>
using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	AVLTreeNode(const K& key, const V& value)
		: _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _bf(0)
	{}

	AVLTreeNode<K, V>* _pLeft;
	AVLTreeNode<K, V>* _pRight;
	AVLTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	int _bf;    // 平衡因子
};


template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		: _pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		Node* pCur  = _pRoot;
		Node* parent=NULL;
		if(_pRoot == NULL)//树为空
		{
			_pRoot = new Node(key,value);
			return true;
		}
		else//树不为空
		{
			while(pCur)//寻找要插入的节点
			{
				if(pCur->_key > key)
				{
					parent = pCur;
					pCur = pCur->_pLeft;
				}
				else if(pCur->_key < key)
				{
					parent = pCur;
					pCur = pCur->_pRight;
				}
				else
				{
					return false;
				}
			}
		}
		 pCur = new Node(key,value);
        if(parent->_key < key)
        {
            parent->_pRight = pCur;
			pCur->_pParent = parent;
        }
        else
        {
            parent->_pLeft = pCur;
			pCur->_pParent = parent;
        }

		//更新平衡因子
		 while(parent)
        {
			if(parent->_pLeft == pCur)
                (parent->_bf)--;
            else
                (parent->_bf)++;

            if(parent->_bf == 0)
                return true;    
            else if(parent->_bf == -1 || parent->_bf == 1)
            {                                           
                pCur = parent;
				parent = pCur->_pParent;
            }
            else
            {
                if(parent->_bf == 2)//右子树
                {
                    if(pCur->_bf == 1)
                        _RotateL(parent);//左旋调整
                    else
                        _RotateRL(parent);//先右旋再左旋
                }
                else//左子树
                {
                    if(pCur->_bf == -1)
                        _RotateR(parent);
                    else
                        _RotateLR(parent);
                }
                break;
            }
        }
        return true;
	}

	void InOrder()
	{
		cout<<"OrInder: ";
		_InOrder(_pRoot);
		cout<<endl;
	}

	bool IsBalanceTree()
	{
		return _IsBalanceTree(_pRoot);
	}
private:
	bool _IsBalanceTree(Node* pRoot)
	{
		 if(pRoot == NULL)
            return true;
        size_t leftHeight = _Height(pRoot->_pLeft);
        size_t rightHeight = _Height(pRoot->_pRight);

        if(rightHeight-leftHeight != pRoot->_bf || abs(pRoot->_bf)>1)
        {
            cout<<pRoot->_key<<"-->"<<pRoot->_bf<<endl;
            return false;
        }
        return _IsBalanceTree(pRoot->_pLeft)&&_IsBalanceTree(pRoot->_pRight);
	}

	size_t _Height(Node* pRoot)
	{
		if(pRoot == NULL)
			return 0;

		if(pRoot->_pLeft ==NULL  && pRoot->_pRight ==NULL)
			return 1;

		size_t HeighL = _Height(pRoot->_pLeft); 
		size_t HeighR = _Height(pRoot->_pRight);

		return 1+(HeighL > HeighR ? HeighL : HeighR);
	}

	void _InOrder(Node* pRoot)
	{
		if(pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout<<pRoot->_key<<" ";
			_InOrder(pRoot->_pRight);
		}
	}

	void _RotateL(Node* parent)
	{
		Node* subR = parent->_pRight;
		Node* subRL = subR->_pLeft;
		parent->_pRight = subRL;
		if(subRL)
		{
			subRL->_pParent = parent;
		}
		subR->_pLeft = parent;
		Node* pPParent = parent->_pParent;
		parent->_pParent = subR;
		if(NULL == pPParent)
		{
			_pRoot = subR;
			subR->_pParent = NULL;
		}
		else
		{
			if(parent == pPParent->_pLeft)
			{
				pPParent->_pLeft = subR;
				subR->_pParent = pPParent;
			}
			else
			{
				pPParent->_pRight = subR;
				subR->_pParent  = pPParent;
			}
		}
		subR->_bf = 0;
		parent->_bf = 0;
	}
	void _RotateR(Node* parent)
	{
		Node* subL = parent->_pLeft;
		Node* subLR = subL->_pRight;
		parent->_pLeft = subLR;
		if(subLR)
		{
			subLR->_pParent = parent;
		}

		Node* pPParent = parent->_pParent;
		subL->_pRight = parent;
		parent->_pParent = subL;


		if(NULL == pPParent)
		{
			_pRoot = subL;
			 subL->_pParent = NULL;
		}
		else
		{
			if(parent == pPParent->_pLeft)
			{
				pPParent->_pLeft = subL;
				subL->_pParent = pPParent;
			}
			else
			{
				pPParent->_pRight = subL;
				subL->_pParent = pPParent;
			}
		}
		subL->_bf =0;
		parent->_bf =0;
	}
	void _RotateRL(Node* pParent)
	{
		Node* subR = pParent->_pRight;
        Node* subRL = subR->_pLeft;
        int bf = subRL->_bf;

        _RotateR(pParent->_pRight);
        _RotateL(pParent);

        //判断平衡因子应该如何更新
        if(bf == 1)
            pParent->_bf = -1;
        else
            subR->_bf = 1;
	}
	void _RotateLR(Node* pParent)
	{
		Node* subL = pParent->_pLeft;
		Node* subLR = subL->_pRight;
		int bf = subLR->_bf;

		_RotateL(pParent->_pLeft);
		_RotateR(pParent);

		if(bf == 1)
			pParent->_bf = -1;
		else
			subL->_bf =1;
	}
private:
	Node* _pRoot;
};


void TestAVL()
{
	//int array[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
	int array[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
	AVLTree<int,int> a;
	for(int i=0; i<sizeof(array)/sizeof(array[0]); ++i)
		a.Insert(array[i],i);

	a.InOrder();
	a.IsBalanceTree();




}