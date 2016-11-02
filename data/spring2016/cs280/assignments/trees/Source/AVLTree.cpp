/****************************************************************************/
/*!
\file   
\author Damien Baca
\par    email: damien.baca@digipen.edu
\par    DigiPen login: damien.baca
\par    Course: CS280
\par    Assignment #4
\date   
\brief  
*/
/****************************************************************************/
#include "AVLTree.h"
#include <stdlib.h>
#include <iostream>

/*!
 * @brief      Calls the BSTree constructor
 *
 * @param      OA       The Object allocator being shared
 * @param[in]  ShareOA  If we want to enable sharing
 */
template <typename T>
AVLTree<T>::AVLTree(ObjectAllocator *OA, bool ShareOA) : BSTree<T>(OA, ShareOA)
{
}

/*!
 * @brief      No OP
 */
template <typename T>
AVLTree<T>::~AVLTree()
{
}

/*!
 * @brief      Inserts into an AVL Tree
 *
 * @param[in]  value  Value to insert
 */
template <typename T>
void AVLTree<T>::insert(const T& value)
{
	std::stack<BinTree *> nodes;
	insert_item(BSTree<T>::get_root(), value, nodes);
}

/*!
 * @brief      The helper auxilary function for insert
 *
 * @param      tree   The tree we are inserting into
 * @param[in]  Data   The value we are inserting
 * @param      nodes  The stack of visited nodes
 */
template <typename T> 
void AVLTree<T>::insert_item(typename AVLTree<T>::BinTree &tree, const T& Data, std::stack<typename AVLTree::BinTree*>& nodes)
{
  if (tree == 0)
  {
    tree = BSTree<T>::make_node(Data);
    BalanceAVLTree(nodes); // Balance it now
  }
  else if (Data < tree->data)
  {
    nodes.push(&tree); // save visited node
    insert_item(tree->left, Data, nodes);
  }
  else if (Data > tree->data)
  {
    nodes.push(&tree); // save visited node
    insert_item(tree->right, Data, nodes);
  }
  else
    std::cout << "Error, duplicate item" << std::endl;
}

/*!
 * @brief      Rotates the tree to the left about the give node
 *
 * @param      tree  The tree being deleted
 */
template <typename T>
void AVLTree<T>::RotateLeft(typename AVLTree<T>::BinTree &tree)
{
  BinTree temp = tree;
  tree = tree->right;
  temp->right = tree->left;
  tree->left = temp;
}

/*!
 * @brief      Rotates the tree to the right about the give node
 *
 * @param      tree  The tree being deleted
 */
template <typename T>
void AVLTree<T>::RotateRight(typename AVLTree<T>::BinTree &tree)
{
  BinTree temp = tree;
  tree = tree->left;
  temp->left = tree->right;       
  tree->right = temp;
}

/*!
 * @brief      Balance the tree
 *
 * @param      nodes  The stack of visited nodes
 */
template <typename T>
void AVLTree<T>::BalanceAVLTree(std::stack<typename AVLTree<T>::BinTree *>& nodes)
{
  while (!nodes.empty())
  {
    BinTree *node = nodes.top();
    nodes.pop();
    
    //std::cout << "DIFF[" << (*node)->data<<"] : "<<abs(BSTree<T>::tree_height((*node)->left) - BSTree<T>::tree_height((*node)->right)) << std::endl;
    if(abs(BSTree<T>::tree_height((*node)->left) - BSTree<T>::tree_height((*node)->right)) <= 1) 
    	continue;
    
    /*
    y = (*node)
    u = (*node)->right
    v = (*node)->right->left
    w = (*node)->right->right
    */
    if(BSTree<T>::tree_height((*node)->right) > BSTree<T>::tree_height((*node)->left))
    {
    	if((*node)->right)
    	{
    		//if zig-zig
    		if(BSTree<T>::tree_height((*node)->right->left) > BSTree<T>::tree_height((*node)->right->right))
    		  RotateRight((*node)->right);
    		RotateLeft(*node);
      }
    }
    /*
    y = (*node)
    u = (*node)->left
    v = (*node)->left->left
    w = (*node)->left->right
    */
    if(BSTree<T>::tree_height((*node)->right) < BSTree<T>::tree_height((*node)->left))
    {
    	if((*node)->left)
    	{
    		//if zig-zig
    		if(BSTree<T>::tree_height((*node)->left->left) < BSTree<T>::tree_height((*node)->left->right))
    		  RotateLeft((*node)->left);
    		RotateRight(*node);
      }
    }
  }
}

/*!
 * @brief      Removes an element from an AVL tree by value
 *
 * @param[in]  value  The value to be removed
 */
template <typename T>
void AVLTree<T>::remove(const T& value)
{
	std::stack<BinTree *> nodes;
	delete_item(BSTree<T>::get_root(), value, nodes);
}

/*!
 * @brief      The helper auxilary function for remove
 *
 * @param      tree   The tree you are removing from
 * @param[in]  Data   The value being removed
 * @param      nodes  The stack of nodes visited
 */
template <typename T>
void AVLTree<T>::delete_item(typename AVLTree<T>::BinTree &tree, const T& Data, std::stack<typename AVLTree<T>::BinTree*>& nodes)
{
  if (tree == 0)
    return;
  else if (Data < tree->data)
  {
  	nodes.push(&tree); // save visited node
    delete_item(tree->left, Data, nodes);
  }
  else if (Data > tree->data)
  {
  	nodes.push(&tree); // save visited node
    delete_item(tree->right, Data, nodes);
  }
  else // (Data == tree->data)
  {
    if (tree->left == 0)
    {
      BinTree temp = tree;
      tree = tree->right;
      BSTree<T>::free_node(temp);
    }
    else if (tree->right == 0)
    {
      BinTree temp = tree;
      tree = tree->left;
      BSTree<T>::free_node(temp);
    }
    else
    {
      BinTree pred = 0;
      BSTree<T>::find_predecessor(tree, pred);
      tree->data = pred->data;
      delete_item(tree->left, tree->data, nodes);
    }
    BalanceAVLTree(nodes);
  }
}
