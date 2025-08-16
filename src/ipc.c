#include "ipc.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int ipc_pipe_init(ipc_pipe_t *pipe, const char *path, int mode) {
    if (!pipe || !path) {
        perror("ipc_pipe_init: invalid arguments");
        return -1;
    }
    strncpy(pipe->path, path, sizeof(pipe->path));
    pipe->path[sizeof(pipe->path) - 1] = '\0';
    pipe->mode = mode;

    /* Create the FIFO if it does not exists. */
    if (access(path, F_OK) < 0) {
        if (mkfifo(path, mode & 0777) < 0) {
            perror("ipc_pipe_init: mkfifo");
            return -1;
        }
    }
    if (mode == 0) {
        pipe->fd = open(path, O_RDONLY | O_NONBLOCK);
    } else {
        pipe->fd = open(path, O_WRONLY);
    }
    if (pipe->fd < 0) {
        perror("ipc_pipe_init: open");
        return -1;
    }
    return 0;
}

int ipc_pipe_send(ipc_pipe_t *pipe, const void *buf, size_t len) {
    if (!pipe || !buf || pipe->mode < 0) {
        perror("ipc_pipe_send: invalid arguments");
        return -1;
    }
    ssize_t n = write(pipe->fd, buf, len);
    if (n != (ssize_t)len) {
        perror("ipc_pipe_send: write");
        return -1;
    }
    return 0;
}

int ipc_pipe_recv(ipc_pipe_t *pipe, void *buf, size_t len) {
    if (!pipe || !buf || pipe->mode < 0) {
        perror("ipc_pipe_recv: invalid arguments");
        return -1;
    }
    ssize_t n = read(pipe->fd, buf, len);
    if (n != (ssize_t)len) {
        perror("ipc_pipe_recv: read");
    }
    return (int)n;
}

int ipc_pipe_close(ipc_pipe_t *pipe) {
    if (!pipe) {
        perror("ipc_pipe_close: invalid arguments");
        return -1;
    }
    close(pipe->fd);
    unlink(pipe->path);
    return 0;
}
