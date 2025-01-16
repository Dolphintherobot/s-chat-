/* John Thoms, gvr812, 11357558
 * Dalton Beauchesne,nqr159,11353160 */

#include<list.h>
#include<stdlib.h>
#include<stdio.h>

/*
 * Gets first item in list and sets cursor to it
 * Post Condition:
 *   Cursor left at first item in list
 * Params:
 *   list: Pointer to LIST to modify
 * Return:
 *   Pointer to first item in list,NULL if failed 
 */
void* ListFirst(LIST* list) {
  void * vPtr;
	
  if (list == NULL)
    return (void*) NULL;
  
  if (list -> size == 0) return NULL;

  list -> cur = list -> head;
  vPtr = list -> cur -> data;
  return vPtr;
  
  
  
  
}


/*
 * Gets last item in list and sets cursor to it
 * Post Condition:
 *   Cursor left at last item in list
 * Params:
 *   list: Pointer to LIST to modify
 * Return:
 *   Pointer to last item in list
 */
void* ListLast(LIST* list) {
  void * vPtr;
	
  if (list == NULL)
    return (void*) NULL;
  
  if (list -> size == 0) return NULL;

  list -> cur = list -> tail;
  vPtr = list -> cur -> data;
  return vPtr;
} 

/*
 * Advances cursor by one item, gets new current item
 * Post Condition:
 *   Cursor is advanced by one item
 * Params:
 *   list: Pointer to LIST to modify
 * Return:
 *   Next item in list 
 */
void* ListNext(LIST* list) {
  void * vPtr;
	
  if (list == NULL)
    return (void*) NULL;
  
  if (list -> size < 2) return NULL;

  if (list -> cur == list -> tail) return NULL;

  list -> cur = list ->cur ->next;
  
  vPtr = list -> cur -> data;
  return vPtr;

}


/*
 * Moves cursor back one item, gets new current item
 * Post Condition:
 *   Cursor is moved back one item
 * Params:
 *   list: Pointer to LIST to modify
 * Return:
 *   Previous item in list 
 */
void* ListPrev(LIST* list) { 
  void * vPtr;
	
  if (list == NULL)
    return (void*) NULL;
  
  if (list -> size < 2) return NULL;

  if (list -> cur == list -> head) return NULL;

  list -> cur = list ->cur ->prev;

  vPtr = list -> cur -> data;
  return vPtr;


}

/*
 * Gets current item in list
 * Params:
 *   list: Pointer to LIST type
 * Return:
 *   A pointer to item at the current cursor position,NULL pointer if fail
 */
void* ListCurr(LIST* list) {
  void * vPtr;
	
  if (list == NULL)
    return (void*) NULL;
  
  if (list -> size < 1) return NULL;

  vPtr = list -> cur -> data;
  return vPtr;


}

/*
 * Searches list starting at cursor until match is found or end is
 * reached. 
 * Params:
 *   list: The list to search for item in
 *   comparator: Pointer to routine that performs item comparison
 *   comparisonArg: The item to be searched for in list
 * Post Condition:
 *   Cursor is left at matched item or end if not found
 * Return: 
 *   Pointer to item if found, NULL pointer if not
 *
 */
void *ListSearch(LIST* list, int (*comparator)(void* item, 
  void* comparisonArg), void* comparisonArg) {
  
  NODE * node;
  int retval;
  void * node_item;
  
  if (list == NULL)
	  return NULL;
  if (comparisonArg == NULL)
	 return NULL;
  if (list -> size == 0 )
	  return NULL;


  node = list -> cur;

  while (node){
	  node_item = node -> data;
	  retval = comparator(node_item,comparisonArg);

	  if (retval == 1) {
		  list -> cur = node;
	     return node_item;
	  }

	  node = node -> next;

  }

  list -> cur = list -> tail;
  return NULL;
}

