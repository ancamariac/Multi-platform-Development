#include "so_stdio.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

struct _so_file {
int fd;
long cursor;
char buffer[BUFFER_SIZE];
int buffer_pos;
long size;
int err_ind;
int chunk_number;
char mode[3];
char last_op;
};

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
    int fd = -1;
    long cursor = 0;
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
    file->chunk_number = -1;
    strcpy(file->mode, mode);
    file->last_op = 'r';

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

    if (stream->last_op == 'w')
	    so_fflush(stream);
    /* close the file and free the stream */
    r = close(stream->fd);
    free(stream);
    if (r == SO_EOF)
        return SO_EOF;

    return 0;
}

int so_fflush(SO_FILE *stream)
{
    int n = 0;

    /* empty the buffer and write to file */
    if (stream->buffer_pos)
        n = write(stream->fd, stream->buffer, stream->buffer_pos);

    if (n == -1) {
        stream->err_ind = SO_EOF;
        return SO_EOF;
    }

    stream->buffer_pos = 0;
    stream->last_op = 'r';

    return 0;
}

int so_fgetc(SO_FILE *stream)
{
    ssize_t n = 0;
    int chunk = (int)(stream->cursor / BUFFER_SIZE);
    int pos = (int)(stream->cursor % BUFFER_SIZE);

    /* read a character from the stream and returns it */

    if (stream->last_op == 'w')
        so_fflush(stream);

    if (stream->cursor == stream->size) {
	    stream->err_ind = SO_EOF;
        return SO_EOF;
    }

    /* check if that's the chunk for reading */
    if (!(chunk == stream->chunk_number)) {
        /* place the cursor at the character to be read */
        stream->chunk_number = chunk;
        lseek(stream->fd, BUFFER_SIZE * stream->chunk_number, SEEK_SET);
        /* place in the buffer a chunk of BUFFER_SIZE characters */
        n = read(stream->fd, stream->buffer, BUFFER_SIZE);

        stream->buffer_pos = n;

        if (n == -1 || n < stream->buffer_pos) {
            stream->err_ind = SO_EOF;
            return SO_EOF;
        }
    }

    stream->last_op = 'r';
    stream->cursor += 1;

    return (int)(stream->buffer[pos]);
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
    if (stream->last_op == 'w')
        so_fflush(stream);

    /* sets the file cursor */
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

    lseek(stream->fd, stream->cursor, SEEK_SET);
    stream->chunk_number = -1;

    return 0;
}

long so_ftell(SO_FILE *stream)
{
    return stream->cursor;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    size_t cnt = 0;
    int var = 0;
    int n = 0;

    while (cnt < size * nmemb) {
        var = so_fgetc(stream);

        if (var == SO_EOF) {
            lseek(stream->fd, cnt, SEEK_SET);
            n = read(stream->fd, stream->buffer, BUFFER_SIZE);
            if (n == 0) {
                stream->err_ind = SO_EOF;
                return cnt / size;
            } else
		        break;
        } else
            *(unsigned char *)(ptr + cnt) = (unsigned char)var;

        cnt++;
    }

    return cnt / size;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    size_t cnt = 0;

    while (cnt < size * nmemb) {
        unsigned char character = *(unsigned char *)(ptr + cnt);

        so_fputc(character, stream);

        cnt++;
    }

    return cnt / size;
}

int so_fputc(int c, SO_FILE *stream)
{
    /* put a character into the stream->buffer */
    unsigned char converted_c = (unsigned char)c;

    if (stream->last_op == 'r')
	stream->buffer_pos = 0;

    if (stream->buffer_pos == BUFFER_SIZE - 1)
        so_fflush(stream);

    stream->buffer[stream->buffer_pos] = converted_c;

    stream->buffer_pos++;
    stream->cursor++;
    stream->size++;
    stream->last_op = 'w';

    return c;
}

int so_feof(SO_FILE *stream)
{
    if (stream->cursor == stream->size)
        return 1;

    return 0;
}

int so_ferror(SO_FILE *stream)
{
    return stream->err_ind;
}

SO_FILE *so_popen(const char *command, const char *type)
{
    return NULL;
}

int so_pclose(SO_FILE *stream)
{
    return 0;
}
