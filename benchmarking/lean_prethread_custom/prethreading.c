#include "prethreading.h"

void sbuf_init(struct sbuf_t *sb, int n)
{
    sb->buf = (int *) malloc(n * sizeof(int));
    sb->n = n;
    sb->front = sb->rear = 0;
    pthread_mutex_init(&sb->mutex, NULL);
    pthread_cond_init(&sb->slots, NULL);
    pthread_cond_init(&sb->items, NULL);
}

void sbuf_delete(struct sbuf_t *sb)
{
    free(sb->buf);
    sb->buf = NULL;
    pthread_mutex_destroy(&sb->mutex);
    pthread_cond_destroy(&sb->slots);
    pthread_cond_destroy(&sb->items);
}

void sbuf_insert(struct sbuf_t *sb, int item)
{
    pthread_mutex_lock(&sb->mutex);
    while ((sb->rear - sb->front) == sb->n)
        pthread_cond_wait(&sb->slots, &sb->mutex);
    sb->buf[sb->rear % sb->n] = item;
    sb->rear++;
    pthread_cond_signal(&sb->items);
    pthread_mutex_unlock(&sb->mutex);
}

int sbuf_remove(struct sbuf_t *sb)
{
    pthread_mutex_lock(&sb->mutex);
    while (sb->front == sb->rear)
        pthread_cond_wait(&sb->items, &sb->mutex);
    int item = sb->buf[sb->front % sb->n];
    sb->front++;
    pthread_cond_signal(&sb->slots);
    pthread_mutex_unlock(&sb->mutex);
    return item;
}