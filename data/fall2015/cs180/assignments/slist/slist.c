/******************************************************************************/
/*!
  \file   slist.c
  \author Damien Baca
  \par    email: damien.baca\@digipen.edu
  \par    DigiPen login: damien.baca
  \par    Course: CS180
  \par    Assignment #1
  \date   09/16/2015
  \brief  Implements basic functions for manipulation of a linked list.
          - make_node
          - sll_dump
          - sll_add
          - sll_remove
          - sll_insert_before
          - sll_insert_after
          - sll_destroy
*/
/******************************************************************************/
#include <stdio.h>  /* printf             */
#include <stdlib.h> /* malloc, free, exit */
#include <string.h> /* strncpy            */
#include "slist.h"



static sll_node *make_node(int value, const char *label)
{
  sll_node *node = (sll_node *)malloc(sizeof(sll_node));
  if (!node)
  {
    printf("Can't allocate new node.\n");
    exit(-1);
  }

  node->value = value;
  node->next = NULL;

    /* Be sure not to overwrite memory */
  strncpy(node->label, label, LABEL_SIZE - 1);
  node->label[LABEL_SIZE - 1] = 0;
  
  return node;
}

void sll_dump(const sll_node *list)
{
  printf("==================\n");
  while (list)
  {
    printf("%4i: %s\n", list->value, list->label);
    list = list->next;    
  }
}


/******************************************************************************/
/*!
  \brief Adds a node to the end of a linked list that is initialized with
           a given value and label.
    
  \param list  - The linked list to which we are adding a node.

  \param value - The value that the new node will be initialized with.
  
  \param label - The label that the new node will be initilaized with.

  \return The head of the linked list.
    
*/
/******************************************************************************/
sll_node *sll_add(sll_node *list, int value, const char *label)
{
  sll_node* NewNode = make_node(value, label);
  sll_node* NodePtr = list;
  
  if(!list)
  {
    list = NewNode;
    return list;
  }

  while(NodePtr->next)
    NodePtr = NodePtr->next;

  NodePtr->next = NewNode;
  return list;
}


/******************************************************************************/
/*!
  \brief Removes a node that has the value "search_value" from the linked list
    
  \param list         - The linked list that we are taking a node from.

  \param search_value - The value we use to know what node to remove.

  \return The head of the linked list.
    
*/
/******************************************************************************/
sll_node *sll_remove(sll_node *list, int search_value)
{
  sll_node* NodePtr = list;
  sll_node* FreeNode = list;

  /* If there is no list then there is nothing to remove */
  if(!list)
   return list;
    
  /* Check to see if the removed node is the first node in the list */
  if(NodePtr->value == search_value)
  {
    /* Case for removing the first node */
    if(list->next == NULL)
      list = NULL;
    else
      list = list->next;

    free(NodePtr);
    return list;
  }
  
  /* Search for the correct node */
  while(NodePtr->next->value != search_value)
  {
    NodePtr = NodePtr->next;
    /* If for whatever reason we don't find the node then just return */
    if(NodePtr->next == NULL)
      return list;
  }

  /* Checks to see if the removed node is the last node in the list   */
  if(NodePtr->next->next == NULL)
  {
    /* Case for the last node in the list      */
    free(NodePtr->next);
    NodePtr->next = NULL;
  }
  else
  {
    /* Case for node in the middle of the list */
    FreeNode = NodePtr->next;
    NodePtr->next = NodePtr->next->next;
    free(FreeNode);
  }

  return list;
}


/******************************************************************************/
/*!
  \brief Uses the value of a node to indicate the location where a new node
           should be inserted. The node is inserted before the node of value
           "search_value".
    
  \param list         - The linked list we are inserting a node into.

  \param search_value - The value we are using to find what node should mark
                          where the insertion is to take place.

  \param value        - The value that the new node is initialized too.

  \param label        - The label that the new node is initialized too.
    
  \return The head of the linked list.
    
*/
/******************************************************************************/
sll_node *sll_insert_before(sll_node *list,
                            int search_value,
                            int value,
                            const char *label)
{
  sll_node* NewNode = make_node(value, label);
  sll_node* NodePtr = list;

  /* If there is no list than just add the node to the list */
  if(!list)
  {
    list = NewNode;
    return list;
  }

  /* Checks to see if the first node is the one we are searching for */
  if(NodePtr->value == search_value)
  {
    /* Inserts the node at the front position */
    NewNode->next = list;
    list= NewNode;
    return list;
  }
  
  /* Find the node we are looking for */
  while(NodePtr->next->value != search_value)
  {
    NodePtr = NodePtr->next;
    /* If for whatever reason we don't find the node then just return */
    if(NodePtr->next == NULL)
    {
      free(NewNode);
      return list;
    }
  }
  
  /* Found the node we were looking for, so insert NewNode before it */
  NewNode->next = NodePtr->next;
  NodePtr->next = NewNode;
  
  
   /* NewNode will never be placed at the end of the list */
  return list;
}

/******************************************************************************/
/*!
  \brief Uses the value of a node to indicate the location where a new node
           should be inserted. The node is inserted after the node of value
           "search_value".
    
  \param list         - The linked list we are inserting a node into.

  \param search_value - The value we are using to find what node should mark
                          where the insertion is to take place.

  \param value        - The value that the new node is initialized too.

  \param label        - The label that the new node is initialized too.
    
  \return void
    
*/
/******************************************************************************/

void sll_insert_after(sll_node *list,
                      int search_value,
                      int value,
                      const char *label)
{
  sll_node* NewNode = make_node(value, label);
  sll_node* NodePtr = list;

  /* If there is no list than just add the node to the list */
  if(!list)
    list = NewNode;

  /* Find the node we are looking for */
  while(NodePtr->value != search_value)
  {
    /* If for whatever reason we don't find the node then just return */
    if(NodePtr->next == NULL)
    {
      free(NewNode);
      return;
    }

    NodePtr = NodePtr->next;
  }

  /* Check to see if the node we were looking for is the last node */
  if(NodePtr->next != NULL)
  {
    /* Case for found node that is not last */
    NewNode->next = NodePtr->next;
    NodePtr->next = NewNode;
  }
  else
  {
    /* Case for found node that is last     */ 
    NodePtr->next = NewNode;
    NewNode->next = NULL;
  }
}


/******************************************************************************/
/*!
  \brief Deallocates a linked list
    
  \param list - The linked list to be destroyed.
    
  \return void
    
*/
/******************************************************************************/
void sll_destroy(sll_node *list)
{
  sll_node* NodePtr1 = list;
  sll_node* NodePtr2 = list;
  
  /* Check to make sure the list exists */
  if(!list)
    return;

  while(NodePtr2->next != NULL)
  {
    NodePtr2 = NodePtr2->next;
    free(NodePtr1);
    NodePtr1 = NodePtr2;
  }
  free(NodePtr1);
}
