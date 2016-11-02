////////////////////////////////////////////////////////////////////////////////
#ifndef BLIST_H
#define BLIST_H
////////////////////////////////////////////////////////////////////////////////

#include <string>     // error strings
#include <iostream>

class BListException : public std::exception
{
  private:  
    int m_ErrCode;
    std::string m_Description;

  public:
    BListException(int ErrCode, const std::string& Description) :
    m_ErrCode(ErrCode), m_Description(Description) {};

    virtual int code(void) const { 
      return m_ErrCode; 
    }

    virtual const char *what(void) const throw() {
      return m_Description.c_str();
    }

    virtual ~BListException() throw() {
    }

    enum BLIST_EXCEPTION {E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR};
};

struct BListStats
{

  BListStats() : NodeSize(0), NodeCount(0), ArraySize(0), ItemCount(0)  {};
  BListStats(size_t nsize, int ncount, int asize, int count) : 
     NodeSize(nsize), NodeCount(ncount), ArraySize(asize), ItemCount(count)  {};

  size_t NodeSize; // Size of a node
  int NodeCount;   // Number of nodes in the list
  int ArraySize;   // Max number of items in each node
  int ItemCount;   // Number of items in the entire list
};  


/******************************************************************************/
/*!
  \class BList
  \brief  
    The BList class implements an improved Linked List data
    structure for storing/retrieving elements of any type.

    Operations include:

    Insert element in the back
    Insert element in the fornt
    Insert into sorted list
    Remove by index
    Remove by value
    Index operator [] support
    Return number of elements total
    Clear the list

*/
/******************************************************************************/
template <typename T, int Size = 1>
class BList
{
 
  public:
    struct BNode
    {
      BNode *next;
      BNode *prev;
      int count; // number of items currently in the node
      T values[Size];
      BNode() : next(0), prev(0), count(0) {};
    };

    BList();                 // default constructor                        
    BList(const BList &rhs); // copy constructor
    ~BList();                // destructor                         

    BList& operator=(const BList &rhs);

      // arrays will be unsorted
    void push_back(const T& value);
    void push_front(const T& value);

      // arrays will be sorted
    void insert(const T& value);

    void remove(int );
    void remove_by_value(const T& ); 

    int find(const T& value) const;       // returns index, -1 if not found

    T& operator[](int index);             // for l-values
    const T& operator[](int index) const; // for r-values

    int size(void) const;   // total number of items (not nodes)
    void clear(void);          // delete all nodes 

    static size_t nodesize(void); // so the driver knows the size
    
      // For debugging
    const BNode *GetHead() const;
    BListStats GetStats() const;


  private:
    BNode *head_;           // points to the first node
    BNode *tail_;           // points to the last node
    BListStats *stats_;
    bool sorted;

      // Helper Functions
    BNode *new_node();                // creates a new node
    BNode *split_node(BNode *node);   // splits a node
    int LinearSearch(const T *array, int size, const T &value) const;
    int BinarySearch(const T *array, int size, const T &value) const;
    int Insert_Array(T *array, int count, const T &value);

};

#include "BList.cpp"

#endif // BLIST_H
