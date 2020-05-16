#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "st.h"
#include "tree.h"
#include "libstr.h"


bool First_create = 1;
stack_t st_bracket;//
bool Flag_bracket;
data_t x_node = 0;
FILE* file = NULL;
int flag_div = 0;


void destruct_stack() {
	stack_destruct(&st_bracket);
}

void init_stack() {
	stack_init(&st_bracket);
}


bool is_leaf(Node* n) {
	if (n->left == NULL && n->right == NULL)
		return 1;
	else
		return 0;
}

void print_text_in_file(const char* s) {
	fprintf(file, " %s", s);
}


void init_file(const char* file_name, const char* mode) {
	if ((file = fopen(file_name, mode)) == NULL) {
		printf("cannot open file.\n");
		exit(1);
	}
}

void close_file() {
	fclose(file);
}


void visit_p(Node* n, bool(*func)(Node* n)) {
	if (n) {
		if (n->data == VDIV)
			flag_div += 1;
		if (flag_div) {
		flag_div--;
		fprintf(file, " {");
		visit_p(n->left, func);
		fprintf(file, "}");
		//flag_div++;
		assert((*func)(n));
		fprintf(file, " {");
		visit_p(n->right, func);
		fprintf(file, "}");
		} else {
			fprintf(file, " (");
			visit_p(n->left, func);
			assert((*func)(n));
			visit_p(n->right, func);
			fprintf(file, ")");
		}
	}
	return;
}

void delete_node(Node* n)
{
	if (n != NULL) {
		delete_node(n->right);
		delete_node(n->left);
		n->left = NULL;
		n->right = NULL;
		free(n);
	}
}

Node* create_node(data_t data, type_t type, Node* left, Node* right) {
	Node* n = (Node*) calloc(1, sizeof(Node));
	n->data = data;
	n->type = type;
	n->left = left;
	n->right = right;
	assert(n);
	return n;
}

bool Print_node(Node* n) {
	assert(n);
		switch (n->type){
			case TNUM:
				fprintf(file, " %d", n->data);
				break;
			case TADD:
				fprintf(file, " %c", (n->data == VADD)? '+' : '-');
				break;
			case TMUL:
				if (n->data == VMUL)
					fprintf(file, " \\cdot");
				else
					fprintf(file, " \\over");
				break;
			case TX:
				fprintf(file, " x");
				break;
			case TUNAR:
				if (n->data == VCOS)
					fprintf(file, " cos");
				else if (n->data == VSIN)
					fprintf(file, " sin");
				else if (n->data == VEXP)
					fprintf(file, " exp");
				else
					fprintf(file, " qrt");
			default:
				break;
		}
	stack_dump(&st_bracket);

	return 1;
}

void print_bracket() {
	elem_t ValBranch = pop(&st_bracket);
	if (ValBranch == NBranch0) {
		fprintf(file, ")");
		if (st_bracket.size) {
			push(&st_bracket,pop(&st_bracket) - 1 );
			print_bracket();
		} //else 
			//fprintf(file, "\n");
	} else if (ValBranch == QBracket || ValBranch == QBracket - 1) {
		fprintf(file, " []");
		if (st_bracket.size) {
			push(&st_bracket,pop(&st_bracket) - 1 );
			print_bracket();
		} //else 
			//fprintf(file, "\n");
	} else
		push(&st_bracket, ValBranch);
}
/*
void get_string(char* answer, int count) {
	char* swap = NULL;
	if (fgets(answer, count, stdin) == NULL){
		printf("WTF fgets of string == NULL\n");
	}
	else {
	if((swap = strchr(answer, '\n')) == NULL)
		printf("not found <\\n> in get_string\n");
	else
		*swap = '\0';
	}
}
Node* add_node(Node* n, data_t data, int side) {
	if (n == NULL || side == SELF) {
		n = create_node(n, data);
	} else	if ((n->right == NULL && side == RIGHT) || (n->right == NULL && side == LIBERTY))
			n->right = create_node(n->right, data);
		else if ((n->left == NULL && side == LEFT) || (n->left == NULL && side == LIBERTY))
			n->left = create_node(n->left, data);
		else
			printf("\nerror in chois of side in add_node\n");
	return n;
}






void create_tree(Node** n, file_tree* buf) {
	bool shift_side = 0;
	int flag_was = 0;
	int c = 0;
	if (First_create){
		buf->count = 2;
		First_create = 0;
	}
	data_t data = (data_t) calloc(SIZE_DATA, sizeof(char));
	my_scanf(buf, data);//узнать слово //узнать кол-во символов в слове
	*n = add_node(NULL, data, SELF);

	while (buf->count < buf->size) {
		printf("while: buf->count = %d", buf->count);
		c = *(buf->str + buf->count++);
		printf("<> c = %c ", c);
		if (c == ' ') {
			printf("<>c = _");
			c = *(buf->str + buf->count++);
			if (c == '(') {
				printf("int( ");
				create_with_br_1(n, buf, &shift_side, &flag_was);
			} else if (c == '[') {
				create_with_br_2(n, buf, &shift_side, &flag_was);
			}
			flag_was++;
		} else if (c == ')') {
			printf("<>c = )");
			flag_was--;
			return;
		}
	}
	//printf("\nerror in create tree, returns didn't work\n");
}


void create_with_br_1(Node** n, file_tree* buf, bool* shift_side, int* flag_was) {
	if (*shift_side) {
		*shift_side = 0;
		create_tree(&((*n)->right), buf);
		*flag_was++;
	} else if (*flag_was) {
			create_tree(&((*n)->right), buf);
			*flag_was--;
	} else {
		create_tree(&((*n)->left), buf);
		*flag_was++;
	}
}

void create_with_br_2(Node** n, file_tree* buf, bool* shift_side, int* flag_was) {
	int c = 0;
	if ((*n)->left  == NULL && *(buf->str + buf->count + 1) != ')') {
		buf->count++;
		*shift_side = 1;
	} else {
		c = *(buf->str + buf->count + 1);
		if(c != ')')
			printf("\tWTF after []\n");
		buf->count += 2;
		return;
	}
}




int my_scanf(file_tree* buf, data_t data) {
	char c;
	int first_value = buf->count;
	int counter = 0;
	while(counter < SIZE_DATA) {
		sscanf((*buf).str + buf->count, "%c", &c);
		if (c != ' ' && c != ')') {
			printf("c = %c\t", c);
			data[counter] = c;
			counter++;
			buf->count++;
		} else {
			printf("c = _ or )\n");
			break;
		}
	}
	return buf->count - first_value;
}


Node* create_func(Node* n) {
	file_tree buf = {NULL, 0, 0};
	buf.size = seek_size(file);
 	buf.str = (char*) calloc(buf.size + 1, sizeof(char));
	cpy_FinToFile(buf.str, file);
	//close_file();
	puts(buf.str);
	create_tree(&n, &buf);
	free(buf.str);
	return n;
}
*/