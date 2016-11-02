/******************************************************************************/
/*!
\file   BList.cpp
\author Damien Paige Baca
\par    email: Gamerlios123@gmail.com
\par    DigiPen login: damien.baca
\par    Course: CS280
\par    Assignment #2
\date   1/23/2016

\brief  
  This is the implementation file for the BList class.

  Operations include:

  [Complete] - Insert element in the back
  [Complete] - Insert element in the fornt
  [] - Insert into sorted list
  [] - Remove by index
  [] - Remove by value
  [Complete] - Index operator [] support
  [Complete] - Return number of elements total
  [Complete] - Clear the list

*/
/******************************************************************************/

template <typename T, int Size>
BList<T, Size>::BList()
{
  head_ = NULL;
  tail_ = NULL;

  BListStats stats;
  stats_ = &stats;

  sorted = true;
}   

/////////////////////////////////////////////////////////////////////////////////
// Copy Constructor                                                            //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>                             
BList<T, Size>::BList(const BList<T, Size>&rhs)
{
  BList<T, Size>::BNode *ptr = rhs.head_;

  BListStats stats;
  stats_ = &stats;

  while(ptr)
  {
  	// Nodes will be partially full so 'Size' cannot be used
    for(int i = 0; i < (ptr->count); i++)
      push_back(ptr->values[i]);

    ptr = ptr->next;
  }
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor                                                                  //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
BList<T, Size>::~BList()
{
  BList<T, Size>::BNode *ptr = head_;
  BList<T, Size>::BNode *del = head_;

  while(del)
  {
  	ptr = ptr->next;
  	delete del;
  	del = ptr;
  }
}   

/////////////////////////////////////////////////////////////////////////////////
// Size of List                                                                //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size> 
int BList<T, Size>::size(void) const
{
  BList<T, Size>::BNode *ptr = head_;
  int size = 0;

  while(ptr)
  {
    size += ptr->count;
    ptr = ptr->next;
  }
  return size;
}

/////////////////////////////////////////////////////////////////////////////////
// Clear List                                                                  //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size> 
void BList<T, Size>::clear(void)
{
  BList<T, Size>::BNode *ptr = head_;
  BList<T, Size>::BNode *del = head_;

  while(del)
  {
  	ptr = ptr->next;
  	delete del;
  	del = ptr;
  }

  head_ = NULL;
}   

/////////////////////////////////////////////////////////////////////////////////
// Get Stats                                                                   //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size> 
BListStats BList<T, Size>::GetStats() const
{
  BList<T, Size>::BNode *ptr = head_;
  int count = 0;

  stats_->NodeSize = nodesize();
  stats_->ArraySize = Size;
  stats_->ItemCount = size();

  while(ptr)
  {
  	ptr = ptr->next;
  	count++;
  }
  stats_->NodeCount = count;

  return *stats_;
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor                                                                  //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
size_t BList<T, Size>::nodesize(void)
{	
  return sizeof(BNode);
}


/////////////////////////////////////////////////////////////////////////////////
// Destructor                                                                  //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
const typename BList<T, Size>::BNode* BList<T, Size>::GetHead() const
{
  return head_;	
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor                                                                  //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>                        
BList<T, Size>& BList<T, Size>::operator=(const BList<T, Size> &rhs)
{
  
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor                                                                  //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
int BList<T, Size>::find(const T& value) const
{
  BList<T, Size>::BNode *ptr = head_;
  int index = 0;
  int nodeindex = 0;
    
  while(ptr)
  {
  	if(sorted)
  		nodeindex = BinarySearch(ptr->values, ptr->count, value);
  	if(!sorted)
  		nodeindex = LinearSearch(ptr->values, ptr->count, value);

    if(nodeindex != -1)
    	return (index + nodeindex);

    index += ptr->count;
    ptr = ptr->next;
  }
  return -1;

}   

///////////////////////////////////////////////////////////////////////////////////
// Push Back                                                                     //
/*/////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  Function : void push_back(int value)                                         //
//                                                                               //
//  Breif    : Add an element to the end of a Blist object in the order given    //
//                                                                               //
*//////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
void BList<T, Size>::push_back(const T& value)
{
/*
  Mark the list as unsorted
*/
  sorted = false;

/*
  If (The List Is Empty)
    Point head to a new node
    Update tail to node
    Increment the count of the node
*/
  if(head_ == NULL && tail_ == NULL)
  {
  	head_ = new_node();
  	head_->values[0] = value;
  	tail_ = head_;
  	head_->count++;
  	return;
  }

/*
  If (There Is Room In The Tail Node)
    Set the end index equal to value
  If (The Node Is Full)
    Point tail->next to a new node
    Point newnode's prev to tail
    Set the tail to new tail node
  Increment the count of the node
*/
  if(tail_->count < Size)
  {
  	// Yes, fill it
  	tail_->values[tail_->count] = value;
  }
  else
  {
  	// No, make a new node
    tail_->next = new_node();
    tail_->next->values[0] = value;
    tail_->next->prev = tail_;
    tail_ = tail_->next; 
  }
  tail_->count++;

  return;
}

///////////////////////////////////////////////////////////////////////////////////
// Push Front                                                                    //
/*/////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  Function : void push_front(int value)                                        //
//                                                                               //
//  Breif    : Add an element to the start of a Blist object in the order given  //
//                                                                               //
*//////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
void BList<T, Size>::push_front(const T& value)
{
/*
  Mark the list as unsorted
*/
  sorted = false;

/*
  If (The List Is Empty)
    Point head to a new node
    Update tail to node
    Increment the count of the node
*/
  if(head_ == NULL && tail_ == NULL)
  {
  	head_ = new_node();
  	head_->values[0] = value;
  	tail_ = head_;
  	head_->count++;
  	return;
  }

/*
  If (There Is Room In The Head Node)
    Shift all values over one
    Set the first element to value
  If (The Node Is Full)
    Point current heads prev to a new node
    Point newnode's next to current head
    Set the head to new head node
  Increment the count of the node

*/
  if(head_->count < Size)
  {
  	// Yes, shift everything and fill it
  	for(int i = head_->count - 1; i >= 0; i--)
  		head_->values[i + 1] = head_->values[i];
  	head_->values[0] = value;
  }
  else
  {
  	// No, make a new node
  	head_->prev = new_node();
  	head_->prev->values[0] = value;
  	head_->prev->next = head_;
  	head_ = head_->prev;
  }
  head_->count++;
 }

///////////////////////////////////////////////////////////////////////////////////
// Push Front                                                                    //
/*/////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  Function : void push_front(int value)                                        //
//                                                                               //
//  Breif    : Add an element to the start of a Blist object in the order given  //
//                                                                               //
*//////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
void BList<T, Size>::insert(const T& value)
{
  BList<T, Size>::BNode *ptr = head_;
  BList<T, Size>::BNode *split = NULL;
  
  if(head_ == NULL)
  {
    head_ = new_node();
    head_->values[0] = value;
    head_->count++;
    return;
  }

  while(ptr->next)
  {
  	if(ptr->next->values[0] < value)
  	    ptr = ptr->next;
  	else
  	{
      if(ptr->count < Size) // There is room
      {
        Insert_Array(ptr->values, ptr->count, value);
        ptr->count++;
        return;
      }
      else // There is no room
      {
        split = split_node(ptr);
        if(value < ptr->values[0])
        {
          Insert_Array(split->values, split->count, value);
          split->count++;
          return;
        }
        else
        {
          Insert_Array(ptr->values, ptr->count, value);
          ptr->count++;
          return;
        }
      }
    }
  }
  if(ptr->count < Size) // There is room
  {
    Insert_Array(ptr->values, ptr->count, value);
    ptr->count++;
    return;
  }
  else
  {
  	split = split_node(ptr);
    if(value < ptr->values[0])
    {
      Insert_Array(split->values, split->count, value);
      split->count++;
      return;
    }
    else
    {
      Insert_Array(ptr->values, ptr->count, value);
      ptr->count++;
      return;
    }
  }
}


template <typename T, int Size>
void BList<T, Size>::remove(int )
{

}


template <typename T, int Size>
void BList<T, Size>::remove_by_value(const T& )
{

} 
   
template <typename T, int Size> 
T& BList<T, Size>::operator[](int index)
{
  BList<T, Size>::BNode *ptr = head_;

  while(ptr)
  {
    for(int i = 0; i < ptr->count; i++)
    {
      if(index < ptr->count)
      	return ptr->values[index];
    }
    index -= ptr->count;
    ptr = ptr->next;
  }
  return ptr->values[index];
}   

     
template <typename T, int Size>      
const T& BList<T, Size>::operator[](int index) const
{
  BList<T, Size>::BNode *ptr = head_;

  while(ptr)
  {
    for(int i = 0; i < ptr->count; i++)
    {
      if(index < ptr->count)
      	return ptr->values[index];
    }
    index -= ptr->count;
    ptr = ptr->next;
  }
  return ptr->values[index];
} 

template <typename T, int Size> 
typename BList<T, Size>::BNode *BList<T, Size>::new_node()
{
  BList<T, Size>::BNode *temp = new BList<T, Size>::BNode;
  return temp;
}

///////////////////////////////////////////////////////////////////////////////////
// Split Node                                                                    //
/*/////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  Function : void push_back(int value)                                         //
//                                                                               //
//  Breif    : Add an element to the end of a Blist object in the order given    //
//                                                                               //
*//////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size> 
typename BList<T, Size>::BNode *BList<T, Size>::split_node(BList<T, Size>::BNode *A_node)
{
/*
  Create the new node
  Initialize variable for the size of half the node (int split_size)

  If (node->count Is Not Equal To One)
    Calculate: (node->count / 2)
  If (node->count Is Equal To One)
    Set it to one
*/    
  BList<T, Size>::BNode *B_node = new_node();
  int split_size = 0;

  if(A_node->count != 1)
  	split_size = reinterpret_cast<int>((A_node->count) / 2);
  else
  	split_size = 1;

/*
  Copy split_size values from A_node to B_node
*/
  for(int i = 0; i < split_size; i++)
  	B_node->values[i] = A_node->values[i];
  B_node->count = A_node->count - (A_node->count - split_size);

/*
  Shift all elements in A_node
*/

  for(int i = 0; i < A_node->count - split_size; i++)
  	A_node->values[i] = A_node->values[i + split_size];
  A_node->count = (A_node->count - split_size);

  if(A_node->prev == NULL)
  {
  	B_node->next = A_node;
  	A_node->prev = B_node;
  	head_ = B_node;
  }
  else
  {
  	A_node->prev->next = B_node;
  	B_node->next = A_node;
  	B_node->prev = A_node->prev;
  	A_node->prev = B_node;
  }

  return B_node;
}

/////////////////////////////////////////////////////////////////////////////////
// Binary Search (Tested - Complete)                                           //
/////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
int BList<T, Size>::BinarySearch(const T *array, int size, const T &value) const
{
  int low = 0;
  int mid = 0;
  int high = size - 1;

  while(low < mid)
  {
    mid = (high + low) / 2;
    
    if(array[mid] == value)
    	return mid;
    else if(value < array[mid])
    	high = mid - 1;
    else if(array[mid] < value)
    	low = mid + 1;
  }
  return -1;
}

template <typename T, int Size>
int BList<T, Size>::LinearSearch(const T *array, int size, const T &value) const
{
  for(int i = 0; i < size; i++)
  {
    if(array[i] == value)
      return i;
  }
  return -1;
}

///////////////////////////////////////////////////////////////////////////////////
// Insert_Array                                                                  //
/*/////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  Function : void push_front(int value)                                        //
//                                                                               //
//  Breif    : Add an element to the start of a Blist object in the order given  //
//                                                                               //
*//////////////////////////////////////////////////////////////////////////////////
template <typename T, int Size>
int BList<T, Size>::Insert_Array(T *array, int count, const T &value)
{
  int i;
  for(i = 0; i < Size; i++)
  {
  	if(value < array[i])
    {
      for(int j = Size - 1; i < j; j--)
   	    array[j] = array[j - 1];

      array[i] = value;
      return 1;
    }
  }
  array[count] = value;

  return 0;
}

