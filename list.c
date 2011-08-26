#include <stdio.h>
#include <stdlib.h>
#ifndef LIST_H
#define LIST_H
#include "list.h"
#endif

#define CHKMLC(x) \
    if ((x) == NULL){ \
        fprintf(stderr, "Malloc Failed\n"); \
        abort();}

list_t* list_init()
{
    list_t *test;

    CHKMLC(test = malloc(sizeof(list_t)));
    CHKMLC(test->head = malloc(sizeof(node_t)));

    test->tail = test->head;
    test->head->next = NULL;
    test->head->prev = NULL;
    test->head->item = 0;
    test->len = 0;
    return test;
}

void list_empty(list_t *list)
{
    int i;
    node_t *aux;

    for (i = 0; i < list->len; i++){
        aux = list->head->next;
        list->head->next = aux->next;
        free(aux);
    }
    list->tail = list->head;
    list->head->next = NULL;
    list->head->item = 0;
    list->len = 0;
}

void list_free(list_t *list)
{
    list_empty(list);
    free(list->head);
    free(list);
    list = NULL;
}

int list_insert(list_t *list, int pos, const item_t item)
{
    int i;
    node_t *aux, *newNode;
    CHKMLC(newNode = malloc(sizeof(node_t)));

    /* Invalid position. */
    if (pos < 0 || pos > list->len)
        return 1;

    /* Insert at end. */
    if (pos == list->len){
        list->tail->next = newNode;
        newNode->next = NULL;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    /* Any other valid position. */
    else {
        aux = list->head;
        for (i = 0; i < pos; i++)
            aux = aux->next;
        newNode->next = aux->next;
        newNode->prev = aux;
        newNode->next->prev = newNode;
        aux->next = newNode;
    }

    newNode->item = item;
    list->len++;
    return 0;
}

void list_append(list_t *list, const item_t item)
{
    list_insert(list, list->len, item);
}

int list_remove(list_t *list, const item_t item)
{
    int i;
    node_t *aux = list->head->next;

    for (i = 0; i < list->len; i++){
        if (aux->item == item){
            /* Removing from end. */
            if (aux->next == NULL)
                list->tail = aux->prev;
            else
                aux->next->prev = aux->prev;
            aux->prev->next = aux->next;
            free(aux);
            aux = NULL;
            list->len--;
            return 0;
        }
        aux = aux->next;

    }
    return 1;
}

node_t* list_pop(list_t *list, int pos)
{
    int i;
    node_t *aux = list->head->next;

    /* Invalid position. */
    if (pos < 0 || pos >= list->len)
        return NULL;

    for (i = 0; i < pos; i++)
        aux = aux->next;
    /* Removing from end. */
    if (aux->next == NULL)
        list->tail = aux->prev;
    else
        aux->next->prev = aux->prev;
    aux->prev->next = aux->next;
    list->len--;
    return aux;
}

int list_index(list_t list, const item_t item)
{
    int i;
    node_t *aux = list.head->next;

    for (i = 0; i < list.len; i++){
        if (aux->item == item)
            return i;
        aux = aux->next;
    }
    /* Not found. */
    return -1;
}

int list_count(list_t list, const item_t item)
{
    int i, count = 0;
    node_t *aux = list.head->next;

    for (i = 0; i < list.len; i++){
        if (aux->item == item)
            count++;
        aux = aux->next;
    }
    return count;
}

int list_swap(list_t *list, int posA, int posB)
{
    int tmp, i;
    node_t *auxA, *auxB, *auxTmpA, *auxTmpB;

    /* Invalid positions. */
    if (posA < 0 || posA >= list->len ||
            posB < 0 || posB >= list->len)
        return 1;

    /* Ensure posA always before posB. */
    if (posA > posB){
        tmp = posA;
        posA = posB;
        posB = tmp;
    }

    /* No point in swapping with itself. */
    if (posA == posB || posA == list->len - 1)
        return 0;

    /* Navigate. */
    auxA = list->head->next;
    for (i = 0; i < posA; i++)
        auxA = auxA->next;
    auxB = auxA;
    for (; i < posB; i++)
        auxB = auxB->next;
    
    /* Drowning in pointers. */
    auxTmpA = auxA->prev;
    auxTmpB = auxB->prev;
    auxTmpA->next = auxB;
    auxTmpB->next = auxA;
    auxA->prev = auxTmpB;
    auxB->prev = auxTmpA;

    auxTmpA = auxA->next;
    auxTmpB = auxB->next;
    auxTmpA->prev = auxB;
    if (posB == list->len - 1)
        list->tail = auxA;
    else
        auxTmpB->prev = auxA;
    auxA->next = auxTmpB;
    auxB->next = auxTmpA;
    return 0;
}

void list_reverse(list_t *list)
{
    int i;
    node_t *aux = list->head;

    for (i = 0; i < list->len; i++){
        list->tail->next = list->head->next;
        list->head->next->prev = list->tail;
        list->head->next = list->tail;
        list->tail = list->tail->prev;
        list->head->next->prev = list->head;
        list->head = list->head->next;
    }
    list->tail->next = NULL;
    list->head = aux;
}

void list_print(list_t list, const char *separator)
{
    int i;
    node_t *aux = list.head->next;

    if (list.len == 0)
        return;
    for (i = 0; i < list.len - 1; i++){
        printf("%i%s", aux->item, separator);
        aux = aux->next;
    }
    printf("%i", aux->item);
}

void list_printrev(list_t list, const char *separator)
{
    int i;
    node_t *aux = list.tail;

    if (list.len == 0)
        return;
    for (i = 0; i < list.len - 1; i++){
        printf("%i%s", aux->item, separator);
        aux = aux->prev;
    }
    printf("%i", aux->item);
}

void list_concat(list_t *destiny, list_t *source)
{
    int i;
    node_t *aux;
    aux = source->head->next;
    for (i = 0; i < source->len; i++){
        list_append(destiny, aux->item);
        aux = aux->next;
    }
}

void list_copy(list_t *destiny, list_t *source)
{
    list_empty(destiny);
    list_concat(destiny, source);
}
