#include "listdy.h"

const char *const IMGNUMFILE = "imgnum.txt";
const char *const  GRAPHFILE =  "graph.txt";

const char *const NEXTCOLOR = "blue";
const char *const PREVCOLOR = "red";

void List_txt_dmup (List_t *list, FILE *stream, const char *func_name, const char *file_name, int line)
{
    if (stream == nullptr) stream = stdout;

    if (func_name == nullptr) func_name = "(NULL)";
    if (file_name == nullptr) file_name = "(NULL)";

    fprintf (stream, "\nList dump from (%s) at (%s) at line (%d):\n", func_name, file_name, line);

    if (list == nullptr)
    {
        fprintf (stream, "Unknown list (nullptr).\n");
        return;
    }

    if (list -> err & LIST_INFO_CORRUPTED) fprintf (stream, "Unknown list (info corrupted):\n");
    else fprintf (stream, "list [%p] \"%s\" at (%s) at (%s)(%d):\n", list, list -> info.     name, list -> info.func_name, 
                                                                           list -> info.file_name, list -> info.     line);
    fprintf (stream, "{\n");
    fprintf (stream, "\tstatus   = %d\n", list -> status);
    fprintf (stream, "\terror    = %d\n", list -> err);
    

    fprintf (stream, "\n\tdata[%p]:\n", list -> data);
    if (list -> data == nullptr) return;
    
    fprintf (stream, "\thead [%p]\n", list -> data -> next);
    fprintf (stream, "\ttail [%p]\n", list -> data -> prev);

    fprintf (stream, "\n");

    fprintf (stream, "\tindex: ");

    int index = 0;
    ListElem_t *elem = list -> data;
    while (1) 
    {
        fprintf (stream, "  %4d", index++);
        elem = elem -> next;
        if (elem == list -> data) break;
    }

    elem = list -> data;
    fprintf (stream, "\n\tvalue: ");
    while (1)
    {
        if (elem -> value == POISON_VAL) fprintf (stream, "   PSN");
        else fprintf (stream, "  %4d", elem -> value);
        elem = elem -> next;
        if (elem == list -> data) break;
    }

    fprintf (stream, "\n}\n");
}


void List_dump (List_t *list, const char *func_name, const char *file_name, int line)
{
    fprintf (LOG, "<pre>\n");
    List_txt_dmup (list, LOG, func_name, file_name, line);

    if (list != nullptr && list -> data != nullptr)
    {
        int imgnum = 0;
        FILE *numfile = fopen (IMGNUMFILE, "r");
        if (numfile != nullptr)
        {
            fscanf (numfile, "%d", &imgnum);
            fclose (numfile);
            numfile = fopen (IMGNUMFILE, "w");
            fprintf (numfile, "%d", imgnum + 1);
            fclose (numfile);
        }
        Generate_img (list, imgnum);
        fprintf (LOG, "<img src=\"./images/dumpimg%d.png\", width=\"80%%\">", imgnum);
    }
}


void Generate_img (List_t *list, int imgnum)
{
    FILE *graph = fopen (GRAPHFILE, "w");
    if (graph == nullptr) return;

    fprintf (graph, "digraph {\n rankdir = LR;\n"
                    "node [shape = record, fontsize = 12, style = \"rounded, filled\", fillcolor = lightblue];\n"
                    "graph [splines = true];\n");
    
    fprintf (graph, "elem0 [color = red, style = \"rounded, filled\", fillcolor = white, label = \"index: 0|");
    if (list -> data [0].value == POISON_VAL)   fprintf (graph, "value = PSN");
    else                                        fprintf (graph, "value = %d", list -> data -> value);
    fprintf (graph, "|adress: %p\"];\n", list -> data);

    int index = 1;
    ListElem_t *elem = list -> data -> next;    
    while (elem != list -> data)
    {
        fprintf (graph, "elem%d -> elem%d [style = invis, weight = 1000];\n", index - 1, index);
        fprintf (graph, "elem%d [label = \"index: %d|", index, index);
        if (list -> data [index].value == POISON_VAL)   fprintf (graph, "value = PSN");
        else                                            fprintf (graph, "value = %d", elem -> value);
        fprintf (graph, "|adress: %p\"];\n", elem);
        fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", index - 1, index, NEXTCOLOR);
        fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", index, index - 1, PREVCOLOR);

        elem = elem -> next;
        index++;
    }

    index -= 1;
    fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", index, 0, NEXTCOLOR);
    fprintf (graph, "elem%d -> elem%d [color = %s, weight = 1];\n", 0, index, PREVCOLOR);

    fprintf (graph, "}");

    fclose (graph);

    char cmd [64] = "";
    sprintf (cmd, "dot -T png -o ./images/dumpimg%d.png %s", imgnum, GRAPHFILE);
    system (cmd);
}