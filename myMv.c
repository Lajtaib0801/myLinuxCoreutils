#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

//TODO: Create header(s) for all these
typedef enum operation
{
    op_move,
    op_rename,
    op_not_achievable
} operation_t;


void check_parameter(const int, const char*);
bool is_exists(const char*);
operation_t decide_operation(char*, char*);
char* get_item_parents(const char*);
char* get_item(const char*);

int main(int argc, char** argv)
{
    check_parameter(argc, argv[1]);
    printf("%d", decide_operation(argv[1], argv[2]));
    return 0;
}

void check_parameter(const int count, const char* param1)
{
    //TODO: Later add switches, at least --help
    if (3 != count)
    {
        fprintf(stderr, "Bad arguments! The correct format is: 'myMv {source} {dest}'");
        exit(EXIT_FAILURE);
    }

    if (!is_exists(param1))
    {
        fprintf(stderr, "Item does not exist: %s", param1);
        exit(EXIT_FAILURE);
    }
}


bool is_exists(const char* item)
{
    struct stat buf;
    if (0 != stat(item, &buf))
        return false;
    return true;
}

operation_t decide_operation(char* param1, char* param2)
{
    //TODO: This should be expanded (devices for example)
    struct stat p1_stat;
    stat(param1, &p1_stat);
    struct stat p2_stat;
    stat(param2, &p2_stat);
    if (S_ISDIR(p1_stat.st_mode) && S_ISREG(p2_stat.st_mode)) //first parameter is a directory and the second is a regular file -> cannot move, nor rename
    {
        return op_not_achievable;
    }

    char* parent1 = get_item_parents(param1);
    char* parent2 = get_item_parents(param2);
    if ((NULL == parent1 && NULL == parent2) || 0 == strcmp(parent1, parent2))
    {
        return op_rename;
    }

    return op_move;
}

char* get_item_parents(const char* full_path)
{
    char* path;
    int full_length = strlen(full_path) + 1;
    if (NULL == (path = malloc(sizeof(char) * full_length)))
    {
        fprintf(stderr, "Could not allocate memory for the string of the parent path of: %s", full_path);
        exit(EXIT_FAILURE);
    }
    strcpy(path, full_path);

    if ('/' == path[full_length - 2] || '\\' == path[full_length - 2])
    {
        path[full_length - 2] = '\0';
    }
    char* last_slash = NULL == strrchr(path, '/') ? strrchr(path, '\\') : strrchr(path, '/');

    if (NULL == last_slash)
    {
        free(path);
        return NULL;
    }
    *last_slash = '\0';
    return path;
}
char* get_item(const char* full_path)
{
    char* path;
    int full_length = strlen(full_path) + 1;
    if (NULL == (path = malloc(sizeof(char) * full_length)))
    {
        fprintf(stderr, "Could not allocate memory for the string of the parent path of: %s", full_path);
        exit(EXIT_FAILURE);
    }
    strcpy(path, full_path);

    if ('/' == path[full_length - 2] || '\\' == path[full_length - 2])
    {
        path[full_length - 2] = '\0';
    }
    char* last_slash = NULL == strrchr(path, '/') ? strrchr(path, '\\') : strrchr(path, '/');

    if (NULL == last_slash)
    {
        free(path);
        return NULL;
    }
    char* item;
    
    if (NULL == (item = malloc(sizeof(char) * full_length)))
    {
        fprintf(stderr, "Could not allocate memory for the string of the item name of: %s", full_path);
        exit(EXIT_FAILURE);
    }
    strncpy(item, last_slash+1, &path[full_length] - last_slash);
    free(path);
    return item;
}

