#include "ObjectAllocator.h"
#include <stdio.h>
#include <string.h>
#include "iostream"

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config)
{
  PageList_ = NULL;
  FreeList_ = NULL;
  Conf_ = config;
  allocation_num_ = 0;

  Stats_.ObjectSize_ = ObjectSize;
  
  if(Conf_.UseCPPMemManager_)
    return;

	// Allocate a new page
	allocate_new_page();
}

ObjectAllocator::~ObjectAllocator()
{
  return;
}

void *ObjectAllocator::Allocate(const char *label)
{

  if(Conf_.UseCPPMemManager_)
  {
    void *data = new char[Stats_.ObjectSize_];
  
    Stats_.Allocations_++;
    Stats_.ObjectsInUse_++;

    if(Stats_.ObjectsInUse_ > Stats_.MostObjects_)
      Stats_.MostObjects_ = Stats_.ObjectsInUse_;

    return data;
  }
    

  void *temp;

  if(Stats_.FreeObjects_ == 0)
      allocate_new_page();

  temp = FreeList_;
  FreeList_ = FreeList_->Next;

  if(Conf_.DebugOn_ == true)
    memset(temp, ALLOCATED_PATTERN, Stats_.ObjectSize_);

  Stats_.FreeObjects_--;
  Stats_.Allocations_++;
  Stats_.ObjectsInUse_++;
  allocation_num_++;

  // Setting Up Header Block -------------------------------------------------
  temp = ((reinterpret_cast<char*>(temp)
                                 - Conf_.PadBytes_)
                                 - Conf_.HBlockInfo_.size_);

  if(Conf_.HBlockInfo_.type_ == Conf_.hbBasic)
  {
    (*reinterpret_cast<unsigned*>(temp)) = allocation_num_;
    temp = reinterpret_cast<char*>(temp) + sizeof(unsigned);
    (*reinterpret_cast<char*>(temp)) = 1; // 1 -> 0000 0001
    temp = reinterpret_cast<char*>(temp) + sizeof(char) + Conf_.PadBytes_;
  }
  if (Conf_.HBlockInfo_.type_ == Conf_.hbExtended)
  {
    temp = reinterpret_cast<char*>(temp) + Conf_.HBlockInfo_.additional_;
    (*reinterpret_cast<short*>(temp))++;
    temp = reinterpret_cast<char*>(temp) + sizeof(short);
    (*reinterpret_cast<unsigned*>(temp)) = allocation_num_;
    temp = reinterpret_cast<char*>(temp) + sizeof(unsigned);
    (*reinterpret_cast<char*>(temp)) = 1; // 1 -> 0000 0001
    temp = reinterpret_cast<char*>(temp) + sizeof(char) + Conf_.PadBytes_; 
  }
  if(Conf_.HBlockInfo_.type_ == Conf_.hbExternal)
  {
    MemBlockInfo *block = new MemBlockInfo;
    block->in_use = 1;
    block->alloc_num = allocation_num_;

    if(label)
    {
      block->label = new char[strlen(label) + 1];
      strcpy(block->label, label);
    }
    else
    {
      block->label = new char[1];
      block->label[0] = 0;
    }
    MemBlockInfo **Header = reinterpret_cast<MemBlockInfo**>(temp);
    *Header = block;

    temp = reinterpret_cast<char*>(temp) + sizeof(void*) + Conf_.PadBytes_; 
  }
  if(Conf_.HBlockInfo_.type_ == Conf_.hbNone)
    temp = reinterpret_cast<char*>(temp) + Conf_.PadBytes_; 


  if(Stats_.ObjectsInUse_ > Stats_.MostObjects_)
  	Stats_.MostObjects_ = Stats_.ObjectsInUse_;

	return temp;
}

void ObjectAllocator::Free(void *Object)
{

  if(Conf_.UseCPPMemManager_)
  {
    delete [] (reinterpret_cast<char*>(Object));
    Stats_.Deallocations_++;
    Stats_.ObjectsInUse_--;
    return;
  }

  GenericObject *temp;

  if(Conf_.DebugOn_ == true)
  {
    if(not_on_boundry(Object))
      throw OAException(OAException::E_BAD_BOUNDARY, "Address given does not lie on a proper boundry");
    
    if(is_on_freelist(Object))
      throw OAException(OAException::E_MULTIPLE_FREE, "FreeObject: Object has already been freed.");

    if(pad_corrupted(Object))
      throw OAException(OAException::E_CORRUPTED_BLOCK, "Corrupted Memory!");
  	
    memset(Object, FREED_PATTERN, Stats_.ObjectSize_);
  }
  
  temp = FreeList_;
  FreeList_ = reinterpret_cast<GenericObject*>(Object);
  FreeList_->Next = temp;

  Stats_.FreeObjects_++;
  Stats_.Deallocations_++;
  Stats_.ObjectsInUse_--;

  // Setting Up Header Block -------------------------------------------------
  Object = ((reinterpret_cast<char*>(Object)
                                    - Conf_.PadBytes_)
                                    - Conf_.HBlockInfo_.size_);

  if(Conf_.HBlockInfo_.type_ == Conf_.hbBasic)
  {
    (*reinterpret_cast<unsigned*>(Object)) = 0;
    Object = reinterpret_cast<char*>(Object) + sizeof(unsigned);
    (*reinterpret_cast<char*>(Object)) = 0; // 1 -> 0000 0001
    Object = reinterpret_cast<char*>(Object) + 1 + Conf_.PadBytes_;
  }
  if(Conf_.HBlockInfo_.type_ == Conf_.hbExtended)
  {
    Object = reinterpret_cast<char*>(Object) + Conf_.HBlockInfo_.additional_ + sizeof(short);
    (*reinterpret_cast<unsigned*>(Object)) = 0;
    Object = reinterpret_cast<char*>(Object) + sizeof(unsigned);
    (*reinterpret_cast<char*>(Object)) = 0; // 0 -> 0000 0000
    Object = reinterpret_cast<char*>(Object) + 1 + Conf_.PadBytes_;      
  }
  if(Conf_.HBlockInfo_.type_ == Conf_.hbExternal)
  {
    MemBlockInfo **Header = reinterpret_cast<MemBlockInfo**>(Object);

    delete []((*Header)->label);
    delete(*Header);

    char *temp2 = reinterpret_cast<char*>(Header);
    memset(temp2, 0, sizeof(void*));
  }
}

void ObjectAllocator::allocate_new_page(void)
{
	// Conf_.MaxPages_ = 0 means that we can use an unlimited ammount of pages
	if(Stats_.PagesInUse_ == Conf_.MaxPages_ && Conf_.MaxPages_ != 0)		
		throw OAException(OAException::E_NO_PAGES, "Allocation Failure: Max number of in use pages reached");

  size_t pagesize = (Conf_.ObjectsPerPage_ * Stats_.ObjectSize_) 
                  + (2 * Conf_.PadBytes_ * Conf_.ObjectsPerPage_) 
                  + (Conf_.HBlockInfo_.size_ * Conf_.ObjectsPerPage_) 
                  + sizeof(void*);
	
  Stats_.PageSize_ = pagesize;

  char *PageTemp = reinterpret_cast<char*>(PageList_);

  try//hard
  {
    PageList_ = reinterpret_cast<GenericObject*>(new char[pagesize]);
  } catch (std::bad_alloc& Error) {
    throw OAException(OAException::E_NO_MEMORY, "Allocation Failure: No Memory");
  }
  
  if(Conf_.DebugOn_ == true)
   	memset(PageList_, UNALLOCATED_PATTERN, pagesize);

  PageList_->Next = reinterpret_cast<GenericObject*>(PageTemp);

  PageTemp = reinterpret_cast<char*>(PageList_) + sizeof(void*);
    
  for(unsigned int i = 0; i < Conf_.ObjectsPerPage_; i++)
  {
    // Set Up Header Block
    memset(PageTemp, 0, Conf_.HBlockInfo_.size_);
    PageTemp = reinterpret_cast<char*>(PageTemp) + Conf_.HBlockInfo_.size_;
    
    // Set Up Pad Bytes
    if(Conf_.DebugOn_ == true)
      memset(PageTemp, PAD_PATTERN, Conf_.PadBytes_);
    PageTemp = reinterpret_cast<char*>(PageTemp) + Conf_.PadBytes_;

    put_on_freelist(PageTemp);

    PageTemp = reinterpret_cast<char*>(PageTemp) + Stats_.ObjectSize_;
    
    // Set Up Pad Bytes
    if(Conf_.DebugOn_ == true)
      memset(PageTemp, PAD_PATTERN, Conf_.PadBytes_);
    PageTemp = reinterpret_cast<char*>(PageTemp) + Conf_.PadBytes_;
  }
    
  // Increment number of pages in use
  Stats_.PagesInUse_++;

  return;
}

void ObjectAllocator::put_on_freelist(void *Object)
{
	GenericObject *freetemp;
    
  // Save FreeLists latest position
	freetemp = FreeList_;
	// Point FreeList to the new object
	FreeList_ = reinterpret_cast<GenericObject*>(Object);
	// Connect the new object to the rest of the list
	FreeList_->Next = freetemp;

  // Increment number of free objects
	Stats_.FreeObjects_++;

    return;
}

bool ObjectAllocator::is_on_freelist(void *Object) const
{
  GenericObject *ptr = FreeList_;
  
  if(Stats_.FreeObjects_ == 0)
  	return false;
  if(Conf_.HBlockInfo_.type_ == Conf_.hbNone)
  {
    while(ptr)
    {
      if(ptr == Object)
      	return true;
      ptr = ptr->Next;
    }
    return false;
  }

  if(Conf_.HBlockInfo_.type_ == Conf_.hbBasic || Conf_.HBlockInfo_.type_ == Conf_.hbExtended)
  {
    char *temp = reinterpret_cast<char*>(Object) - 1 - Conf_.PadBytes_;

    if(*reinterpret_cast<char*>(temp) == 0)
      return true;
    else
      return false;
  }
  if(Conf_.HBlockInfo_.type_ == Conf_.hbExternal)
  {
    char *temp = reinterpret_cast<char*>(Object) - sizeof(void*) - Conf_.PadBytes_;
    MemBlockInfo **Header = reinterpret_cast<MemBlockInfo**>(temp);

    if((*Header)->in_use == false)
      return true;
    else
      return false;
  }

  return false;
}

bool ObjectAllocator::not_on_boundry(void *Object)
{
    size_t difference;
    GenericObject *Page_Start = PageList_;
	  
    size_t ObjectPos = reinterpret_cast<size_t>(Object);

    while(true)
    {
      difference = ObjectPos - reinterpret_cast<size_t>(Page_Start);

      if(difference > 0)
        break;

      Page_Start = Page_Start->Next;
    }

    difference -= sizeof(void*);
    difference -= Conf_.PadBytes_;
    difference -= Conf_.HBlockInfo_.size_;
    difference = difference % (Stats_.ObjectSize_ + (2 * Conf_.PadBytes_) + Conf_.HBlockInfo_.size_);

    if(difference)
      return true;
    else
      return false;
}

bool ObjectAllocator::pad_corrupted(void *Object)
{
  char *temp = reinterpret_cast<char*>(Object) - Conf_.PadBytes_;

  for(unsigned int i = 0; i < Conf_.PadBytes_; i++)
  {
    if(*reinterpret_cast<unsigned char*>(temp) != PAD_PATTERN)
       return true;
    temp = reinterpret_cast<char*>(temp) + 1;
  }

  temp = reinterpret_cast<char*>(temp) + Stats_.ObjectSize_;

  for(unsigned int i = 0; i < Conf_.PadBytes_; i++)
  {
    if(*reinterpret_cast<unsigned char*>(temp) != PAD_PATTERN)
       return true;
    temp = reinterpret_cast<char*>(temp) + 1;
  }
  
  return false;
}

bool ObjectAllocator::ImplementedExtraCredit(void)
{
	return false;
}

const void *ObjectAllocator::GetFreeList(void) const
{
  return FreeList_;
}

const void *ObjectAllocator::GetPageList(void) const
{
  return PageList_;
}

OAConfig ObjectAllocator::GetConfig(void) const
{
  return Conf_;
}
OAStats ObjectAllocator::GetStats(void) const
{
  return Stats_;
}

unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
  char *temp = reinterpret_cast<char*>(PageList_);
  unsigned counter = 0;
  char *PageTemp;

  for(unsigned int i = 0; i < Stats_.PagesInUse_; i++)
  {
    PageTemp = temp;
    temp += sizeof(void*) + Conf_.HBlockInfo_.size_ + Conf_.PadBytes_;

    for(unsigned int i = 0; i < Conf_.ObjectsPerPage_; i++)
    {  
      if(is_on_freelist(reinterpret_cast<void*>(temp)))
      {
        fn(temp, Stats_.ObjectSize_);
        counter++;
      }
    
      temp += Stats_.ObjectSize_ + (2 * Conf_.PadBytes_) + Conf_.HBlockInfo_.size_;
    }
    temp = PageTemp;
    *reinterpret_cast<GenericObject**>(temp) = reinterpret_cast<GenericObject*>(temp)->Next;
  }
  return counter;
}

unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  void *temp = NULL;
  fn(temp, 0);
  return 1;
}

unsigned ObjectAllocator::FreeEmptyPages(void)
{
	return 1;
}

void ObjectAllocator::SetDebugState(bool State)
{
  Conf_.DebugOn_ = State;
}
