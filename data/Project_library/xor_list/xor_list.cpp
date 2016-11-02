#include <iostream>
#include <stdlib.h>
#include <stdint.h>

template <typename T>
xor_list<T>::xor_list() : head_(NULL), tail_(NULL), size_(0) {}

template <typename T>
xor_list<T>::~xor_list()
{
	xor_node* curr = head_;
	xor_node* prev = NULL;
	xor_node* next;
	while(curr != 0) 
	{
		next = XOR(prev, curr->ptx);
		delete prev;
		prev = curr;
		curr = next;
	}
	delete prev;
}

template <typename T>
int xor_list<T>::push_front(T* data)
{
	xor_node* newnode = makeNode(data);

	if(head_ == NULL) 
	{
		  // In the event of an empty list, the head becomes the new node
		head_ = newnode;
		tail_ = newnode;
	} 
	else 
	{	  
		  // Former First  Node = FFN
		  // Former Second Node = FSN
		  // Newnode's ptx set: (NULL) <-- [newnode] --> (FFN)
	    newnode->ptx = XOR(NULL, head_);
	      // Former first node's ptx set: (newnode) <-- [FFN] --> (FSN)
	    head_->ptx = XOR(newnode, XOR(head_->ptx, NULL)); 
	}
	  // Newnode becomes new first node
	head_ = newnode;	

	return 1;
}

template <typename T>
int xor_list<T>::push_back(T* data)
{
	xor_node* newnode = makeNode(data);
	xor_node* curr = head_;
	xor_node* prev = NULL;
	xor_node* next = XOR(curr->ptx, prev);

	while(next != NULL)
	{
		prev = curr;
		curr = next;
		next = XOR(curr->ptx, prev);
	}
	curr->ptx = XOR(prev, newnode);
	newnode->ptx = XOR(curr, NULL);
	tail_ = newnode;

	return 1;
}

template <typename T>
int xor_list<T>::pop_front()
{
	if(head_ == NULL)
		return 0;
    if(head_ == tail_)
    {
    	delete head_;
    	head_ = NULL;
    	tail_ = NULL;

    	return 1;
    }

	xor_node* prev = head_;
	xor_node* curr = XOR(prev->ptx, NULL);
	xor_node* next = XOR(curr->ptx, prev);

	head_ = curr;
	delete prev;
	curr->ptx = XOR(NULL, next);

	return 1;
}

template <typename T>
int xor_list<T>::pop_back()
{
	if(head_ == NULL)
		return 0;
	if(head_ == tail_)
    {
    	delete head_;
    	head_ = NULL;
    	tail_ = NULL;

    	return 1;
    }

	xor_node* newlast = XOR(tail_->ptx, NULL);
	xor_node* prev = XOR(newlast->ptx, tail_);

	delete tail_;
	newlast->ptx = XOR(prev, NULL);
	tail_ = newlast;

	return 1;
}

template <typename T>
int xor_list<T>::size()
{
    return size_;
}

template <typename T>
int xor_list<T>::insert(T* data, int pos)
{
	if(pos == 0)
		push_front(data);
	if(pos => size())
		push_back(data);

	xor_node* prev = NULL;
	xor_node* curr = head_;
	xor_node* next = xor(curr->ptx, prev);

	for(int i = 0; i < pos; i++)
	{
		prev = curr;
		curr = next;
		next = xor(curr->ptx, prev);
	}

	
}

template <typename T>
int xor_list<T>::displayList(const char* msg)
{
	xor_node* curr = head_;
	xor_node* prev = NULL;
	xor_node* next;

	if(head_ == NULL && tail_ == NULL)
	{
		std::cout << "[ Empty List ]" << std::endl;
		std::cout << "----------------------------------------------------------" << std::endl << std::endl;

		return 1;
	}

	std::cout << "HEAD/NULL";
	while(curr != NULL) 
	{
		std::cout << " -- " << *(curr->data_);
		next = XOR(prev, curr->ptx);
		prev = curr;
		curr = next;
	}
	std::cout << " -- TAIL/NULL "<< std::endl;
	std::cout << "----------------------------------------------------------" << std::endl << std::endl;

	return 1;
}

// Private //////////////////////////////////
/////////////////////////////////////////////

template <typename T>
typename xor_list<T>::xor_node* xor_list<T>::XOR(xor_list<T>::xor_node* p1, xor_list<T>::xor_node* p2)
{
  return (xor_node*)((uintptr_t)p1 ^ (uintptr_t)p2);
}

template <typename T>
typename xor_list<T>::xor_node* xor_list<T>::makeNode(T* data)
{
	xor_node* newnode = new xor_node;
	newnode->data_ = data;
	newnode->ptx = NULL;

	return newnode;
}
