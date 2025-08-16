#ifndef RJOS_IPC_H
#define RJOS_IPC_H

#define IPC_MAX_MSG_SIZE 1024
#include <stddef.h>

typedef struct {
    char path[128];
    int fd;
    int mode;
} ipc_pipe_t;

/**
 * @brief Create/open pipe for reading or writing.
 */
int ipc_pipe_init(ipc_pipe_t *pipe, const char *path, int mode);

/**
 * @brief Send a message.
 */
int ipc_pipe_send(ipc_pipe_t *pipe, const void *msg, size_t len);

int ipc_pipe_recv(ipc_pipe_t *pipe, void *msg, size_t len);
int ipc_pipe_close(ipc_pipe_t *pipe);

#endif