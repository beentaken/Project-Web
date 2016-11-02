#include "BList.h"
#include <iostream>
#include <iomanip>

template <typename T, int Size>
void DumpList(const BList<T, Size>& blist, bool flat = false)
{
  const typename BList<T, Size>::BNode *node = blist.GetHead();
  int count = 0;
  if (flat)
    std::cout << "List: ";
  while (node)
  {
    if (!flat)
      std::cout << "Node " << std::setw(3) << ++count << " (" << std::setw(2) << node->count << "): ";

    for (int i = 0; i < node->count; i++)
    {
      std::cout << node->values[i] << " ";
    }

    if (!flat)
      std::cout << std::endl;
    node = node->next;
  }
  std::cout << std::endl;
}

int main()
{
	BList<int, 2> list;
	int array[11] = {35, 12, 67, 6, 34, 1, 99, 8, 23, 87} ;

	for(int i = 1; i < 10; i++)
	{
		std::cout << "Inserting (" << array[i] << ")" << std::endl;
		list.insert(array[i]);
		DumpList(list);
	    DumpList(list, true);
	    std::cout << "======================================================" << std::endl;
	}

	
	
	return 0;
}