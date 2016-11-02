//---------------------------------------------------------------------------
#ifndef AVLTREE_H
#define AVLTREE_H
//---------------------------------------------------------------------------
#include <stack>
#include "BSTree.h"

template <typename T>
class AVLTree : public BSTree<T>
{
  public:
  	typedef typename BSTree<T>::BinTreeNode* BinTree; 
    AVLTree(ObjectAllocator *OA = 0, bool ShareOA = false);
    virtual ~AVLTree();
    virtual void insert(const T& value);
    virtual void remove(const T& value);
    
  private:
  	void BalanceAVLTree(std::stack<BinTree *>& nodes);
	  void RotateLeft (BinTree &tree);
	  void RotateRight(BinTree &tree);
  	void insert_item(BinTree &tree, const T& Data, std::stack<BinTree*>& nodes);
  	void delete_item(BinTree &tree, const T& Data, std::stack<BinTree*>& nodes);
};

#include "AVLTree.cpp"

#endif
//---------------------------------------------------------------------------
