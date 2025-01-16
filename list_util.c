/* John Thoms, gvr812, 11357558
 * Dalton Beauchesne,nqr159,11351360 */

#include<list.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

/* setting the macros to variables, in case we do the realloc part */
size_t list_mem = LIST_MEM;
size_t node_mem = NODE_MEM;
LIST free_nodes;
LIST* free_lists;
/* Allocating space for lists and nodes, variables to keep track of open
 * memory. Should these go in header? */
LIST* list_pool = NULL;
LIST* re_list_pool;
NODE* node_pool = NULL;
NODE* re_node_pool;


/*
 * Makes new, empty list
 * Return: 
 *   LIST reference on success, NULL pointer on failure
 */
LIST* ListCreate() {
  size_t i;
  LIST * new_list= NULL;
   
   /* Seeing if any lists have been made yet. If not, we have to
   allocate the space. */

  if (list_pool == NULL) {
    list_pool = (LIST*) malloc(sizeof(LIST) * list_mem);
     if (list_pool == NULL) {
       return NULL;
    }
    node_pool = (NODE*) malloc(sizeof(NODE) * node_mem);
    if (node_pool == NULL) {
      free(list_pool);
      return NULL;
    }

  /* Linking each list together, chain acts as a pool of open memory */
  for (i = 0; i < list_mem-1; i++)
    list_pool[i].next_list_mem = &list_pool[i+1];

  list_pool[list_mem-1].next_list_mem = NULL;
  free_lists = list_pool;

   
  /* Putting each allocated node in a linked list of 'free' nodes, up
     for grabs. */
    for (i = 0; i < node_mem-1; i++)
      node_pool[i].next = &node_pool[i+1];

    node_pool[node_mem -1].next = NULL;
    free_nodes.head = node_pool;
  }

   if (free_lists == NULL)
     return NULL;

   new_list = free_lists;
   free_lists = free_lists->next_list_mem;
  
  /* Pretty sure you only need to init one struct var, the rest will
   automatically set to 0/null, but just making sure. */

  new_list->next_list_mem = NULL; /* NEVER ACCESS THIS IN NORMAL OPERATIONS!*/
  new_list->head = NULL;
  new_list->head = NULL;
  new_list->tail = NULL;
  new_list->size = 0;

  return new_list;
}

/*
 * Gets number of items currently in list
 * Params:
 *   list: Pointer to LIST to get num of items of
 * Return:
 *   Number of items in list, -1 on failure.
 */
int ListCount(LIST* list) {
  if (list == NULL)
  return -1;
  return list->size;
}

