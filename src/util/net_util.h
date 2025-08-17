#ifndef NET_UTIL_H
#define NET_UTIL_H

/**
* Sets the close-on-exec (FD_CLOEXEC) flag on a file descriptor.
 * This ensures that the file descriptor will be automatically closed
 * during an exec family function call.
 *
 * @param fd The file descriptor on which to set the close-on-exec flag.
 * @return Returns 0 on success, or -1 in case of failure.
 */
int set_cloexec(int fd);

/**
 * Sets the blocking mode of a file descriptor.
 * This modifies the file descriptor flags to enable or disable non-blocking mode.
 *
 * @param fd The file descriptor to configure.
 * @param blocking A flag indicating whether to enable (non-zero) or disable (zero) blocking mode.
 * @return Returns 0 on success, or -1 in case of failure.
 */
int set_blocking(int fd, int blocking);

#endif