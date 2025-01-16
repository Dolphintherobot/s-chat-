/* John Thoms, gvr812, 11357558
 * Dalton Beauchesne,nqr159,11353160 */

#include<list.h>
#include<stddef.h>
#include<stdio.h>

/*
 * Gets current item and removes it from list. Moves cursor
 * to next item
 * Post Condition:
 *   Item at cursor is removed, cursor moved to next item
 *   If the item is at the tail of the list, the cursor will be
 *   moved to the previous item
 * Params:
 *   list: list to modify
 * Return:
 *   Item that was removed from list
 *   NULL Pointer if failed 
 */
void *ListRemove(LIST* list) {
  NODE* to_delete;
  NODE * prev_node;
  NODE * next_node;
  if (list == NULL )
    return NULL;


  if (list -> size == 0) return NULL;
  
  to_delete = list->cur;
  prev_node = to_delete -> prev;
  next_node = to_delete -> next;
  

  if (list -> size == 1) {
    list -> head = NULL;
    list -> tail = NULL;
    list -> cur = NULL;
  }


  else if (list->cur == list->head) {
    list->head = list->head->next;
    list->cur = list->head; 
  }

  else if (list -> cur == list ->tail){
	  list -> cur =prev_node ;
	  prev_node -> next = NULL;
	  list -> tail = prev_node;

  }

  else {
	  list -> cur = next_node;
	  next_node -> prev = prev_node;
	  prev_node -> next = next_node;
  }

  list -> size--;
  return to_delete -> data;
}

/*
 * Deletes the specified list
 * Post Condition:
 *   List is deleted
 * Params:
 *   list: pointer to LIST to delete
 *   itemFree: function pointer to routine that frees item
 * PostCon:
 *   if a precon is violated an error to standard out printed
 *   should dereference the pointer to the list after this call as
 *   function will not do it for you 
 */
void ListFree(LIST* list, void (*itemFree)(void* item)) {
  
  NODE * temp;
  NODE * current;
  void * item;
  
  if (list == NULL)
	  return;

  current = list -> head;
  list -> head = NULL;
  list -> tail = NULL;
  list -> size = 0;
  list -> cur = NULL;


  /*go through all nodes and handle their items
   * also put nodes back onto memory
   */
  while (current) {
  
	  item = current -> data;
	  if (itemFree) {
		  itemFree(item);
	  }

	  temp = current;
	  current = current -> next;

	 temp -> next =  free_nodes.head;
	  free_nodes.head = temp;
  
  }



  /* list is added back to the hacky chain of open list memory.
     Couldn't think of a better solution. */
  list->next_list_mem = free_lists;
  free_lists = list;


  

}

/*
 * Gets last item from list and reomves it, cursor is moved
 * to new last item
 * Post Cond:
 *   Last item is removed from given list
 * Params:
 *   list: pointer to LIST to modify
 * Return:
 *   Item that was removed from specified LIST
 *   NULL if failed 
 */
void *ListTrim(LIST* list) {
       void * item;
       NODE * node;
       if (list == NULL) return NULL;
       if (list -> size == 0) return NULL;


       /*Getting item to return, node to put back in open mem */
       node = list -> tail;
       item = node->data;

       list -> tail = list -> tail -> prev;

       if (list->tail != NULL)
         list->tail->next = NULL;

       /* If the tail is null, the list is empty now */
       else
         list->head = NULL;

       list->cur = list->tail;
       list -> size--;

       node->next = free_nodes.head;
       free_nodes.head = node;

       return item;
}

