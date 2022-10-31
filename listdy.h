#ifndef LIST_H
#define LIST_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int val_t;


const char *const LOGFILENAME = "listlog.html";

const val_t POISON_VAL   = 0xF5410007;

extern FILE *LOG;

struct ListInfo_t
{
    const char*      name;
    const char* func_name;
    const char* file_name;
    int              line;
};

struct ListElem_t
{
    val_t value;
    ListElem_t *next;
    ListElem_t *prev;
};

struct List_t
{
    ListInfo_t info;
    int status;
    int err;

    ListElem_t *data;
};

enum LISTERRORS
{
    LIST_OK                 =   0,
    LIST_NULLPTR_ARG        =   1,
    LIST_ALLOC_ERROR        =   2,
    LIST_INCORRECT_INDEX    =   4,
    LIST_INCORRECT_CAPACITY =   8,
    LIST_STATUS_ERROR       =  16,
    LIST_DATA_CORRUPTED     =  32,
    LIST_INFO_CORRUPTED     =  64,
    LIST_INCORRECT_ARGS     = 128,  
};


enum LISTSTATUS
{
    LIST_CREATED        = 0,
    LIST_CONSTRUCTED    = 1,
    LIST_DECONSTRUCTED  = 2,
};


#define ListCtor(list) List_ctor (list, #list, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define ListTxtDump(list, stream) List_txt_dmup (list, stream, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define ListDump(list) List_dump (list, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define ListVerify(list)   {int _list_verify_err = List_verify (list);  \
                            if (_list_verify_err)                       \
                            {                                           \
                                ListPrintError (list);                  \
                                return _list_verify_err;                \
                            }}

#define ListPrintError(list) List_print_error (list, __PRETTY_FUNCTION__, __FILE__, __LINE__)
                         

int List_ctor (List_t *list, const char *name, const char *func_name, const char *file_name, int line);

int List_set_info (List_t *list, const char *name, const char *func_name, const char *file_name, int line);

ListElem_t *ListAllocElem (void);

int ListDtor (List_t *list);

ListElem_t *ListGetHead (List_t *list);

ListElem_t *ListGetTail (List_t *list);

int ListInsert (ListElem_t *prev, val_t value);

int ListInsertHead (List_t *list, val_t value);

int ListInsertTail (List_t *list, val_t value);

int ListRemove (ListElem_t *elem);

int ListRemoveHead (List_t *list);

int ListRemoveTail (List_t *list);

int ListGetIndex (List_t *list, ListElem_t *elem, int *index);

int List_verify (List_t *list);

int List_verify_data (List_t *list);

void List_print_error (List_t *list, const char *func_name, const char *file_name, int line);

void List_txt_dmup (List_t *list, FILE *stream, const char *func_name, const char *file_name, int line);

void List_dump (List_t *list, const char *func_name, const char *file_name, int line);

void Generate_img (List_t *list, int imgnum);

#endif