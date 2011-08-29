typedef int item_t;

typedef struct node
{
    struct node *next, *prev;
    item_t item;
} node_t;

typedef struct list
{
    node_t *head, *tail;
    int len;
} list_t;

/* Return list allocated and initialized or abort if malloc fails. */
list_t* list_init();

/* Remove all nodes from list */
void list_empty(list_t *list);

/* Free the contents of list. */
void list_free(list_t *list);

/* Insert item in position pos of list. Return 0 if success, 1 if
 * position invalid, and abort if malloc fails. */
int list_insert(list_t *list, const int pos, const item_t item);

/* Insert item at the end of list. */
void list_append(list_t *list, const item_t item);

/* Remove from list first instance of item. Return 1 if item not found. */
int list_remove(list_t *list, const item_t item);

/* Remove and return node in position pos of the list. Return NULL if
 * position invalid. */
node_t* list_pop(list_t *list, const int pos);

/* Return index of first match of item. Return -1 if item not found. */
int list_index(list_t list, const item_t item);

/* Return count of how many times item appears in list. */
int list_count(list_t list, const item_t item);

/* Swap items in positions posA and posB. Return 0 if success and 1 if
 * postions invalid. */
int list_swap(list_t *list, int posA, int posB);

/* Invert the contents of a list inplace. */
void list_reverse(list_t *list);

/* Appends the list source to destiny. */
void list_concat(list_t *destiny, list_t *source);

/* Copy the contents of source to destiny. */
void list_copy(list_t *destiny, list_t *source);

/* Print the contents of a list with separator between each item. */
void list_fprint(FILE* output, list_t *list, const char *separator);
void list_fprintrev(FILE* output, list_t *list, const char *separator);
