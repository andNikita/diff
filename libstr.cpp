#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "libstr.h"
#define ERR 666



void printstrbuf(strbuf_t* strbuf, int n_lines, FILE* f_out)
{
    int i = 0;
    //int fd = fileno(f_out);  
    //printf("n_line = %d", n_lines);  
    for (i = 0; i < n_lines ; i++)
    {
        assert(strbuf[i].s);
        assert(strbuf[i].len);
        //printf("strbuf[i].s = %p\n", strbuf[i].s);
        fwrite(strbuf[i].s, sizeof(char), strbuf[i].len, f_out);
    }
};

long seek_size(FILE* f_in)
{
    fseek(f_in, 0, SEEK_END);
    long size_byte = ftell(f_in);
    fseek(f_in, 0, SEEK_SET);

    return size_byte;
};

void fill_strbuf(strbuf_t** strbuf, char *file, int sizebyte)
{
    //printf("noerr1");
    int n = sizebyte;
    int i = 0;
    (*strbuf)->s = file;
    char* adr_n = file;
    char* adr_last = file;
    //printf("noerr\nval");
    adr_n = (char*) memchr(file, '\n', n);
    //printf("adr_n aftr first memch = %p letter = %c\n", adr_n, *(adr_n));
    (*strbuf)->len = (int) ((long long)adr_n - (long long)adr_last + 1);
    //printf("file = %p strbuf->len = %d\n",file, (*strbuf)->len);
    adr_last = adr_n;
    for (i = 1; *(adr_n + 1) != '\0'; i++)
    {
        assert(adr_n);
       // printf(" i = %d\n", i);    
        (*strbuf + i)->s = adr_n + 1;
        //printf("poiner%d = %p\n", i + 1, (*strbuf + i)->s);
        adr_n = (char*) memchr(adr_n + 1, '\n', n - (int)((long long)adr_n + 1 - (long long)file));
        (*strbuf + i)->len = (int) ((long long)adr_n - (long long)adr_last); // длина на 1 больше чтобы захватить \n
        //printf("strbuf->len = %d\n", (*strbuf + i)->len);
        adr_last = adr_n;
    }    
    //printf("after for: i = %d adr_last = %d[%p]\n", i,  *(adr_n + 1), adr_n + 1);
    //printf("strbuf start = %p\n", (*strbuf)->s);
    //printf("strbuf last = %p\n", (*strbuf)->s);
    /*for(int i = 0; i < 10; i++)
    {
        printf("poiner%d = %p\n", i + 1, (*strbuf + i)->s);
    }*/

};

int cpy_FinToFile(char* buf, FILE* f_in)//buf уже должен быть с выделенной памятью
{
	long size_byte = seek_size(f_in);
    unsigned int fd = fileno(f_in);
    assert(fd > 0);
    fseek(f_in, 0, SEEK_SET);
    //int n_read = read(fd, (void*) buf, (unsigned) size_byte);
    long int n_read = fread((void *)buf, sizeof(char), size_byte, f_in);
    if (n_read != size_byte)
    {
        if(feof(f_in)) 
            printf("Premature end of file.\n");
        else
            printf("File read error.\n");

        printf("fd = %u\n", fd);
        printf("size_byte = %ld\n", size_byte);
        printf("n_read = %ld\n", n_read);
        printf("error in func read: n_read != size_byte\n");
        return ERR;
    }
    return 0;
};

int n_count(char* file, int size)
{
    int i = 0;
    int k = 0;
    while(i < size)
    {
        if (file[i++] == '\n')
            k++;
    }
    return k;
};

/*void print_pointers(void* poiner1,int step,int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("poiner%d = %lld\n", i + 1, (long long)poiner1 + 1);
    }
}*/