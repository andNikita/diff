
#ifndef ST_H//если уже задейфайнено
#define ST_H

#define INIT_SIZE 10
#define CANARY 666666
#define POISON 0  // s
#define FACTOR 2
#define OK 0
#define NOERR 1
//#define ERR 0
#define ISERR 0
#define ST_MORE 1
#define ST_LESS 0
#define SIZE_INSTRUCT 10
#define SIZE_NUMB 8
#define COUNT_R 4
//#define YESERR -666//1
#define FOERR -1



typedef long long elem_t;


struct stack_t
{
	long long canary1;
	char* name = "stack_akinator";
	elem_t *data;
	int size;
	long long maxsize;
	int errcode;
	int* reg;
	long long hash;
	long long canary2;
};

typedef struct stack_t stack_t;
#endif


void stack_init(stack_t* st);
void stack_destruct(stack_t* st);
void stack_change(stack_t *st, int flag);
void push(stack_t* stk, elem_t elem);
elem_t pop(stack_t* st);
int stack_ok(stack_t *st, char* fname);
void del_file();
int stack_dump(stack_t *st);
float charToAlfa(char c);
int findNumb(char* inst);

