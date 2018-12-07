#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int INF = 9999999;
const int EXIT_FILEOPEN_FAILURE = 1;
const int EXIT_FILECOTENT_FAILURE = 2;
const int EXIT_MALLOC_FAILURE = 3;
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

int graph_init(Graph *graph, int n, StringList *vertex_names)
{
    if (graph == NULL || n <= 0 || vertex_names == NULL)
    {
        return 0;
    }
    graph->vertex_names = vertex_names;
    graph->edge_array = malloc(sizeof(int) * n * n);
    memoryCheck(graph->edge_array == NULL);
    for (int i = 0; i < n * n; i++)
    {
        graph->edge_array[i] = INF;
    }
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
int readLine(char **line, FILE *fp)
{
    int size = 1;
    char *tmp;
    tmp = malloc(size * sizeof(char));
    memoryCheck(tmp == NULL);
    char ch;
    int pos = 0;
    do
    {
        if (feof(fp))
        {
            tmp[pos - 1] = '\0';
            break;
        }
        fscanf(fp, "%c", &ch);
        if (ch != '\n')
        {
            tmp[pos] = ch;
            pos++;
            if (pos >= size)
            {
                size = size * 2;
                tmp = realloc(tmp, sizeof(char) * size);
                if (tmp == NULL)
                {
                    return -1;
                }
            }
        }
    } while (ch != '\n');
    tmp[pos] = '\0';
    *line = tmp;
    return 0;
}

int str2int(char *str)
{
    int res = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

int processCell(int coloum, int row, StringList **station_names, char *value,
                char **from_station, Graph *graph)
{
    StringList *station_names_tmp = *station_names;
    if ((coloum > list_length(station_names_tmp) + 1 ||
         row > list_length(station_names_tmp) + 1) &&
        coloum >= 2)
    {
        printf("Invalid distances file.");
        exit(EXIT_FILECOTENT_FAILURE);
    }
    int len = strlen(value);
    if (len == 0)
    {
        return 0;
    }
    if (coloum == 1)
    {
        char *station_name = malloc(sizeof(char) * (len + 1));
        memoryCheck(station_name == NULL);
        strcpy(station_name, value);
        list_append(station_names, station_name);
        // graph_add_vertex(g, station_name);
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
        char *to_station = list_get(station_names_tmp, row - 2);
        int distance = str2int(value);
        if (graph_add_edge(graph, from_station_tmp, to_station, distance) == -1)
        {
            printf("Invalid distances file.");
            exit(EXIT_FILECOTENT_FAILURE);
        };
    }
    return 1;
}

int readFile(const char *filename, Graph *graph)
{
    FILE *fp;
    StringList *station_names = NULL;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("Cannot open file.");
        exit(EXIT_FILEOPEN_FAILURE);
    }

    int coloum = 1;
    while (!feof(fp))
    {
        if (coloum == 2)
        {
            graph_init(graph, list_length(station_names), station_names);
        }
        char *line;
        readLine(&line, fp);
        // printf("\nColoum : %d\n", coloum);
        int row = 1;
        int pos = 0;
        int temp_len = 0;
        char *from_station = NULL;
        int line_len = strlen(line);
        for (int i = 0; i < line_len + 1; i++)
        {
            char temp[temp_len + 1];
            if (line[i] == ',' || i == line_len)
            {
                strncpy(temp, line + pos, temp_len);
                temp[temp_len] = '\0';
                processCell(coloum, row, &station_names, temp, &from_station, graph);
                //printf("%s %p %d %d %d\n", temp, temp, row, temp_len, pos);
                pos = i + 1;
                temp_len = 0;
                row++;
            }
            else
            {
                temp_len++;
            }
        }
        free(line);
        free(from_station);
        coloum++;
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
    readFile(argv[1], graph);
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