#ifndef wrappers_h
#define wrappers_h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>


#define MALLOC_ERROR printf("MALLOC ERROR!\n");
#define DUPLICATE_ERROR printf("NO DUPLICATES ALLOWED!\n");
#define DELETE_ERROR printf("DELETE NODE NOT FOUND!\n");
#define NOT_FOUND_ERROR printf("TARGET NOT FOUND!\n");
#define FOPEN_ERROR printf("ERROR OPENING FILE!\n");
#define INDEX_ERROR printf("INDEX OUT OF RANGE!\n");


/*  GENERAL ASSUMED BEHAVIOR OF COMPARE FUNCTION FOR ADTs:
    int compare(void *arg1, void *arg2)
    {
        //CAST to application-specific data type
        if a > b
            ret +1;
        if a < b;
            ret -1;
        else
            ret 0;
    }
*/

/****** HEAP MEMORY ALLOCATION FUNCTONS *****/
void *Malloc(size_t size)
{
    void *ptr;
    if((ptr= (void*)malloc(size)) == NULL)
    {
        MALLOC_ERROR;
        exit(101);
    }
    return ptr;
}

void *Realloc(void *ptr, size_t numMembers)
{
    void *newptr;
    
    if ((newptr = (void *) realloc(ptr, numMembers)) == NULL)
    {
        printf("Realloc error");
        exit(1);
    }
    return newptr;
}

void *Calloc(size_t numMembers, size_t size)
{
    void *ptr;
    
    if ((ptr = (void *) calloc(numMembers, size)) == NULL)
    {
        printf("Calloc error");
        exit(1);
    }
    return ptr;
}

/****** FILE DESCRIPTOR FUNCTONS *****/
FILE *Fopen(char *file, char *mode)
{
    FILE *fp;
    
    if ((fp = fopen(file, mode)) == NULL)
    {
        FOPEN_ERROR;
        exit(102);
    }
    return fp;
}

void Fclose(FILE *fp)
{
    if (fclose(fp) != 0)
    {
        printf("Fclose failed!\n");
        exit(1);
    }
}


#endif /* libs_h */
