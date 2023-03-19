#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

int main() {
    int fd, wd;
    char buffer[BUF_LEN];
    fd = inotify_init();

    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    wd = inotify_add_watch(fd, "/path/to/directory", IN_CLOSE_WRITE);

    if (wd < 0) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int len, i = 0;
        len = read(fd, buffer, BUF_LEN);

        while (i < len) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            if (event->mask & IN_CLOSE_WRITE) {
                printf("Archivo creado: %s\n", event->name);
                /* procesar el archivo que llegó al directorio aquí */
            }

            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
