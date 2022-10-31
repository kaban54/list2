#include "listdy.h"

FILE *LOG = NULL;

int main()
{
    LOG = fopen (LOGFILENAME, "w");
    if (LOG == nullptr) return 0;
    fprintf (LOG, "<pre>\n");

    List_t list = {};

    ListCtor (&list);

    fprintf (LOG, "<h2>LIST CONSTRUCTED</h2>");
    ListDump (&list);

    ListInsertHead (&list, 1);
    fprintf (LOG, "<hr><h2>INSERT 1 IN HEAD</h2>");
    ListDump (&list);

    ListInsertHead (&list, 2);
    fprintf (LOG, "<hr><h2>INSERT 2 IN HEAD</h2>");
    ListDump (&list);

    ListInsertTail (&list, 3);
    fprintf (LOG, "<hr><h2>INSERT 3 IN TAIL</h2>");
    ListDump (&list);

    ListRemoveHead (&list);
    fprintf (LOG, "<hr><h2>REMOVE HEAD</h2>");
    ListDump (&list);

    ListInsertTail (&list, 4);
    fprintf (LOG, "<hr><h2>INSERT 4 IN TAIL</h2>");
    ListDump (&list);

    ListInsert (ListGetElem (&list, 2), 5);
    fprintf (LOG, "<hr><h2>INSERT 5 AFTER SECOND ELEM</h2>");
    ListDump (&list);

    ListDtor (&list);

    fclose (LOG);
    return 0;
}