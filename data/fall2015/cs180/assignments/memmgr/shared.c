#include "shared.h"
#include <string.h> /* strcat, strcpy */
#include <dlfcn.h>  /* dl stuff       */
#include <stdint.h> /* intptr_t       */
#include <stdio.h>  /* printf         */

int load_library(const char *libname, void **libhandle)
{
  libhandle = dlopen(libname, 0);
  if(libhandle)
  	return FAILURE;
  return SUCCESS;
}

int get_functions(void *libhandle, 
                   LibraryFunctions *functions, 
                   const char **fn_names)
{
	functions->create = (MM_CREATE)(intptr_t)dlsym(libhandle, fn_names[0]);
      if(!functions->create)
    	return FAILURE;
    functions->destroy = (MM_DESTROY)(intptr_t)dlsym(libhandle, fn_names[1]);
      if(!functions->destroy)
    	return FAILURE;
    functions->allocate = (MM_ALLOCATE)(intptr_t)dlsym(libhandle, fn_names[2]);
      if(!functions->allocate)
    	return FAILURE;
    functions->deallocate = (MM_DEALLOCATE)(intptr_t)dlsym(libhandle, fn_names[3]);
      if(!functions->deallocate)
    	return FAILURE;
    
    return SUCCESS;
}