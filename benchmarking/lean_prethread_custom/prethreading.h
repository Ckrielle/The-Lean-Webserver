#include <pthread.h>
#include <stdlib.h>

struct sbuf_t
{
    int *buf;
    int n;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t slots;
    pthread_cond_t items;
};

void sbuf_init(struct sbuf_t *sb, int n);
void sbuf_delete(struct sbuf_t *sb);
void sbuf_insert(struct sbuf_t *sb, int item);
int sbuf_remove(struct sbuf_t *sb);
