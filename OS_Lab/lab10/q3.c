#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BLOCK_SIZE 4096

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s -<n> <file>\n", prog);
    exit(EXIT_FAILURE);
}

int parse_n(const char *arg) {
    if (arg[0] != '-' || arg[1] == '\0') return -1;
    char *endptr;
    int n = strtol(arg + 1, &endptr, 10);
    if (*endptr != '\0' || n <= 0) return -1;
    return n;
}

int main(int argc, char *argv[]) {
    if (argc != 3) usage(argv[0]);

    int n = parse_n(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Invalid line count.\n");
        usage(argv[0]);
    }

    const char *filename = argv[2];
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek");
        close(fd);
        return EXIT_FAILURE;
    }

    off_t offset = file_size;
    int newline_count = 0;
    char *buffer = NULL;
    size_t buffer_size = 0;

    while (offset > 0 && newline_count <= n) {
        ssize_t to_read = (offset >= BLOCK_SIZE) ? BLOCK_SIZE : offset;
        offset -= to_read;

        if (lseek(fd, offset, SEEK_SET) == -1) {
            perror("lseek");
            break;
        }

        char temp[BLOCK_SIZE];
        ssize_t bytes = read(fd, temp, to_read);
        if (bytes < 0) {
            perror("read");
            break;
        }

        for (ssize_t i = bytes - 1; i >= 0; --i) {
            if (temp[i] == '\n') {
                newline_count++;
                if (newline_count > n) {
                    // Keep data from i+1 to end
                    ssize_t keep = bytes - i - 1;
                    buffer = realloc(buffer, buffer_size + keep);
                    if (!buffer) {
                        perror("realloc");
                        close(fd);
                        return EXIT_FAILURE;
                    }
                    memmove(buffer + keep, buffer, buffer_size);
                    memcpy(buffer, temp + i + 1, keep);
                    buffer_size += keep;
                    goto print_result;
                }
            }
        }

        // Prepend entire block
        buffer = realloc(buffer, buffer_size + bytes);
        if (!buffer) {
            perror("realloc");
            close(fd);
            return EXIT_FAILURE;
        }
        memmove(buffer + bytes, buffer, buffer_size);
        memcpy(buffer, temp, bytes);
        buffer_size += bytes;
    }

print_result:
    size_t written = 0;
    while (written < buffer_size) {
        ssize_t w = write(STDOUT_FILENO, buffer + written, buffer_size - written);
        if (w < 0) {
            if (errno == EINTR) continue;
            perror("write");
            break;
        }
        written += w;
    }

    free(buffer);
    close(fd);
    return 0;
}
