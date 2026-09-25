#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct stack
{
    size_t size;
    size_t capacity;
    double *data;
};

int InitStack(stack *stk, size_t capacity);

int StackPush(stack *stk, double value);

double StackPop(stack *stk);

int StackDestroy(stack *stk);

int main()
{
    stack stk = {};
    InitStack(&stk, 10);

    StackPush(&stk, 10);
    StackPush(&stk, 20);
    StackPush(&stk, 30);
    printf("%lg\n", StackPop(&stk));
    StackPush(&stk, 67);
    printf("%lg\n", StackPop(&stk));
    printf("%lg\n", StackPop(&stk));

    StackDestroy(&stk);
}


int InitStack(stack *stk, size_t capacity)
{
    assert(stk);
    stk->data = (double *) calloc(capacity, sizeof(double));
    stk->size = 0;
    return 0;
}

int StackPush(stack *stk, double value)
{
    assert(stk);
    stk->data[stk->size++] = value;

}

double StackPop(stack *stk)
{
    assert(stk);

    double pop_elem = stk->data[--stk->size];
    stk->data[stk->size] = 0;

    return pop_elem;
}

int StackDestroy(stack *stk)
{
    assert(stk);
    free(stk->data);
}