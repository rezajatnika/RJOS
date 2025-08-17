#include "net_util.h"
#include "../logger.h"

#include <fcntl.h>

int set_cloexec(int fd) {
    int flags = fcntl(fd, F_GETFD, 0);
    if (flags < 0) {
        logger_log(LOG_LEVEL_ERROR, "set_cloexec: fcntl failed");
        return -1;
    }
    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0) {
        logger_log(LOG_LEVEL_ERROR, "set_cloexec: fcntl failed");
        return -1;
    }
    return 0;
}

int set_blocking(int fd, int blocking) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        logger_log(LOG_LEVEL_ERROR, "set_blocking: fcntl failed");
        return -1;
    }
    if (blocking) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }
    return fcntl(fd, F_SETFL, flags);
}