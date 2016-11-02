/******************************************************************************/
/*!
\file   ChHashTable.cpp
\author Damien Paige Baca
\par    email: Gamerlios123@gmail.com
\par    DigiPen login: damien.baca
\par    Course: CS280
\par    Assignment #5
\date   March 28, 2016

\brief
	Hash table implemented using chaining.
*/
/******************************************************************************/
#include <string.h>
#include <cmath>

/**********************************************************************************/
/*!
  \brief 
    Constructs the hash table with a given config class and an obkect allocator

  \param Config
  	Configuration information for the table

  \param allocator
  	Object Allocator for creating nodes
*/
/**********************************************************************************/
template <typename T>
ChHashTable<T>::ChHashTable(const HTConfig& Config, ObjectAllocator* allocator)
{
	if(allocator == 0)
	{
		stats_.Allocator_ = NULL;
		CPPMemMngr_ = true;
	}
	else
	{
		stats_.Allocator_ = allocator;
		CPPMemMngr_ = false;
	}

	stats_.HashFunc_ = Config.HashFunc_;
	stats_.TableSize_ = Config.InitialTableSize_;
	config_ = &Config;
	table_ = new ChHTHeadNode[stats_.TableSize_];
}

/**********************************************************************************/
/*!
  \brief 
    Destructs the object
*/
/**********************************************************************************/
template <typename T>
ChHashTable<T>::~ChHashTable()
{
	clear();
	delete [] table_;
}

/**********************************************************************************/
/*!
  \brief 
    Inserts an item into the hash table.
      - No duplicates
      - The table will grow upon reaching the max load factor

  \param Key
  	The key that will be hashed to yield the index of the element

  \param Data
  	The actual data being stored
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::insert(const char *Key, const T& Data)
{
	unsigned HashKey = config_->HashFunc_(Key, stats_.TableSize_);
	double load_factor = (double)(stats_.Count_ + 1)/(double)stats_.TableSize_;
	ChHTNode *NewNode;

	if(load_factor > config_->MaxLoadFactor_)
	{
		GrowTable();
		HashKey = config_->HashFunc_(Key, stats_.TableSize_);
	}

	DuplicateCheck(&table_[HashKey], Key);

	NewNode = Make_Node(Key, Data);
	addtofront(&table_[HashKey], NewNode);

	stats_.Probes_++;
	stats_.Count_++;
	return;
}

/**********************************************************************************/
/*!
  \brief 
    Removes an item from the hash table

  \param Key
  	The key to remove
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::remove(const char *Key)
{
	unsigned HashKey = config_->HashFunc_(Key, stats_.TableSize_);
	remove_node(&table_[HashKey], Key);
	stats_.Count_--;
}

/**********************************************************************************/
/*!
  \brief 
    Helper function for remove.

  \param head
  	The head of the appropriate chain

  \param Key
  	The key being removed
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::remove_node(ChHTHeadNode *head, const char *Key)
{
	ChHTNode *temp = head->Nodes;

	if(head->Nodes == NULL) 
		throw(HashTableException(HashTableException::E_ITEM_NOT_FOUND, "error: Item Not Found"));
		
	stats_.Probes_++;
	if(strcmp(head->Nodes->Key, Key) == 0)
	{
		head->Nodes = head->Nodes->Next;
		if(CPPMemMngr_ == false)
				stats_.Allocator_->Free(temp);
			else
				delete temp;
		return;
	}
	
	while(temp->Next)
	{
		stats_.Probes_++;
		if(strcmp(temp->Next->Key, Key) == 0)
		{
			ChHTNode *temp2 = temp->Next;
			temp->Next = temp2->Next;
			if(CPPMemMngr_ == false)
				stats_.Allocator_->Free(temp2);
			else
				delete temp2;
			return;
		}
		temp = temp->Next;
	}
	throw(HashTableException(HashTableException::E_ITEM_NOT_FOUND, "error: Item Not Found"));
}

/**********************************************************************************/
/*!
  \brief 
    Finds and returns an element in the table

  \param Key
  	Key to be found
*/
/**********************************************************************************/
template <typename T>
const T& ChHashTable<T>::find(const char *Key) const
{
	unsigned HashKey = config_->HashFunc_(Key, stats_.TableSize_);
	ChHTNode *current = table_[HashKey].Nodes;

	while(current)
	{
		stats_.Probes_++;
		if(strcmp(current->Key, Key) == 0)
			return current->Data;
		current = current->Next;
	}
	throw(HashTableException(HashTableException::E_ITEM_NOT_FOUND, "error: Item Not Found"));
}

/**********************************************************************************/
/*!
  \brief 
    Clears the table, deleteing all nodes.
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::clear()
{
	for(int i = 0; i < stats_.TableSize_; i++)
	{
		ChHTNode *Destroyer = table_[i].Nodes;
		ChHTNode *Pointer = Destroyer;
		while(Destroyer != NULL)
		{
			Pointer = Pointer->Next;
			if(CPPMemMngr_ == false)
				stats_.Allocator_->Free(Destroyer);
			else
				delete Destroyer;
			Destroyer = Pointer;
		}
		table_[i].Nodes = NULL;
	}
	stats_.Count_ = 0;
}

/**********************************************************************************/
/*!
  \brief 
    Returns the stats
*/
/**********************************************************************************/
template <typename T>
HTStats ChHashTable<T>::GetStats() const
{
	return stats_;
}

/**********************************************************************************/
/*!
  \brief 
    Returns the table
*/
/**********************************************************************************/
template <typename T>
const typename ChHashTable<T>::ChHTHeadNode *ChHashTable<T>::GetTable() const
{
	return table_;
}

/**********************************************************************************/
/*!
  \brief 
    Allocates a node and returns it

  \param Key
  	The nodes key

  \param Data
  	The nodes data
*/
/**********************************************************************************/
template <typename T>
typename ChHashTable<T>::ChHTNode *ChHashTable<T>::Make_Node(const char *Key, const T& Data)
{
	ChHTNode *NewNode;
	if(CPPMemMngr_ == true)
	{
		try 
		{
			NewNode = new ChHTNode(Data);
		} 
		catch (std::bad_alloc)
		{
			throw(HashTableException(HashTableException::E_NO_MEMORY, "error: Node Allocation Failed [Out Of Memory]"));
		}
	}
	else
	{
		ChHTNode *mem =  reinterpret_cast<ChHTNode *>(stats_.Allocator_->Allocate());
		NewNode = new (mem) ChHTNode(Data);
	}
	strcpy(NewNode->Key, Key);
	NewNode->Next = NULL;
	return NewNode;
}

/**********************************************************************************/
/*!
  \brief 
    Checks to make sure that the node being inserted is not a duplicate

  \param head
  	The chain being searched

  \param Key
  	The key being checked
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::DuplicateCheck(const ChHTHeadNode *head, const char *Key)
{
	ChHTNode *current = head->Nodes;
	while(current)
	{
		stats_.Probes_++;
		if(strcmp(current->Key, Key) == 0)
			throw(HashTableException(HashTableException::E_DUPLICATE, "error: Attempted To Insert Duplicate Item"));

		current = current->Next;
	}
}

/**********************************************************************************/
/*!
  \brief 
    AInserts a node onto the fornt of a chain

  \param head
  	The head of the chain

  \param node
  	The node to be inserted
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::addtofront(ChHTHeadNode *head, ChHTNode *node)
{
	if(head == NULL)
	{
		head->Nodes = node;
		head->Count++;
		return;
	}

	node->Next = head->Nodes;
	head->Nodes = node;
	head->Count++;
	return;
}

/**********************************************************************************/
/*!
  \brief 
    Expands the table if needed
*/
/**********************************************************************************/
template <typename T>
void ChHashTable<T>::GrowTable()
{
	double factor = std::ceil(stats_.TableSize_ * config_->GrowthFactor_);
    unsigned new_size = GetClosestPrime(static_cast<unsigned>(factor));
	ChHTHeadNode *NewTable = new ChHTHeadNode[new_size];
	unsigned HashKey;

	for(int i = 0; i < stats_.TableSize_; i++)
	{
		ChHTNode *current = table_[i].Nodes;
		while(current != NULL)
		{
 			HashKey = config_->HashFunc_(current->Key, new_size);
 			ChHTNode *temp = current->Next;
 			addtofront(&NewTable[HashKey], current);
 			current = temp;
		}
		table_[i].Nodes = NULL;
	}
	delete [] table_;
	table_ = NewTable;
	stats_.TableSize_ = new_size;
	stats_.Expansions_++;
	return;
}
