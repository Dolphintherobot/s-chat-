/**
Dalton Beauchesne
nqr159 11353160
John Thoms
gvr812
11357558
**/

#include <list.h>
#include <stddef.h>
#include <stdio.h>

extern size_t list_mem;
extern size_t node_mem;

extern LIST* list_pool;
extern NODE* node_pool;

/**
 * Purpose: to add a item to a list after the current item
 * Param list: list of node data types 
 * Param item: pointer to the item 
 * return 0 if success -1 if invalid list, -2 if invalid item, -3 if out
 * of memory
 */
int ListAdd(LIST* list, void * item) {
  NODE* new_node;
  
  if (list == NULL)
    return -1;
  if (item == NULL)
    return -2;

  if (free_nodes.head == NULL) {
    return -3;
  }

  /* Grabbing node from open memory */
  new_node = free_nodes.head;
  free_nodes.head = free_nodes.head->next;
  new_node->data = item;

  /* first item in the list */
  if (list->size == 0) {
    new_node->next = NULL;
    new_node->prev = NULL;
    list->head = new_node;
  }
  else {
    new_node->next = list->cur->next;
    new_node->prev = list->cur;
    list->cur->next = new_node;
  }
  /* If it is the last in the list, it is the tail */
  if (new_node->next == NULL)
    list->tail = new_node;
  
  list->cur = new_node;
  list->size++;
  return 0;
}


/**
 * Purpose: insert an item directly before current item and makes new 
 * item current item
 * Param list: list of node data types
 * Param item: pointer to the item to be stored in the node
 * return 0 if successful,-1 if list is null, -2 if item is null, -3 if out
 * of memory 
 */
int ListInsert(LIST* list, void * item) {
  NODE* new_node;
  if (list == NULL)
    return -1;
  if (item == NULL)
    return -2;

  if (free_nodes.head == NULL)
    return -3;
  new_node = free_nodes.head;
  free_nodes.head = free_nodes.head->next;
  new_node->data = item;

  if (list->size == 0) {
    new_node->next = NULL;
    new_node->prev = NULL;
    list->tail = new_node;
  }

  else {
    new_node->next = list->cur;
    new_node->prev = list->cur->prev;
    /* setting current prev node's next to new node, if it exists */
    if (list->cur != list->head)
      list->cur->prev->next = new_node;

    list->cur->prev = new_node;
  }

  if(new_node->prev == NULL)
    list->head = list->cur;

  if (list ->size == 1) {
    list -> head -> prev = new_node;
  }

  if (list -> head == list ->cur)
    list -> head = new_node;
  
  list->cur = new_node;
  list->size++;  
  return 0;
}


/**
 * Purpose: appends an item to the list makes new 
 * item current item
 * Param list: list of node data types
 * Param item: pointer to the node to be stored in the list
 * return 0 if successful,-1 if invalid list, -2 if invalid item, -3 if out
 * of node memory 
 */
int ListAppend(LIST* list, void * item) {
  NODE* new_node;
  if (list == NULL)
    return -1;

  if (item == NULL)
    return -2;

  if (free_nodes.head == NULL) {
   return -3;   
  }

  /*grab a free node off the chain and set new head */
  new_node = free_nodes.head;
  free_nodes.head = free_nodes.head->next;
  new_node->data = item;

  /* End of list, will always be NULL */
  new_node->next = NULL;
  new_node->prev = list->tail;

  /*check if the list size is empty */
  if (list->size == 0 )
    list->head = new_node;

  else 
    list->tail->next = new_node;

  list->tail = new_node;
  list->cur = new_node;
  list->size++;

  return 0;
}


/**
 * Purpose: Concatinates 2 lists
 * precon: lists cannot be null or empty 
 * Param list1: list of node data types
 * Param list2: list of node data types
 * Postcon deletes list2 and sets currentitem to currentpoint of list1
 * Postcon: prints to stdout of precon is violated 
*/
void  ListConcat(LIST* list1, LIST* list2) {
  if (list1 == NULL) {
    printf("Error in procedure ListConcat: invalid parameter list1\n");
    return;
  }
  if (list2 == NULL) {
    printf("Error in procedure ListConcat: invalid parameter list2\n");
    return;  
  }

  if (list1 == list2 ) {
    printf("Error in procedure ListConcate: argument list1 and list2"
		    " are same\n");
	return;
  }

  if (list1 -> size == 0) {
    printf("Error in procedure ListConcat: passed in a list1 with"
		    " no nodes\n");

    return;
  }

  if (list2 -> size == 0) {
    printf("Error in procedure ListConcat: passed in a list2 with"
		   " no nodes\n");
    return;
  }

  list1->tail->next = list2->head;
  list2->head->prev = list1->tail;
  list1->tail = list2->tail;
  list1->size += list2->size;

  list2->head = NULL;
  list2->tail = NULL;
  list2->cur = NULL;
  list2->size = 0;
  
  /* list2 is added back to the hacky chain of open list memory.
     Couldn't think of a better solution. */
  list2->next_list_mem = free_lists;
  free_lists = list2;

  list2 = NULL;
  return;
}


/**
 * Purpose: insert an item at the start of the list, sets cursor to that
 * item
 * Param list: list of node data types
 * Param item: pointer to the item to be stored in the node
 * return 0 if successful,-1 otherwise 
 */
int ListPrepend(LIST* list, void * item) {
  NODE* new_node;
  if (list == NULL)
    return -1;
  if (item == NULL)
    return -2;
  if (free_nodes.head == NULL)
    return -3;
  new_node = free_nodes.head;
  free_nodes.head = free_nodes.head->next;

  new_node->data = item;
  new_node->prev = NULL;
  
  if (list->size == 0) {
    new_node->next = NULL;
    list->tail = new_node;
  }
  
  else{
    new_node->next = list->head;

  list -> head -> prev = new_node;
  }
  list->head = new_node;
  list->cur = new_node;
  
  list->size++;
  return 0;
}


