#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int INF = 9999999;               // the distance is not likely to be more than 9999999 KM
const int EXIT_FILEOPEN_FAILURE = 1;   // Exit code for file open error
const int EXIT_FILECOTENT_FAILURE = 2; // Exit code for file content invalid
const int EXIT_MALLOC_FAILURE = 3;     // Exit code for any memory allocation error
const int EXIT_OTHER_FAILURE = 4;      // Exit code for other failure

//---------------------------------------//
// check whether memory is allocated correctly
void memoryCheck(int i)
{
    if (i)
    {
        printf("Unable to allocate memory.\n");
        exit(EXIT_MALLOC_FAILURE);
    }
}

// prompt from Lecture
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
// modified from Int_list
// This is used for graph

typedef struct string_list
{
    char *value;
    struct string_list *next;
} StringList;

// print the list
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
// add one string to the end of list
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

// destroy the list
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

// get the value of string on index
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

// get the index of one string in list
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

// return the length of list
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

// initialize a graph
Graph *graph_create()
{
    Graph *graph = malloc(sizeof(Graph));
    memoryCheck(graph == NULL);
    graph->vertex_names = NULL;
    graph->edge_array = NULL;
    return graph;
}

// add one vertex to graph
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
    // transfer values in old array to new array
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

// add one edge from one vertex to another vertex
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

// print the graph
void graph_print(Graph *graph)
{
    if (graph == NULL)
    {
        return;
    }
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

// destroy the graph
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

// transform a string to int (only interger biggher or eqaul to 0)
int str2int(char *str)
{
    if (str == NULL)
    {
        return -1;
    }
    int res = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] < 48 || str[i] > 57)
        {
            return -1;
        }
        res = res * 10 + str[i] - '0';
    }
    return res;
}

// calculate the cost of trip
int cal_cost(int distance, int sta_num)
{
    float cost_float = distance * 1.2 + sta_num * 25;
    int cost_final = cost_float;
    // floor to ceil
    if (cost_float != (float)cost_final)
    {
        cost_final++;
    }
    return cost_final;
}

// process the cell
// receive value&coloum&row of one cell and do sth
int process_cell(int coloum, int row, char *value,
                 char **from_station, Graph *graph)
{
    int len = strlen(value);
    // empty string
    if (len == 0)
    {
        return 0;
    }
    // coloum 1 : read station names
    if (coloum == 1)
    {
        // create new memory for store in graph
        char *station_name_tmp = malloc(sizeof(char) * (len + 1));
        memoryCheck(station_name_tmp == NULL);
        strcpy(station_name_tmp, value);
        if (graph_add_vertex(graph, station_name_tmp) == 0)
        {
            printf("Station Duplicate.\n");
            exit(EXIT_OTHER_FAILURE);
        }
    }
    // coloum 2+ : load the first cell as from_station and following cells are edges
    else if (coloum >= 2 && row == 1)
    {
        // create new memory for from_station
        char *from_station_tmp = malloc(sizeof(char) * (len + 1));
        memoryCheck(from_station_tmp == NULL);
        strcpy(from_station_tmp, value);
        *from_station = from_station_tmp;
        if (list_index(graph->vertex_names, value) == -1)           // there are stations which not shown in coloum 1
        {
            // create new memory for store in graph
            char *add_new_station = malloc(sizeof(char) * (len + 1));
            memoryCheck(add_new_station == NULL);
            strcpy(add_new_station, value);
            graph_add_vertex(graph, add_new_station);
        }
    }
    else if (coloum >= 2 && row != 1)
    {
        char *from_station_tmp = *from_station;
        char *to_station = list_get(graph->vertex_names, row - 2);
        int distance = -1;
        if ((distance = str2int(value)) == -1)
        {
            printf("Invalid distances file.\n");
            exit(EXIT_FILECOTENT_FAILURE);
        }
        if (graph_add_edge(graph, from_station_tmp, to_station, distance) == -1)
        {
            printf("Invalid distances file.\n");
            exit(EXIT_FILECOTENT_FAILURE);
        }
    }
    return 1;
}

// read file and get cells
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
        // when feof(fp), the last char is read twice, change it to '\n'
        if (feof(fp))
        {
            ch = '\n';
        }
        if (ch == '\n' || ch == ',')
        {
            tmp[pos] = '\0';
            // printf("%-12s %p\t%d\t%d\t%d\n", tmp, tmp, coloum, row, pos);
            // process the cell
            process_cell(coloum, row, tmp, &from_station, graph);
            free(tmp);
            size = 16;
            pos = 0;
            tmp = malloc(size * sizeof(char));
            memoryCheck(tmp == NULL);
            // while char reach to '\n' and row is not still one, change line
            if (ch == '\n' && row != 1)
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
            // alloc more memory
            if (pos >= size)
            {
                size = size * 2;
                char *realloc_tmp = realloc(tmp, sizeof(char) * size);
                memoryCheck(realloc_tmp == NULL);
                tmp = realloc_tmp;
            }
        }
    }
    free(tmp);
    free(from_station);
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
    const int len = list_length(graph->vertex_names);
    int d[len], path[len], visit[len];
    
    for (int i = 0; i < len; i++)
    {
        d[i] = graph->edge_array[len * from_index + i];         // distance form from_index to other stations
        path[i] = from_index;                                   // store the path (previous station)
        visit[i] = 0;                                           // whether this station is visited
    }
    
    visit[from_index] = 1;
    d[from_index] = 0;
    for (int i = 1; i < len; i++)
    {
        int min_num = -1;
        int min = INF;
        // get the shortest station to from_station (not visited before)
        for (int j = 0; j < len; j++)
        {
            if (visit[j] == 0 && d[j] <= min)
            {
                min = d[j];
                min_num = j;
            }
        }
        visit[min_num] = 1;
        // check if there is a shorter way
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
    
    if (d[to_index] >= INF)
    {
        printf("No possible journey.\n");
        return 1;
    }
    else if (from_index == to_index)
    {
        printf("No journey, same start and end station.\n");
        return 1;
    }
    else if (path[to_index] == from_index)
    {
        printf("From %s\ndirect\nTo %s\n", from_station, to_station);
        printf("Distance %d km\n", d[to_index]);
        printf("Cost %d RMB\n", cal_cost(d[to_index], 0));
    }
    else
    {
        printf("From %s\nvia\n", from_station);
        int i = path[to_index];
        int arr[len];
        int j = 0;
        while (i != from_index)
        {
            arr[j] = i;
            j++;
            i = path[i];
        }
        for (int i = j - 1; i > -1; i--)
        {
            printf("%s\n", list_get(graph->vertex_names, arr[i]));
        }
        printf("To %s\n", to_station);
        printf("Distance %d km\n", d[to_index]);
        printf("Cost %d RMB\n", cal_cost(d[to_index], j));
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
    for (;;)
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