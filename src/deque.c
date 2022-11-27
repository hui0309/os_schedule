#include <errno.h>
#include <stdio.h>
#include<stdbool.h>
#include "coroutine_int.h"
void deq_init(struct deq *dq)
{
    dq->out = 0;
    dq->in = 0;
    dq->mask = RINGBUFFER_SIZE - 1;
}

static inline unsigned int __ringbuffer_unused(struct deq *dq)
{
    return dq->mask + 1 - (dq->in - dq->out);
}

// enqueue at in
int deq_enqueue(struct deq *dq, struct task_struct *task)
{
    if (!__ringbuffer_unused(dq))
        return -EAGAIN;

    dq->arr[dq->in & dq->mask] = task;
    dq->in++;

    return 0;
}

// dequeue at out
struct task_struct *deq_dequeue(struct deq *dq)
{
    struct task_struct *rev;
    if (dq->in == dq->out)
        return NULL;
    bool h = (dq -> in - dq -> out) & 1;
    printf("choose the %s\n",h? "head" : "tail");
    if(h){
        //printf("%d\n", (dq->out & dq->mask));
        rev = dq->arr[dq->out & dq->mask];
        dq->out++;
    }
    else{
        //printf("%d\n",((dq->in - 1) & dq->mask));
        rev = dq->arr[(dq->in - 1) & dq->mask];
        dq->in--;
    }
    return rev;
}
