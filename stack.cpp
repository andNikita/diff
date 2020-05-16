/*
Nikita Oskolkov
Stack must contain numbers

in stack_t:
errcode == 0 => 0 errors
in function write funcname to stack ok
fname = "...\n";

*/
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int New_Dump = 0;

int stack_dump(stack_t *st/*, char* fname*/)
{
	assert(st);
	FILE* fdump = fopen("st_dump", "a");
	if (fdump == NULL){
		perror("in stack_ok file doesn't open");
		return FOERR;
	} else
	{
		if (!New_Dump++)
			fprintf(fdump,(!New_Dump++)?"\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n" : "");
		fprintf(fdump, "stackDump \"%s\" [%p] (%s)\n ", st->name, st, stack_ok(st, "dump")?"ok":"no ok");
		fprintf(fdump, "\t{\n\tsize = %d\n\tdata[%lld] = %p\n", st->size, st->maxsize - 1, &st->data[st->maxsize - 1]);
		fprintf(fdump, "\t\t{\n");
		for (int i = 0; i < st->maxsize - 1; i++)
		{
			fprintf(fdump,"\t\t\t%c[%d] = %d%s\n",(i < st->size) ? '*' : ' ', i, st->data[i], (i < st->size)?"":" (poisin?)") ;
		}
		fprintf(fdump, "\t\t}\n");
		fprintf(fdump, "\t\tcanary1 = %lld [%p];\n", st->canary1, &st->canary1);
		fprintf(fdump, "\t\thash = %lld;\n", st->hash);
		fprintf(fdump, "\t\tcanary2 = %lld [%p];\n", st->canary2, &st->canary2);
		fprintf(fdump, "\t}\n\n");
		fclose(fdump);
	}
}

void del_file()
{
	printf("\nхотите ли вы удалить файл \"stack_ok\"? да - 1, нет - 0\n");
	int ok = 0;
	scanf("%d", &ok);
	if (ok)
		if (remove("stack_ok") == -1)
			perror("stack_ok didn't delete");
	printf("\nхотите ли вы удалить файл \"st_dump\"? да - 1, нет - 0\n");
	scanf("%d", &ok);
	if (ok)
		if (remove("st_dump") == -1)
			perror("st_dump didn't delete");

}

long long f_hash(stack_t* st)
{
	assert(st);
	//char fname[] = "f_hash\n";
	long long hash = st->canary1*31 + st->canary2*13;
	int i = 0;

	for (i = 0; i < st->size; i++)
	{
		hash += st->data[i]*(i + 3);
	}

	return hash;
}

int stack_ok(stack_t *st,char* fname)
{
	assert(st);
	int n_err = 0;
	FILE* file_ok = fopen("stack_ok", "a");
	if (file_ok == NULL){
		perror("in stack_ok file doesn't open");
		return FOERR;
	}
	else 
	{
		fputs(fname, file_ok);
		if (st->canary1 != st->canary2 && st->canary1 != CANARY)
		{
				fputs("canarys were changed\n", file_ok);
				n_err++;
		}
		if (f_hash(st) != st->hash)
		{
			fputs("hash was changed\n", file_ok);
			n_err++;
		}
		if (st->size > st->maxsize || st->size < 0)
		{
			fputs("problem with stack's size\n", file_ok);
			n_err++;
		}
		if (st->hash != f_hash(st))//dada == null
		{
			fputs("stack's hash != f_hash\n", file_ok);
			n_err++;
		}
		if (!n_err)
		{
			fputs("no errors\n", file_ok);
			fclose(file_ok);
			return NOERR;
		} else 
		return ISERR;
	}
	return ISERR;
}



void push(stack_t* st, elem_t elem)
{
	assert(st);
	char fname[] = "push\n";
	stack_ok(st, fname);
	if (st->size >= st->maxsize)
		stack_change(st, st->maxsize * FACTOR);
	st->data[st->size++] = elem;
	st->hash = f_hash(st);
}

elem_t pop(stack_t* st)
{
	assert(st);
	char fname[] = "pop\n";
	stack_ok(st, fname);
	elem_t elem = st->data[--st->size];
	st->hash = f_hash(st);
	if(st->size < (st->maxsize / FACTOR - 100))
		stack_change(st, st->maxsize / FACTOR);
	return elem;
}

void stack_change(stack_t *st, int size)
{
	assert(st);
	//printf("msize %lld\n", st->maxsize);
	/*if (flag)
		st->maxsize = st->maxsize * FACTOR;
	else
	{
		//printf(" size = %lld msize %lld\n", st->size, st->maxsize);
		st->maxsize = (st->maxsize / FACTOR);
	}	*/
	st->maxsize = size;
	st->data = (elem_t*) realloc(st->data, sizeof(elem_t) * st->maxsize);
	if (!st->data)
		perror("ralloc didn't give memory");
}

void stack_init(stack_t* st)
{
	assert(st);
	st->data = (elem_t*) calloc(INIT_SIZE, sizeof(elem_t));//init_size
	assert(st->data);
	st->reg = (int*) calloc(COUNT_R, sizeof(int));
	st->maxsize = INIT_SIZE;
	st->size = 0;
	st->canary1 = CANARY;
	st->canary2 = CANARY;
	st->hash = f_hash(st);
}

void stack_destruct(stack_t* st)
{
	assert(st);
	int i = 0;
	for (i = 0; i < st->size; i++)
		st->data[i] = POISON;
	st->canary1 = POISON;
	st->canary2 = POISON;
	st->size = POISON;
	st->maxsize = POISON;
	st->hash = POISON;
	free(st->data);
	free(st->reg);
}
