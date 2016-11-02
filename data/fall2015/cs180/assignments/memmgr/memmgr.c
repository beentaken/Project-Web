#include "memmgr.h"
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BESTFIT (unsigned)-1
int Set_Label(block_info* currnode, const char* label);

static MMPolicy policy_flag;

int Set_Label(block_info* currnode, const char* name)
{
  int i;

  for (i = 0; i < LABEL_SIZE; ++i)
  {
  	if(name[i])
      currnode->label[i] = name[i];
    else	
      currnode->label[i] = 0;
  }
  return 1;
}

block_info *mm_create(size_t bytes, enum MMPolicy policy)
{
	block_info *heap;
	heap = malloc(sizeof(block_info));

	heap->allocated = 0;
	heap->address = malloc(bytes);
	heap->size = bytes;
	heap->next = NULL;
	Set_Label(heap, "START");
	policy_flag = policy;

	return heap;
}

void *mm_allocate(block_info *heap, size_t bytes, char *label)
{
	block_info* nextblock = heap->next;
	block_info* currblock = heap;
	/* Start with a large number relative to the size being requested */
	size_t Best_Fit =  INITIAL_BESTFIT;

	block_info* free_block = malloc(sizeof(block_info));
	free_block->allocated = 0;
	Set_Label(free_block, "FREE");
	
/* Memory Manager Policy First Fit ////////////////////////////////////////////////////////////////*/
	if(policy_flag == mmpFirstFit) 
	{
      while(currblock->next)
      {
	    if(!currblock->allocated)
	    {
	    	if(currblock->size > bytes)
	    	{
	        /*free_block->allocated = 0 */
              free_block->address = currblock->address + bytes;
              free_block->size = currblock->size - bytes;
              free_block->next = nextblock;

        
              currblock->allocated = 1;
            /*currblock->address = currblock->address */
              currblock->size = bytes;
              currblock->next = free_block;
              Set_Label(currblock, label);

              return currblock->address;
	    	}
	    	else if(currblock->size == bytes)
	    	{
	          currblock->allocated = 1;
	          Set_Label(currblock, label);
	          free(free_block);
	          return currblock->address;
	    	}
	    }

	      if(nextblock->next)
	      	nextblock = nextblock->next;
	      else
	      	nextblock = NULL;
	      currblock = currblock->next;
      }

      if(!currblock->next)
      {
      	if(currblock->allocated)
        {
          free(free_block);
      		return NULL;
        }

      	if(currblock->size < bytes)
        {
          free(free_block);
      		return NULL;
        }

      	if(currblock->size == bytes)
	    {
	      currblock->allocated = 1;
	      Set_Label(currblock, label);
	      free(free_block);
	      return currblock->address;
	    }

      /*free_block->allocated = 0 */
	  	free_block->address = currblock->address + bytes;
        free_block->size = currblock->size - bytes;
	  	free_block->next = NULL;
        
	  	currblock->allocated = 1;
	  /*currblock->address = currblock->address */
        currblock->size = bytes;
        currblock->next = free_block;
        Set_Label(currblock, label);

        return currblock->address;
      }
      free(free_block);
      return NULL;
    }
/*/////////////////////////////////////////////////////////////////////////////////////////////////*/

/* Memory Manager Policy Best Fit /////////////////////////////////////////////////////////////////*/

    if(policy_flag == mmpBestFit)
    {
    	/* First We Find The Smallest Memory Slot That Is Big Enough For Use */
    	while(currblock)
    	{
           if(!currblock->allocated)
           {
           	 if(Best_Fit > currblock->size && bytes <= currblock->size)
           	 	Best_Fit = currblock->size;
           }
           currblock = currblock->next;
    	}
    	currblock = heap;
    	
    	if(Best_Fit == INITIAL_BESTFIT)
    		return NULL;

    	while(currblock->next)
        {
	      if(!currblock->allocated)
	      {
	        if(currblock->size == bytes)
	    	{
	          currblock->allocated = 1;
	          Set_Label(currblock, label);
	          free(free_block);
	          return currblock->address;
	    	}
	    	else if(currblock->size == Best_Fit)
	    	{
	        /*free_block->allocated = 0 */
              free_block->address = currblock->address + bytes;
              free_block->size = currblock->size - bytes;
              free_block->next = nextblock;

        
              currblock->allocated = 1;
            /*currblock->address = currblock->address */
              currblock->size = bytes;
              currblock->next = free_block;
              Set_Label(currblock, label);

              return currblock->address;
	    	}
	      }
	      if(nextblock->next)
	      	nextblock = nextblock->next;
	      currblock = currblock->next;
        }

        if(!currblock->next)
        {
          if(currblock->allocated)
          {
            free(free_block);
      		  return NULL;
          }

      	  if(currblock->size == bytes)
	      {
	        currblock->allocated = 1;
	        Set_Label(currblock, label);
	        free(free_block);
	        return currblock->address;
	      }
	      else if(currblock->size == Best_Fit)
	      {
         /* free_block->allocated = 0 */
	  	    free_block->address = currblock->address + bytes;
            free_block->size = currblock->size - bytes;
	  	    free_block->next = NULL;
          
	  	    currblock->allocated = 1;
	     /* currblock->address = currblock->address */
            currblock->size = bytes;
            currblock->next = free_block;
            Set_Label(currblock, label);
          }

          return currblock->address;
        }
        free(free_block);
        return NULL;
    }
    free(free_block);
    return NULL;
}

int mm_deallocate(block_info *heap, void *address)
{
  block_info* nextblock = heap->next;
  block_info* currblock = heap;
  block_info* prevblock = heap;

  while(currblock)
  {
  	if(currblock->allocated)
  	{
  		if(currblock->address == address)
  		{
          currblock->allocated = 0;
          break;
        }
  	}
  	if(nextblock->next)
	  nextblock = nextblock->next;
    
    /* Give currblock a head start on prevblock */
  	if(currblock != heap)
  	  prevblock = prevblock->next;

  	currblock = currblock->next;
  }

  /* LOOK TO THE LEFT */
  if(prevblock->allocated == 0 && prevblock != currblock)
  {
     prevblock->size = currblock->size + prevblock->size;
     prevblock->next = nextblock;
     free(currblock);
     currblock = prevblock;
  }

  /* LOOK TO THE RIGHT */
  if(nextblock->allocated == 0 && nextblock != currblock)
  {
    currblock->size = currblock->size + nextblock->size;
    currblock->next = nextblock->next;
    free(nextblock);
  }

return 1;
}

void mm_destroy(block_info *heap)
{
  block_info* Pointer = heap;
  block_info* DESTROYER = heap;
  free(heap->address);

  while(DESTROYER)
  {
  	Pointer = Pointer->next;
  	free(DESTROYER);

  	DESTROYER = Pointer;
  }
}
