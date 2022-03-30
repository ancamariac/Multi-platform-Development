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

    //printf("mode : %s\n", file->mode);

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

    if ((strcmp(stream->mode, "w") == 0) ||
	(strcmp(stream->mode, "w+") == 0) ||
	(strcmp(stream->mode, "a") == 0)) {
	so_fflush(stream);
	printf("aici\n");
    }

    /* close the file and free the stream */
    r = close(stream->fd);

    if (r == -1)
        return SO_EOF;

    free(stream);

    return 0;
}

int so_fflush(SO_FILE *stream)
{
    int n = 0;

    /* empty the buffer and write to file */
    if (stream->buffer_pos)
        n = write(stream->fd, stream->buffer, stream->buffer_pos);

    if (n == -1)
        return SO_EOF;

    stream->buffer_pos = 0;
    stream->size += n;
    stream->cursor += n; 

    return 0;
}

int so_fgetc(SO_FILE *stream)
{
    ssize_t n = 0;
    int chunk = (int)(stream->cursor / BUFFER_SIZE);
    int pos = (int)(stream->cursor % BUFFER_SIZE);
    
    /* read a character from the stream and returns it */

    if (stream->cursor >= stream->size) {
        printf("a intrat in eof 1\n");
	    printf("cursor : %ld\n", stream->cursor);
	    stream->err_ind = SO_EOF;
        return SO_EOF;
    }

    /* daca nu sunt in chunkul in care se face citirea */
    if (!(chunk == stream->chunk_number)) {
	    printf("cursor : %ld\n", stream->cursor);
        /* se pozitioneaza cursorul la caracterul care trebuie citit */
        stream->chunk_number = stream->cursor / BUFFER_SIZE;
        lseek(stream->fd, BUFFER_SIZE * stream->chunk_number, SEEK_SET);
        /* citesc in buffer informatie cat pentru un chunk intreg de 4096 caractere */
        n = read(stream->fd, stream->buffer, BUFFER_SIZE);

        stream->buffer_pos = n;

        /* se verifica daca s-a reusit citirea */
        if (n < 0) {
		printf("a intrat in eof 2\n");
            stream->err_ind = SO_EOF;
            return SO_EOF;
        }   
    }

    stream->cursor += 1; 

    return (int)(stream->buffer[pos]);
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
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

    return 0;
}

long so_ftell(SO_FILE *stream)
{
    return stream->cursor;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    size_t cnt = 0;
    long var = 0;

    while (cnt < size * nmemb) {
        var = so_fgetc(stream);

        if (var == SO_EOF)
            break;
        else
            *(unsigned char *)(ptr + cnt) = (unsigned char)var;

        cnt++;
    }

    return cnt / size;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    return 0;
}

int so_fputc(int c, SO_FILE *stream)
{
    /* put a character into the stream->buffer */
    unsigned char converted_c = (unsigned char)c;
    
    if (stream->buffer_pos == BUFFER_SIZE)
        so_fflush(stream);

    stream->buffer[stream->buffer_pos] = converted_c;

    if (!stream->buffer[stream->buffer_pos]) {
        stream->err_ind = SO_EOF;
        return SO_EOF;
    }

    stream->buffer_pos++;
    stream->cursor++;

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
