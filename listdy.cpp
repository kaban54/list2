#include "listdy.h"


#define ListErr(list, error)  {list -> err |= error;   \
                                ListPrintError (list); \
                                return list -> err; }


int List_ctor (List_t *list, const char *name, const char *func_name, const char *file_name, int line)
{
    if (list == nullptr) return LIST_NULLPTR_ARG;

    if (list -> status != LIST_CREATED) ListErr (list, LIST_STATUS_ERROR);

    list -> err |= List_set_info (list, name, func_name, file_name, line);

    list -> data = ListAllocElem ();
    if (list -> data == nullptr) ListErr (list, LIST_ALLOC_ERROR);

    list -> data -> next  = list -> data;
    list -> data -> prev  = list -> data;
    list -> data -> value = POISON_VAL;

    list -> status = LIST_CONSTRUCTED;

    ListVerify (list);

    return LIST_OK;
}

int List_set_info (List_t *list, const char *name, const char *func_name, const char *file_name, int line)
{
    if (list == nullptr) return LIST_NULLPTR_ARG;

    list -> info.     name =      name;
    list -> info.func_name = func_name;
    list -> info.file_name = file_name;
    list -> info.     line =      line;

    return LIST_OK;
}

ListElem_t *ListAllocElem (void)
{
    return (ListElem_t *) (calloc (1, sizeof (ListElem_t)));
}

int ListDtor (List_t *list)
{
    ListVerify (list);

    ListElem_t *elem = list -> data -> next;

    while (elem != list -> data)
    {
        elem = elem -> next;
        free (elem -> prev);
    }

    free (list -> data);
    list -> data = nullptr;
    
    list -> status = LIST_DECONSTRUCTED;

    return LIST_OK;
}


ListElem_t *ListGetHead (List_t *list)
{
    if (list == nullptr || list -> data == nullptr) return nullptr;
    return list -> data -> next;
}

ListElem_t *ListGetTail (List_t *list)
{
    if (list == nullptr || list -> data == nullptr) return nullptr;
    return list -> data -> prev;
}


int ListInsert (ListElem_t *prev, val_t value)
{
    if (prev == nullptr || prev -> next == nullptr) return LIST_NULLPTR_ARG;

    ListElem_t *newelem = ListAllocElem ();
    if (newelem == nullptr) return LIST_ALLOC_ERROR;

    ListElem_t *next = prev -> next;

    newelem -> prev  = prev;
    newelem -> next  = next;
    newelem -> value = value;

    prev -> next = newelem;
    next -> prev = newelem;

    return LIST_OK;
}

int ListInsertHead (List_t *list, val_t value)
{
    if (list == nullptr) return LIST_NULLPTR_ARG;
    return ListInsert (list -> data, value);
}

int ListInsertTail (List_t *list, val_t value)
{
    return ListInsert (ListGetTail (list), value);
}

int ListRemove (ListElem_t *elem)
{
    if (elem == nullptr || elem -> next == nullptr || elem -> prev == nullptr) return LIST_NULLPTR_ARG;
    if (elem -> next == elem || elem -> prev == elem) return LIST_INCORRECT_ARGS;

    elem -> prev -> next = elem -> next;
    elem -> next -> prev = elem -> prev;

    free (elem);

    return LIST_OK;
}

int ListRemoveHead (List_t *list)
{
    return ListRemove (ListGetHead (list));
}

int ListRemoveTail (List_t *list)
{
    return ListRemove (ListGetTail (list));
}


int ListGetIndex (List_t *list, ListElem_t *elem, int *index)
{
    ListVerify (list);

    if (elem == nullptr || index == nullptr) ListErr (list, LIST_NULLPTR_ARG);

    ListElem_t *listelem = list -> data;
    int counter = 0;

    while (1)
    {
        if (elem == listelem) break;
        if (listelem == nullptr) ListErr (list, LIST_DATA_CORRUPTED);
        listelem = listelem -> next;
        if (listelem == list -> data) ListErr (list, LIST_INCORRECT_ARGS);
    }

    *index = counter;
    return LIST_OK;
}


int List_verify (List_t *list)
{
    #ifdef NDEBUG
    return 0;
    #else

    if (list == nullptr) return LIST_NULLPTR_ARG;

    if (list -> status != LIST_CONSTRUCTED) list -> err |= LIST_STATUS_ERROR;

    if (list -> info.     name == nullptr ||
        list -> info.file_name == nullptr ||
        list -> info.func_name == nullptr   ) list -> err |= LIST_INFO_CORRUPTED;

    list -> err |= List_verify_data (list);

    return list -> err;
    #endif
}

int List_verify_data (List_t *list)
{
    if (list -> data == nullptr)             return LIST_DATA_CORRUPTED;
    if (list -> data -> value != POISON_VAL) return LIST_DATA_CORRUPTED;

    ListElem_t *elem = list -> data;
    while (1)
    {
        if (elem -> next == nullptr || elem -> prev == nullptr) return LIST_DATA_CORRUPTED;
        if (elem -> next -> prev != elem)                       return LIST_DATA_CORRUPTED;

        elem = elem -> next;
        if (elem == list -> data) return LIST_OK;
    }
}

void List_print_error (List_t *list, const char *func_name, const char *file_name, int line)
{
    int err = list == nullptr ? LIST_NULLPTR_ARG : list -> err;
    
    printf ("\nERROR (%d) in (%s) at (%s) at line (%d).\n", err, func_name, file_name, line);

    List_dump (list, func_name, file_name, line);
}
