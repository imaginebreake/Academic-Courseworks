#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Unsorted string list. */
struct string_list
{
    char *value;
    struct string_list *next;
};
typedef struct string_list StringList;

// Given a list, print the values in the list.
void list_print(StringList *start)
{
    StringList *cur = start;
    printf("[");
    if (cur != NULL)
    {
        printf("%s", cur->value);
        while (cur->next != NULL)
        {
            cur = cur->next;
            printf(",%s", cur->value);
        }
    }
    printf("]\n");
}

// Given a list, append the given value to the list.
// Return true/false on success/failure.
int list_append(StringList **start, char *val)
{
    if (*start == NULL)
    {
        StringList *n = malloc(sizeof(StringList));
        if (n == NULL)
        {
            // error, cannot allocate memory for new list node.
            return 0;
        }
        n->value = val;
        n->next = NULL;
        *start = n;
    }
    else
    {
        StringList *cur = *start;
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        StringList *n = malloc(sizeof(StringList));
        if (n == NULL)
        {
            // error, cannot allocate memory for new list node.
            return 0;
        }
        n->value = val;
        n->next = NULL;
        cur->next = n;
    }
    return 1;
}

// Given a list, prepend the given value to the list.
// Return true/false on success/failure.
int list_prepend(StringList **start, char *val)
{
    StringList *n = malloc(sizeof(StringList));
    if (n == NULL)
    {
        // error, cannot allocate memory for new list node.
        return 0;
    }
    n->value = val;
    n->next = NULL;

    if (*start == NULL)
    {
        // empty list
        *start = n;
    }
    else
    {
        // existing list
        n->next = *start;
        *start = n;
    }
    return 1;
}

// Given a list, insert a new val after the n'th position of the list
// (zero-based). If n < 0, insert at beginning.  If n >= length, insert at end.
// Return true/false on success/failure.
int list_insert(StringList **start, int n, char *val)
{
    if (*start == NULL || n <= 0)
    {
        // if empty, just reuse the prepend function to put it in list.
        // if n == 0, then we should prepend anyway.
        return list_prepend(start, val);
    }
    else
    {
        n--; // we've already dealt with the first position above, so decrement n.
        StringList *prev = *start;
        StringList *cur = (*start)->next;

        StringList *el = malloc(sizeof(StringList));
        if (el == NULL)
        {
            return 0;
        }
        el->value = val;
        el->next = NULL; // we overwrite this later, setting NULL here is for safety
                         // in case we make a mistake and forget to set it later.

        while (cur != NULL)
        {
            if (n == 0)
            {
                prev->next = el;
                el->next = cur;
                return 1;
            }
            n--;
            prev = cur;
            cur = cur->next;
        }
        prev->next = el;
        el->next = NULL;
        return 1;
    }
}

// Given a list, free all the elements of the list and set the start to NULL
// so we don't accidentally use it in the future.
void list_free(StringList **start)
{
    StringList *cur = *start;
    while (cur != NULL)
    {
        StringList *temp = cur->next;
        free(cur);
        cur = temp;
    }
    *start = NULL;
}

// Given a list, return the value of the n'th position (zero-based).
// If n is out-of-bounds, return NULL.
char *list_get(StringList *start, int n)
{
    StringList *cur = start;
    if (n < 0)
    {
        return NULL;
    }
    while (cur != NULL)
    {
        if (n == 0)
        {
            return cur->value;
        }
        else
        {
            n--;
            cur = cur->next;
        }
    }
    return NULL;
}

// Returns the index of the given value in the list, or -1 if not present.
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

// Given a list, return the length of the list.
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

int readline(char **line, FILE *fp)
{
    int size = 1;
    char *tmp;
    tmp = malloc(size * sizeof(char));
    if (tmp == NULL)
    {
        return -1;
    }
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
    for(int i = 0;i<strlen(str);i++){
            res = res*10 + str[i] - '0';
    }
    return res;
}

int processCell(int coloum, int row, StringList **station_names, char *value,
                char *from_station)
{
    StringList *station_names_tmp = *station_names;
    int len = strlen(value);
    if (len == 0)
    {
        return 0;
    }
    if (coloum == 1)
    {
        char *station_name = malloc(sizeof(char) * (len + 1));
        if (station_name == NULL)
        {
            return -1;
        }
        strcpy(station_name, value);
        list_append(station_names, station_name);
        // graph_add_vertex(g, station_name);
    }
    else if (coloum >= 2 && row != 1)
    {
        char *to_station = list_get(station_names_tmp, row - 2);
        int distance = str2int(value);
        printf("From:%s To:%s Distance:%d\n", from_station, to_station, distance);
    }
    return 1;
}

int readFile(const char *filename)
{
    FILE *fp;
    printf("%s", filename);
    StringList *station_names = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Cannot open file.");
        exit(EXIT_FAILURE);
    }
    else
    {
        int coloum = 1;
        while (!feof(fp))
        {
            char *line;
            readline(&line, fp);
            printf("\nColoum : %d\n", coloum);
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
                    if (row == 1 && coloum != 1)
                    {
                        from_station = malloc(sizeof(char) * (temp_len + 1));
                        printf("%p ", from_station);
                        strcpy(from_station, temp);
                        from_station[temp_len] = '\0';
                        printf("%s\n", from_station);
                    }
                    processCell(coloum, row, &station_names, temp, from_station);
                    // printf("%s %p %d %d %d\n", temp, temp, row, temp_len, pos);
                    pos = i + 1;
                    temp_len = 0;
                    row++;
                }
                else
                {
                    temp_len++;
                }
            }
            // list_print(station_names);
            free(line);
            free(from_station);
            coloum++;
            list_print(station_names);
        }
        fclose(fp);
    }
    return 1;
}

int main(int argc, char *argv[]) { readFile(argv[1]); }