#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int INF = 9999999;                // the distance is not likely to be more than 9999999 KM
const int EXIT_FILEOPEN_FAILURE = 1;    // Exit code for file open error
const int EXIT_FILECOTENT_FAILURE = 2;  // Exit code for file content invalid
const int EXIT_MALLOC_FAILURE = 3;      // Exit code for any memory allocation error
const int EXIT_OTHER_FAILURE = 4;       // Exit code for other failure

//---------------------------------------//
void memoryCheck(int i)
{
    if (i)
    {
        printf("Unable to allocate memory.\n");
        exit(EXIT_MALLOC_FAILURE);
    }
}

// prompt from moodle
char *prompt(const char *mesg)
{
    char *name;
    int size = 8;
    name = malloc(sizeof(char) * size);
    memoryCheck(name == NULL);

    printf("%s", mesg);

    int pos = 0;
    char ch;
    do
    {
        scanf("%c", &ch);
        if (ch != '\n') // User did not press return/enter.
        {
            name[pos] = ch;
            pos++;
            if (pos > size - 1)
            {
                // expand
                char *tmp = malloc(sizeof(char) * (size * 2));
                if (tmp == NULL)
                {
                    free(name); // free the current buffer before giving up.
                    return NULL;
                }
                for (int i = 0; i < size; i++)
                {
                    tmp[i] = name[i];
                }
                free(name);
                name = tmp;
                size = size * 2;
            }
        }
    } while (ch != '\n');

    name[pos] = '\0'; // terminate the string after the return/enter.

    return name;
}

//---------------------------------------//
// StringList
// This is used for graph

typedef struct string_list
{
    char *value;
    struct string_list *next;
} StringList;

void list_print(StringList *start)
{
    if (start == NULL)
    {
        printf("[]\n");
        return;
    }
    StringList *tmp = start;
    printf("[");
    while (tmp->next != NULL)
    {
        printf("%s,", tmp->value);
        tmp = tmp->next;
    }
    printf("%s]\n", tmp->value);
}

int list_append(StringList **start, char *val)
{
    if (val == NULL)
    {
        exit(EXIT_OTHER_FAILURE);
    }
    if (*start == NULL)
    {
        StringList *tmp = malloc(sizeof(StringList));
        memoryCheck(tmp == NULL);
        tmp->value = val;
        tmp->next = NULL;
        *start = tmp;
    }
    else
    {
        StringList *last = *start;
        while (last->next != NULL)
        {
            last = last->next;
        }
        StringList *tmp = malloc(sizeof(StringList));
        memoryCheck(tmp == NULL);
        tmp->value = val;
        tmp->next = NULL;
        last->next = tmp;
    }
    return 1;
}

void list_free(StringList **start)
{
    StringList *tmp = *start;
    while (tmp != NULL)
    {
        StringList *next = tmp->next;
        free(tmp);
        tmp = next;
    }
    *start = NULL;
}

char *list_get(StringList *start, int index)
{
    if (start == NULL)
    {
        exit(EXIT_OTHER_FAILURE);
    }
    StringList *tmp = start;
    if (index < 0)
    {
        return NULL;
    }
    while (tmp != NULL)
    {
        if (index == 0)
        {
            return tmp->value;
        }
        else
        {
            index--;
            tmp = tmp->next;
        }
    }
    return NULL;
}

int list_index(StringList *start, char *val)
{
    int i = 0;
    StringList *cur = start;
    while (cur != NULL)
    {
        if (strcmp(cur->value, val) == 0)
        {
            return i;
        }
        cur = cur->next;
        i++;
    }
    return -1;
}

int list_length(StringList *start)
{
    int len = 0;
    StringList *cur = start;
    while (cur != NULL)
    {
        len++;
        cur = cur->next;
    }
    return len;
}

//---------------------------------------//
// Adjacency matrix graph

typedef struct adj_matrix_graph
{
    StringList *vertex_names;
    int *edge_array;
} Graph;

Graph *graph_create()
{
    Graph *graph = malloc(sizeof(Graph));
    memoryCheck(graph == NULL);
    graph->vertex_names = NULL;
    graph->edge_array = NULL;
    return graph;
}

int graph_add_vertex(Graph *graph, char *vertex_name)
{
    if (graph == NULL || vertex_name == NULL)
    {
        exit(EXIT_OTHER_FAILURE);
    }
    // vertex already in list
    if (list_index(graph->vertex_names, vertex_name) != -1)
    {
        return 0;
    }
    int len_ori = list_length(graph->vertex_names);
    int len_new = len_ori + 1;
    int *edge_array_new = malloc(sizeof(int) * len_new * len_new);
    memoryCheck(edge_array_new == NULL);
    for (int i = 0; i < len_new * len_new; i++)
    {
        edge_array_new[i] = INF;
    }
    for (int i = 0; i < len_ori; i++)
    {
        for (int j = 0; j < len_ori; j++)
        {
            edge_array_new[i * len_new + j] = graph->edge_array[i * len_ori + j];
        }
    }
    list_append(&(graph->vertex_names), vertex_name);
    free(graph->edge_array);
    graph->edge_array = edge_array_new;
    return 1;
}

int graph_add_edge(Graph *graph, char *from_vertex, char *to_vertex,
                   int value)
{
    if (graph == NULL || from_vertex == NULL || to_vertex == NULL)
    {
        return 0;
    }
    int from_index = list_index(graph->vertex_names, from_vertex);
    int to_index = list_index(graph->vertex_names, to_vertex);
    int len = list_length(graph->vertex_names);
    if (from_index == -1 || to_index == -1)
    {
        return -1;
    }
    graph->edge_array[len * from_index + to_index] = value;
    return 1;
}

void graph_print(Graph *graph)
{
    list_print(graph->vertex_names);
    int len = list_length(graph->vertex_names);
    for (int i = 0; i < len; i++)
    {
        printf("%10s\t", list_get(graph->vertex_names, i));
        for (int j = 0; j < len; j++)
        {
            printf("%5d\t", graph->edge_array[len * i + j]);
        }
        printf("\n\n");
    }
}

void graph_free(Graph *graph)
{
    if (graph == NULL)
    {
        return;
    }

    list_free(&(graph->vertex_names));
    if (graph->edge_array != NULL)
    {
        free(graph->edge_array);
    }
    free(graph);
}
//---------------------------------------//
// Function for process file input

// read a singel line in file
// modified from prompt function

int str2int(char *str)
{
    int res = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

int process_cell(int coloum, int row, char *value,
                char **from_station, Graph *graph)
{
    if ((coloum > list_length(graph->vertex_names) + 1 ||
         row > list_length(graph->vertex_names) + 1) &&
        coloum >= 2)
    {
        printf("Invalid distances file.\n");
        exit(EXIT_FILECOTENT_FAILURE);
    }
    int len = strlen(value);
    if (len == 0)
    {
        return 0;
    }
    if (coloum == 1)
    {
        char *station_name_tmp = malloc(sizeof(char) * (len + 1));
        memoryCheck(station_name_tmp == NULL);
        strcpy(station_name_tmp, value);
        if (graph_add_vertex(graph, station_name_tmp) == 0){
            printf("Station Duplicate.\n");
            exit(EXIT_OTHER_FAILURE);
        }
    }
    else if (coloum >= 2 && row == 1)
    {
        char *from_station_tmp = malloc(sizeof(char) * (len + 1));
        memoryCheck(from_station_tmp == NULL);
        strcpy(from_station_tmp, value);
        *from_station = from_station_tmp;
    }
    else if (coloum >= 2 && row != 1)
    {
        char *from_station_tmp = *from_station;
        char *to_station = list_get(graph->vertex_names, row - 2);
        int distance = str2int(value);
        if (graph_add_edge(graph, from_station_tmp, to_station, distance) == -1)
        {
            printf("Invalid distances file.\n");
            exit(EXIT_FILECOTENT_FAILURE);
        };
    }
    return 1;
}

int read_file(const char *filename, Graph *graph)
{
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Cannot open file.");
        exit(EXIT_FILEOPEN_FAILURE);
    }

    int pos = 0;
    int coloum = 1;
    int size = 16;
    int row = 1;
    char *tmp = malloc(size * sizeof(char));
    memoryCheck(tmp == NULL);
    char *from_station = NULL;
    char ch;

    while (!feof(fp))
    {
        fscanf(fp, "%c", &ch);
        if (feof(fp))
        {
            free(tmp);
            free(from_station);
            continue;
        }
        if (ch == '\n' || ch == ',')
        {
            tmp[pos] = '\0';
            //printf("%-12s %p %d %d %d\n", tmp, tmp, row, coloum, pos);
            process_cell(coloum, row, tmp, &from_station, graph);
            free(tmp);
            size = 16;
            pos = 0;
            tmp = malloc(size * sizeof(char));
            memoryCheck(tmp == NULL);
            if (ch == '\n')
            {
                coloum++;
                row = 1;
            }
            else if (ch == ',')
            {
                row++;
            }
        }
        else
        {
            tmp[pos] = ch;
            pos++;
            if (pos >= size)
            {
                size = size * 2;
                void *realloc_tmp = realloc(tmp, sizeof(char) * size);
                memoryCheck(realloc_tmp == NULL);
                tmp = realloc_tmp;
            }
        }
    }
    // coloum is 1 but file reach to the end
    if (coloum == 1)
    {
        printf("Invalid distances file.\n");
        exit(EXIT_FILECOTENT_FAILURE);
    }
    fclose(fp);
    return 1;
}

int shortest_dijkstra(Graph *graph, char *from_station, char *to_station)
{
    int from_index = list_index(graph->vertex_names, from_station);
    int to_index = list_index(graph->vertex_names, to_station);
    int len = list_length(graph->vertex_names);
    int d[len], path[len], visit[len];
    for (int i = 0; i < len; i++)
    {
        d[i] = graph->edge_array[len * to_index + i];
        path[i] = to_index;
        visit[i] = 0;
    }
    visit[to_index] = 1;
    d[to_index] = 0;
    for (int i = 1; i < len; i++)
    {
        int min_num = -1;
        int min = INF;
        for (int j = 0; j < len; j++)
        {
            if (visit[j] == 0 && d[j] <= min)
            {
                min = d[j];
                min_num = j;
            }
        }
        visit[min_num] = 1;
        for (int j = 0; j < len; j++)
        {
            // printf("%d %d %d\n",min_num,min,graph->edge_array[len * min_num + j]);
            if (d[j] > min + graph->edge_array[len * min_num + j])
            {
                path[j] = min_num;
                d[j] = min + graph->edge_array[len * min_num + j];
            }
        }
    }
    if (d[from_index] >= INF)
    {
        printf("No possible journey.\n");
    }
    else if (from_index == to_index)
    {
        printf("No journey, same start and end station.\n");
    }
    else if (path[from_index] == to_index)
    {
        printf("From %s\ndirect\nTo %s\n", from_station, to_station);
        printf("Distance %d km\n", d[from_index]);
    }
    else
    {
        printf("From %s\nvia\n", from_station);
        int i = path[from_index];
        while (i != to_index)
        {
            printf("%s\n", list_get(graph->vertex_names, i));
            i = path[i];
        }
        printf("To %s\n", to_station);
        printf("Distance %d km\n", d[from_index]);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // no argument
    if (argc != 2)
    {
        printf("Invalid command line arguments. Usage: train <disances.txt>");
        exit(EXIT_FILEOPEN_FAILURE);
    }
    Graph *graph = graph_create();
    read_file(argv[1], graph);
    //graph_print(graph);
    /**
    int len = list_length(graph->vertex_names);
    //test
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            shortest_dijkstra(graph, list_get(graph->vertex_names, i),
                              list_get(graph->vertex_names, j));
        }
    }
    **/
    while (1)
    {
        char *from_station = prompt("Start station: ");
        if (from_station == NULL || strlen(from_station) == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else if (list_index(graph->vertex_names, from_station) < 0)
        {
            printf("No such station.\n");
            continue;
        }
        char *to_station = prompt("End station: ");
        if (list_index(graph->vertex_names, to_station) < 0)
        {
            printf("No such station.\n");
            continue;
        }
        shortest_dijkstra(graph, from_station, to_station);
    }
    graph_free(graph);

}