#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "lean.h"
#include "prethreading.h"

#define SHAREDBUFSIZE  4
#define NTHREADS  16

static struct sbuf_t shared_buf;

int open_serverfd(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)))
    {
        perror("bind error:");
        exit(2);
    }

    if (listen(fd, 1))
    {
        perror("listen error:");
        exit(2);
    }

    return fd;
}

void *thread_handler(void *ptr)
{
    while (1)
    {
        int client_fd = sbuf_remove(&shared_buf);
        handle_request(client_fd);
        close(client_fd);
    }
}

int main(int argc, char **argv)
{
    int server_fd;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        fprintf(stderr, "using default port 1337\n");
        server_fd = open_serverfd(1337);
    }
    else server_fd = open_serverfd(atoi(argv[1]));

    sbuf_init(&shared_buf, SHAREDBUFSIZE);
    pthread_t threads[NTHREADS];
    for (int i = 0;  i < NTHREADS; i++)
        pthread_create(&threads[i], NULL, thread_handler, NULL);

    while (1)
    {
        struct sockaddr_storage caddr;
        socklen_t caddr_len = sizeof(caddr);
        int client_fd = accept(server_fd, (struct sockaddr *) &caddr, &caddr_len);
        sbuf_insert(&shared_buf, client_fd);
    }
}