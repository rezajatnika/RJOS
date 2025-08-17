#ifndef RJOS_IPC_H
#define RJOS_IPC_H

#include <stddef.h>

#define IPC_MAX_MSG_SIZE 1024

/**
 * @brief Structure representing an inter-process communication (IPC) pipe.
 *
 * This structure stores data required to manage a named pipe (FIFO) used for
 * inter-process communication. It contains the pipe path, the file descriptor
 * for the open pipe, and the access mode.
 */
typedef struct ipc_pipe {
    char path[128];
    int fd;
    int mode;
} ipc_pipe_t;

/**
 * @brief Initializes an inter-process communication (IPC) pipe.
 *
 * @param pipe Pointer to the ipc_pipe_t structure to initialize.
 * @param path Path to the named pipe (FIFO) that will be created or used.
 * @param mode Mode for the pipe. Use 0 for read-only mode, or non-zero for write mode.
 *        The lower permission bits are used when creating the pipe (e.g., 0666).
 * @return 0 on success, or -1 on failure with an error message printed to stderr.
 */
int ipc_pipe_init(ipc_pipe_t *pipe, const char *path, int mode);

/**
 * @brief Sends data through an inter-process communication (IPC) pipe.
 *
 * @param pipe Pointer to the ipc_pipe_t structure representing the pipe.
 *             Must be properly initialized and opened for writing.
 * @param buf Pointer to the buffer containing the data to send.
 * @param len Length of the data to be sent, in bytes.
 * @return 0 on success, or -1 on failure with an error message printed to stderr.
 */
int ipc_pipe_send(ipc_pipe_t *pipe, const void *msg, size_t len);

/**
 * @brief Receive data from a pipe into the provided buffer.
 *
 * @param pipe A pointer to the ipc_pipe_t structure representing the pipe.
 * @param msg A pointer to the buffer where received data will be stored.
 * @param len The size of the buffer and the maximum number of bytes to read.
 * @return The number of bytes successfully read, or -1 on error.
 */
int ipc_pipe_recv(ipc_pipe_t *pipe, void *msg, size_t len);

/**
 * @brief Close the specified IPC pipe and release associated resources.
 *
 * @param pipe Pointer to the ipc_pipe_t structure representing the pipe to be closed.
 *        Must not be NULL.
 * @return 0 on success, or -1 on failure (e.g., if the pipe is invalid).
 */
int ipc_pipe_close(ipc_pipe_t *pipe);

#endif