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
#include "BSTree.h"
#include <iostream>

/**********************************************************************************/
/*!
  \brief 
    Allocates a new ObjectAllocator if ObjectAllocator does not have
    sharing enabled.

  \param OA
  	Pre-allocated objectallocator

  \param ShareOA
  	Flag for enableing ObjectAllocator Shareing
*/
/**********************************************************************************/
template <typename T>
BSTree<T>::BSTree(ObjectAllocator *OA, bool ShareOA)
{ 
    if (ShareOA)
    {
      objAllocator_ = OA; // Use rhs' allocator
      freeAllocator_ = false;            // We don't own it (won't free it)
      shareAllocator_ = ShareOA;            // If a copy of 'this object' is made, share the allocator
    }
    else // No sharing, create our own personal allocator
    {
      OAConfig config(true); // Set UseCPPMemManager to true, default the others
      objAllocator_ = new ObjectAllocator(sizeof(BinTreeNode), config);

      freeAllocator_ = true;   // We own the allocator, we will have to free it
      shareAllocator_ = ShareOA; // Do not share this allocator with any other list
   }

    get_root() = 0;
}  

/**********************************************************************************/
/*!
	\brief
		Copy Constructor

	\param tree
		The tree to make a copy of
*/
/**********************************************************************************/
template <typename T>
BSTree<T>::BSTree(const BSTree<T>& tree)
{
   if (tree.shareAllocator_)
    {
      objAllocator_ = tree.objAllocator_; // Use rhs' allocator
      freeAllocator_ = false;            // We don't own it (won't free it)
      shareAllocator_ = true;            // If a copy of 'this object' is made, share the allocator
    }
    else // No sharing, create our own personal allocator
    {
      OAConfig config(true); // Set UseCPPMemManager to true, default the others
      objAllocator_ = new ObjectAllocator(sizeof(BinTreeNode), config);

      freeAllocator_ = true;   // We own the allocator, we will have to free it
      shareAllocator_ = false; // Do not share this allocator with any other list
   }

    root_ = copy_tree(tree.root());
}

/**********************************************************************************/
/*!
	\brief
		Assigns one tree to another tree

	\param rhs
		The tree to assign
*/
/**********************************************************************************/
template <typename T>
BSTree<T>& BSTree<T>::operator=(const BSTree<T>& rhs)
{
	clear();
	root_ = copy_tree(rhs.root());

	return *this;
}

/**********************************************************************************/
/*!
	\brief
		Copies the tree given

  \param tree
    The tree to copy
*/
/**********************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::copy_tree(BSTree<T>::BinTree tree)
{
  if (tree == 0)
    return NULL;
  else
  {
    BinTree left = copy_tree( tree->left );
    BinTree right = copy_tree( tree->right );
    BinTree copy = make_node(tree->data);
    copy->left = left;
    copy->right = right;

    return copy;
  }
}

/**********************************************************************************/
/*!
  \brief
    Tree destructor
*/
/**********************************************************************************/
template <typename T>
BSTree<T>::~BSTree()
{
	clear();
  if(freeAllocator_)
    delete objAllocator_;
}

/**********************************************************************************/
/*!
  \brief
    No... I did not implement indexing -_-
    But thanks for rubbing it in
*/
/**********************************************************************************/
template <typename T>
bool BSTree<T>::ImplementedIndexing(void)
{
  return false;
}

/**********************************************************************************/
/*!
  \brief
    Create a new node using the ObjectAllocator and assign a given value to it

  \param value
    The value to set the new node to
*/
/**********************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::make_node(const T& value)
{
	try
	{
  		BSTree::BinTree tree = reinterpret_cast<BinTree>(objAllocator_->Allocate());
  		tree = new (tree) BinTreeNode(value); 
  		return tree;
  	}
  	catch(const OAException &e)
  	{
  		throw(BSTException(BSTException::E_NO_MEMORY, e.what()));
  	}
}

/**********************************************************************************/
/*!
  \brief 
    Frees a given node using the ObjectAllocator

  \param
    The node to be free'd
*/
/**********************************************************************************/
template <typename T>
void BSTree<T>::free_node(BSTree<T>::BinTree node)
{
	node->~BinTreeNode();
	objAllocator_->Free(node);
}

template <typename T>
void BSTree<T>::find_predecessor(BSTree<T>::BinTree tree, BSTree<T>::BinTree &predecessor) const
{
	predecessor = tree->left;
	while (predecessor->right != 0)
  		predecessor = predecessor->right;
}

/**********************************************************************************/
/*!
  \brief
    Inserts a node into the tree given a value

  \param value
    The value to insert
    
*/
/**********************************************************************************/
template <typename T>
void BSTree<T>::insert(const T& value)
{
  insert_item(root_, value);
}

/**********************************************************************************/
/*!
  \brief 
    The recursive auxilary fucntion for insert

  \param tree
    The tree the insert is to take place in

  \param value
    The value to be inserted
*/
/**********************************************************************************/
template <typename T>
void BSTree<T>::insert_item(BSTree<T>::BinTree &tree, const T &value)
{
	if (tree == 0)
   		tree = make_node(value);
	else if (value < tree->data)
    	insert_item(tree->left, value);
	else if (value > tree->data)
    	insert_item(tree->right, value);
	else
    	std::cout << "Error, duplicate item" << std::endl;
}

/**********************************************************************************/
/*!
  \brief
    Removes an element of a given value

  \param value
    The value to be removed
*/
/**********************************************************************************/
template <typename T>
void BSTree<T>::remove(const T& value)
{
	delete_item(root_, value);
}

/**********************************************************************************/
/*!
  \brief
    The helper auxilary function for remove

  \param tree
    The tree the delete is to occur in

  \param Data
    The value to be deleted
*/
/**********************************************************************************/
template <typename T>
void BSTree<T>::delete_item(BSTree<T>::BinTree &tree, const T& Data)
{
  if (tree == 0)
    return;
  else if (Data < tree->data)
    delete_item(tree->left, Data);
  else if (Data > tree->data)
    delete_item(tree->right, Data);
  else // (Data == tree->data)
  {
    if (tree->left == 0)
    {
      BinTree temp = tree;
      tree = tree->right;
      free_node(temp);
    }
    else if (tree->right == 0)
    {
      BinTree temp = tree;
      tree = tree->left;
      free_node(temp);
    }
    else
    {
      BinTree pred = 0;
      find_predecessor(tree, pred);
      tree->data = pred->data;
      delete_item(tree->left, tree->data);
    }
  }
}

/*!
 * @brief      Clears the tree
 */
template <typename T>
void BSTree<T>::clear(void)
{
  clear_tree(root_);
}

/*!
 * @brief      The helper auxilary function for clear
 *
 * @param      tree  The tree to clear
 */
template <typename T>
void BSTree<T>::clear_tree(BSTree<T>::BinTree &tree)
{
	if(!root_)
		return;

 	while(tree->right)
  		clear_tree(tree->right);

	while(tree->left)
    	clear_tree(tree->left);

 	remove(tree->data);
}

/*!
 * @brief      Finds an element based on a given value. Also tracks the
 *             number of comparisions
 *
 * @param[in]  value     The value being searched for
 * @param      compares  The number of comparisons made
 *
 * @return     The result of the search
 */
template <typename T>
bool BSTree<T>::find(const T& value, unsigned &compares) const
{
	bool result = find_item(root_, value, compares);
	return result;
}

/*!
 * @brief      The helper auxilary function for find
 *
 * @param      tree      The tree being searched
 * @param[in]  Data      The value being searched for
 * @param      compares  The number of comparisions it took
 *
 * @return     The result of the search
 */
template <typename T>
bool BSTree<T>::find_item(BSTree<T>::BinTree const &tree, const T& Data, unsigned &compares) const
{
  if (tree == 0)
  {
    compares++;
  	return false;
  }
  else if (Data == tree->data)
  {
    compares++;
  	return true;
  }
  else if (Data < tree->data)
  {
    compares++;
  	return find_item(tree->left, Data, compares);
  }
  else
  {
    compares++;
  	return find_item(tree->right, Data, compares);
  }
}

/*!
 * @brief      Checks if the tree is empty
 *
 * @return     Result of the check
 */
template <typename T>
bool BSTree<T>::empty(void) const
{
	if(root_ == NULL)
		return true;
	else
		return false;
}

/*!
 * @brief      G
 *
 * @tparam     T     { description }
 *
 * @return     { description_of_the_return_value }
 */
template <typename T>
unsigned int BSTree<T>::size(void) const
{
	return tree_size(root_);
}

/*!
 * @brief      Calculates the size of the tree
 *
 * @param[in]  tree  The tree being checked
 *
 * @return     The size of the tree
 */
template <typename T>
unsigned int BSTree<T>::tree_size(BSTree<T>::BinTree tree) const
{
	if (tree == 0)
    return 0;
  else
    return 1 + tree_size(tree->left) + tree_size(tree->right);
}

/*!
 * @brief      Calculates the height of the tree
 *
 * @param[in]  tree  The tree being checked
 *
 * @return     The size of the tree
 */
template <typename T>
int BSTree<T>::height(void) const
{
	return tree_height(root_);
}

/*!
 * @brief      The helper auxilary function for height
 *
 * @param[in]  tree  The tree we are checking
 *
 * @return     The height of the tree
 */
template <typename T>
int BSTree<T>::tree_height(BSTree<T>::BinTree tree) const
{
  	if (tree == 0)
    	return -1;

  	if (tree_height(tree->left) > tree_height(tree->right))
    	return tree_height(tree->left) + 1;
  	else
  		return tree_height(tree->right) + 1;
}

/*!
 * @brief      Returns the root of the tree
 *
 * @return     The root of the tree
 */
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::root(void) const
{
	return root_;
}

/*!
 * @brief      Returns the root of the tree
 *
 * @return     The root of the tree
 */
template <typename T>
typename BSTree<T>::BinTree& BSTree<T>::get_root(void)
{
  return root_;
}