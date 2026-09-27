#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define STACK_DUMP(stk) StackDump(&stk, __FILE__, __LINE__, __func__)

enum errors
{
    OK = 0,
    NULL_PTR = 1,
    MEMORY_ERROR = 2,
    STACK_UNDERFLOW = 3,
};

struct stack
{
    size_t size;
    size_t capacity;
    double *data;
};

int InitStack(stack *stk, size_t capacity);

int StackPush(stack *stk, double value);

double StackPop(stack *stk, int* err);

int StackDestroy(stack *stk);

int StackResize(stack *stk);

int PrintError(int err);

int ErrorCheck(stack *stk);

int StackDump(stack *stk, const char *file, int line, const char *func);


int main()
{
    int err = OK;

    stack stk = {};
    InitStack(&stk, 1);

    StackPush(&stk, 10);
    StackPush(&stk, 20);
    StackPush(&stk, 30);

    printf("%lg\n", StackPop(&stk, &err));
    StackPush(&stk, 67);

    for (int i = 0; i<100; i++) StackPush(&stk, i);

    printf("%lg\n", StackPop(&stk, &err));
    if(err > 0) PrintError(err);
    printf("%lg\n", StackPop(&stk, &err));
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

    if (!capacity) return MEMORY_ERROR;

    stk->data = (double *) calloc(capacity, sizeof(double));

    if (!stk->data) return MEMORY_ERROR;

    stk->size = 0;

    return OK;
}

int StackPush(stack *stk, double value)
{
    if (!stk) return NULL_PTR;

    if(stk->size >= stk->capacity-1) StackResize(stk);
    stk->data[stk->size++] = value;

    return OK;
}

double StackPop(stack *stk, int* err)
{
    if (!stk) *err = NULL_PTR;
    if (stk->size <= 0)
    {
        *err = STACK_UNDERFLOW;
        return *err;
    }
    double pop_elem = stk->data[--stk->size];
    stk->data[stk->size] = 0;

    return pop_elem;
}

int StackDestroy(stack *stk)
{
    if (!stk) return NULL_PTR;

    free(stk->data);

    return OK;
}

int StackResize(stack *stk)
{
    if (!stk) return NULL_PTR;

    printf("Changed capacity from %d to ", stk->capacity);
    stk->data = (double *) realloc(stk->data, (stk->capacity) * 2 * sizeof(double));

    if (!stk->data) return MEMORY_ERROR;

    stk->capacity*=2;
    printf("%d\n", stk->capacity);

    return stk->capacity;
}

int PrintError(int err)
{
    printf("Error code %d ", err);
    switch (err)
    {
        case 0:
            printf("No errors\n");
            break;
        case 1:
            printf("NULL pointer error\n");
            break;
        case 2:
            printf("Memory error\n");
            break;
        case 3:
            printf("Empty stack\n");
            break;
        default:
            printf("No errors\n");
            break;
    }
}

int ErrorCheck(stack *stk)
{
    if (!stk) return NULL_PTR;
    if (!stk->data) return MEMORY_ERROR;
    if (!stk->capacity) return MEMORY_ERROR;
    if (stk->size <= 0) return STACK_UNDERFLOW;

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

    printf("  stk address : %p\n", (const void *)stk);
    printf("  size        : %zu\n", stk->size);
    printf("  capacity    : %zu\n", stk->capacity);
    printf("  data ptr    : %p\n", (const void *)stk->data);

    printf("  --- checks ---\n");
    if (!stk->data)
        printf("  [!] data == NULL\n");
    if (stk->capacity == 0)
        printf("  [!] capacity == 0\n");
    if (stk->size > stk->capacity)
        printf("  [!] size > capacity  (size=%d, capacity=%d)\n",
               stk->size, stk->capacity);
    if (stk->size == 0 && stk->capacity > 0)
        printf("  [i] stack is empty\n");

    if (!stk->data)
    {
        printf("==================================================\n\n");
        return MEMORY_ERROR;
    }

    if (stk->capacity == 0)
    {
        printf("  (capacity is 0, no buffer to print)\n");
    }
    else
    {
        printf("  --- data[0..%d] ---\n", stk->capacity - 1);
        for (size_t i = 0; i < stk->capacity; i++)
        {
            const char *is_used;
            if (i + 1 == stk->size)
                is_used = "<- top";
            else if (i < stk->size)
                is_used = "used";
            else
                is_used = "unused";

            printf("    data[%3d] = %-12lg  (%s)\n",
                   i, stk->data[i], is_used);
        }
    }

    printf("  top pointer : data + size = %p\n",
           (const void *)(stk->data + stk->size));
    printf("==================================================\n\n");
}