/* John Thoms, gvr812, 11357558 */
/* Dalton Beauchesne,nqr159,11353160 */

#ifndef LIST_H
#define LIST_H

#define LIST_MEM 10
#define NODE_MEM 1000


/*likely don't have to store strings, that would be tough without
  malloc */
/*typedef enum type {int, long, float, double, char, boolean } type;*/
  

typedef struct NODE { 
  void* data;
  struct NODE* prev; /*The previous node, doubly linked list*/
  struct NODE* next; /*The next node in the list*/
} NODE;

typedef struct LIST{
  struct LIST* next_list_mem; /* no acutal list functionality, for memory */
  NODE* head;
  NODE* tail;
  NODE* cur; /*The current element of the list, cursor */
  int size; /*current number of elements in the list */
} LIST;


extern LIST free_nodes;
extern LIST* free_lists;

/* These functions don't make a ton of sense in any of the recommended
 * source files, making fourth file called list_util */

/* list_util.c */
LIST* ListCreate();
/* I assume the lists should be passed into functions as pointers */
int ListCount(LIST* list);

/* list_adders.c */
int ListAdd(LIST* list, void* item);
int ListInsert(LIST* list, void * item);
int ListAppend(LIST* list, void * item);
int ListPrepend(LIST* list, void* item);
void ListConcat(LIST* list1, LIST* list2);


/* list_movers.c */
void* ListFirst(LIST* list);
void* ListLast(LIST* list);
void* ListNext(LIST* list);
void* ListPrev(LIST* list);
void* ListCurr(LIST* list);
void *ListSearch(LIST* list, int (*comparator)(void* item, 
  void* comparisonArg), void* comparisonArg);

/* list_removers.c */
void *ListRemove(LIST* list);
void ListFree(LIST* list, void (*itemFree)(void* item));
void *ListTrim(LIST* list);

#endif

