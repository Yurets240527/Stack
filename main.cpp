#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef double StackElem;
#define STACK_ELEM_FMT "%lg"

#define STACK_DUMP(stk) StackDump(&stk, __FILE__, __LINE__, __func__)

#define MAX_CAPACITY 1000000

#define  LEFT_PETUSHARA 3802
#define RIGHT_PETUSHARA 3565

enum errors
{
    OK = 0,
    NULL_PTR = 1,
    MEMORY_ERROR = 2,
    STACK_UNDERFLOW = 3,
    KILLED_PETUSHARA = 4,
};

struct stack
{
    size_t size;
    size_t capacity;
    StackElem *data;

    StackElem *left_petushara;
    StackElem *right_petushara;
};

int InitStack(stack *stk, size_t capacity);

int StackPush(stack *stk, StackElem value);

StackElem StackPop(stack *stk, int* err);

int StackDestroy(stack *stk);

int StackResize(stack *stk);

int PrintError(int err);

int ErrorCheck(stack *stk);

int StackDump(stack *stk, const char *file, int line, const char *func);

int main()
{
    int err = OK;

    stack stk = {};
    InitStack(&stk, 10);

    StackPush(&stk, 10);

    StackPush(&stk, 20);

    StackPush(&stk, 30);

    // printf(STACK_ELEM_FMT "\n", StackPop(&stk, &err));
    StackPush(&stk, 67);

    for (int i = 0; i<100; i++) StackPush(&stk, i);

    printf(STACK_ELEM_FMT "\n", StackPop(&stk, &err));
    if(err > 0) PrintError(err);
    printf(STACK_ELEM_FMT "\n", StackPop(&stk, &err));
    if(err > 0) PrintError(err);

    err = ErrorCheck(&stk);
    if(err > 0) PrintError(err);

    STACK_DUMP(stk);

    StackDestroy(&stk);
}


int InitStack(stack *stk, size_t capacity)
{
    if (!stk) return NULL_PTR;

    stk->capacity = capacity;

    if (capacity == 0 || capacity > MAX_CAPACITY) return MEMORY_ERROR;

    stk->data = (StackElem *) calloc(capacity+2, sizeof(StackElem));

    if (!stk->data) return MEMORY_ERROR;

    stk->data[0]          =  LEFT_PETUSHARA;
    stk->data[capacity+1] = RIGHT_PETUSHARA;

    stk->data++;
    stk->size = 0;

    return OK;
}

int StackPush(stack *stk, StackElem value)
{
    if (!stk) return NULL_PTR;

    int err = 0;
    if(stk->size >= stk->capacity) err = StackResize(stk);

    if (err > 0) return err;

    stk->data[stk->size++] = value;

    return OK;
}

StackElem StackPop(stack *stk, int* err)
{
    if (!stk) *err = NULL_PTR;
    if (*err == 0 && stk->size <= 0)
    {
        *err = STACK_UNDERFLOW;
        return *err;
    }
    StackElem pop_elem = stk->data[--stk->size];
    stk->data[stk->size] = 0;

    return pop_elem;
}

int StackDestroy(stack *stk)
{
    if (!stk) return NULL_PTR;

    free(stk->data - 1);
    stk->data = NULL;
    stk->size = 0;
    stk->capacity = 0;

    return OK;
}

int StackResize(stack *stk)
{
    if (!stk) return NULL_PTR;
    if (stk->data[-1] != LEFT_PETUSHARA || stk->data[stk->capacity] != RIGHT_PETUSHARA) return KILLED_PETUSHARA;

    printf("Changed capacity from %d to ", stk->capacity);
    stk->data = (StackElem *) realloc(stk->data-1, (stk->capacity+2) * 2 * sizeof(StackElem));

    if (!stk->data) return MEMORY_ERROR;

    stk->capacity*=2;
    printf("%d\n", stk->capacity);

    stk->data[0]               =  LEFT_PETUSHARA;
    stk->data[stk->capacity+1] = RIGHT_PETUSHARA;

    stk->data++;

    return OK;
}

int PrintError(int err)
{
    printf("Error code %d ", err);
    switch (err)
    {
        case OK:
            printf("No errors\n");
            break;
        case NULL_PTR:
            printf("NULL pointer error\n");
            break;
        case MEMORY_ERROR:
            printf("Memory error\n");
            break;
        case STACK_UNDERFLOW:
            printf("Empty stack\n");
            break;
        case KILLED_PETUSHARA:
            printf("Killed cannery");
            break;
        default:
            printf("No errors\n");
            break;
    }
}

int ErrorCheck(stack *stk)
{
    if (!stk)                      return NULL_PTR;
    if (!stk->data)                return NULL_PTR;
    if (!stk->capacity)            return NULL_PTR;
    if (stk->size > stk->capacity) return MEMORY_ERROR;
    if (stk->size <= 0)            return STACK_UNDERFLOW;
    if (stk->data[-1] != LEFT_PETUSHARA || 
    stk->data[stk->capacity] != RIGHT_PETUSHARA) return KILLED_PETUSHARA;

    return OK;
}

int StackDump(stack *stk, const char *file, int line, const char *func)
{
    printf("\n=================== STACK DUMP ===================\n");
    printf("Called from: %s:%d  in  %s()\n", file, line, func);

    if (!stk)
    {
        printf("  stk pointer : NULL  -> nothing to dump\n");
        printf("==================================================\n\n");
        return NULL_PTR;
    }

    printf("  stk address : %p\n", stk);
    printf("  size        : %d\n", stk->size);
    printf("  capacity    : %d\n", stk->capacity);
    printf("  data ptr    : %p\n", stk->data);

    printf("  --- checks ---\n");
    if (!stk->data)
    {
        printf("  [!] data == NULL\n");
        return NULL_PTR;
    }

    if (stk->capacity == 0)
        printf("  [!] capacity == 0\n");

    if (stk->size > stk->capacity)
        printf("  [!] size > capacity  (size=%d, capacity=%d)\n",
               stk->size, stk->capacity);

    if (stk->size == 0 && stk->capacity > 0)
        printf("  [i] stack is empty\n");

    if (stk->data[-1] != LEFT_PETUSHARA)
        printf("  [!] LEFT PETUSHARA IS SPOILED");

    if (stk->data[stk->capacity] != RIGHT_PETUSHARA)
        printf("  [!] RIGHT PETUSHARA IS SPOILED");


    if (stk->capacity == 0)
    {
        printf("  (capacity is 0, no buffer to print)\n");
        return MEMORY_ERROR;
    }

    printf("  --- data[0..%d] ---\n\n", stk->capacity - 1);

    printf("    Left petushara: " STACK_ELEM_FMT "\n\n", stk->data[-1]);
    for (size_t i = 0; i < stk->capacity; i++)
    {
        const char *is_used = NULL;

        if (i + 1 == stk->size)
            is_used = "<- top";

        else if (i < stk->size)
            is_used = "used";

        else
            is_used = "unused";

        printf("    data[%3d] = " STACK_ELEM_FMT "  (%s)\n",
                i, stk->data[i], is_used);
    }
    printf("\n    Right petushara: " STACK_ELEM_FMT "\n\n", stk->data[stk->capacity]);
    

    printf("  top pointer : data + size = %p\n",
           (stk->data + stk->size));
    printf("==================================================\n\n");
}