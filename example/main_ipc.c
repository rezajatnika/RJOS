#include "../src/ipc.h"
#include "../src/system.h"
#include "../src/logger.h"

#include <stdio.h>
#include <string.h>


#define BUF_SIZE 256

int main() {
    system_init();
    logger_init("log.txt", LOG_LEVEL_DEBUG);

    ipc_pipe_t pipe;
    const char *path = "/tmp/rjos_fifo";

    /* Initialize the pipe as a writer (mode != 0 for writing mode). */
    if (ipc_pipe_init(&pipe, path, 0666) != 0) {
        fprintf(stderr, "Failed to initialize pipe.\n");
        return 1;
    }

    /* Send a message. */
    char send_buf[BUF_SIZE] = "Hello from sender!\n";
    if (ipc_pipe_send(&pipe, send_buf, strlen(send_buf) + 1) != 0) {
        ipc_pipe_close(&pipe);
        return 1;
    }
    printf("Message sent: %s\n", send_buf);

    if (ipc_pipe_close(&pipe) != 0) {
        fprintf(stderr, "Failed to close pipe.\n");
        return 1;
    }

    logger_destroy();
    return 0;
}