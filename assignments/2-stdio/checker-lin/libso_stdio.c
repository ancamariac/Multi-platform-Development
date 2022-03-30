#include "so_stdio.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

struct _so_file {
int fd;
int cursor;
char buffer[BUFFER_SIZE];
int buffer_pos;
int size;
int err_ind;
};

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
    int fd = -1;
    int cursor = 0;
    struct stat st; 

    if (strcmp(mode, "r") == 0)
        fd = open(pathname, O_RDONLY, 0666);
    else if (strcmp(mode, "r+") == 0)
        fd = open(pathname, O_RDWR, 0666);
    else if (strcmp(mode, "w") == 0)
        fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    else if (strcmp(mode, "w+") == 0)
        fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, 0666);
    else if (strcmp(mode, "a") == 0)
        fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, 0666);
    else if (strcmp(mode, "a+") == 0)
        fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, 0666);
    else
        return NULL;

    if (fd == -1)
        return NULL;

    SO_FILE *file = malloc(sizeof(SO_FILE));

    if (!file)
        exit(12);

    file->fd = fd;
    file->cursor = cursor;
    file->buffer_pos = 0;
    file->err_ind = 0;

    fstat(file->fd, &st);

    file->size = st.st_size;

    return file;
}

int so_fileno(SO_FILE *stream)
{
    return stream->fd;
}

int so_fclose(SO_FILE *stream)
{
    int r = 0;

    r = close(stream->fd);

    if (r == -1)
        return SO_EOF;

    free(stream);

    return 0;
}

int so_fflush(SO_FILE *stream)
{
    ssize_t n = 0;

    if (stream->buffer_pos)
        n = write(stream->fd, stream->buffer, stream->buffer_pos);
    
    stream->buffer_pos = 0;

    if (n == -1)
        return SO_EOF;

    return 0;
}

int so_fgetc(SO_FILE *stream)
{
    ssize_t n = 0;
    
    n = read(stream->fd, stream->buffer, stream->buffer_pos);

    return 0;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
    if (whence == SEEK_SET)
        stream->cursor = offset;
    else if (whence == SEEK_CUR)
        stream->cursor += offset;
    else if (whence == SEEK_END)
        stream->cursor = stream->size + offset;
    else
        return -1;

    if ((stream->cursor < 0) || (stream->cursor > stream->size))
        return -1;

    return 0;
}

long so_ftell(SO_FILE *stream)
{
    return stream->cursor;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    return 0;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    return 0;
}

int so_fputc(int c, SO_FILE *stream)
{
    return 0;
}

int so_feof(SO_FILE *stream)
{
    if (stream->cursor == stream->size)
        return stream->cursor;

    return 0;
}

int so_ferror(SO_FILE *stream)
{
    if (stream->err_ind)
        return stream->err_ind;
        
    return 0;
}

SO_FILE *so_popen(const char *command, const char *type)
{
    return NULL;
}

int so_pclose(SO_FILE *stream)
{
    return 0;
}
