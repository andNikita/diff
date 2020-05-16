#ifndef _LIBSTR_H_
#define _LIBSTR_H_

struct strbuf
{
    char *s;
    int len;
};

typedef struct strbuf strbuf_t;

#endif

int cpy_FinToFile(char* file, FILE* f_in);
void fill_strbuf(strbuf_t **strbuf, char *file, int size_byte);
void printstrbuf(strbuf_t* strbuf, int n_lines, FILE* f_out);
int n_count(char* file, int size);
long seek_size(FILE* f_in);
//void print_pointers((void*)poiner1, int step, int count);


