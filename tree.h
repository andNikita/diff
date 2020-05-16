/*
int main(int atgc, char* argv[])
{
	Node* n = (Node*) calloc(1, sizeof(Node));
	init_file("name", "r");
	init_stack();
	delete_node(n);
	destruct_stack();
	close_file();
	return 0;
}
*/

#ifndef	_TREE_H_
#define _TREE_H_

#define NBranch2 2
#define NBranch1 1
#define NBranch0 0
#define QBracket 5
#define LEFT 11
#define RIGHT 22
#define LIBERTY 33
#define SELF 104
#define SIZE_DATA 20
#define SIZE_COMMAND 20

#define TNUM 1001
#define TADD 1003
#define VADD 901
#define VSUB 903
#define TMUL 999
#define VMUL 805
#define VDIV 807
#define TX 1101
#define VSIN 701
#define VCOS 703
#define TUNAR 700
#define VEXP 752
#define VQRT 754S



typedef int data_t;
typedef int type_t;

struct Node {
	data_t data;
	type_t type;
	struct Node* left;
	struct Node* right;
};
typedef struct Node Node;


struct file_tree {
	char* str;
	int count;
	int size;
};

typedef struct file_tree file_tree;

#endif

int my_scanf(file_tree* buf, data_t data);

void print_text_in_file(const char* s);
void create_with_br_1(Node** n, file_tree* buf, bool* shift_side, int* flag_was);
void create_with_br_2(Node** n, file_tree* buf, bool* shift_side, int* flag_was);
void get_string(char* str, int count);
Node* add_change(Node* n);
//int determ_count_numbers_data(data_t data);
void create_tree(Node** n, file_tree* buf);
void delete_node(Node* n);
Node* add_node(Node* n, data_t data, int side);
void visit_p(Node* n, bool(*func)(Node* n));
bool Print_node(Node* n);
void print_bracket(/*stack_t* st_bracket*/);
bool  Find_node(Node* n);
int def_side(data_t data);
void init_file(const char* file_name, const char* mode);
void close_file();
void init_stack();
Node* create_node(data_t data, type_t type, Node* left, Node* right);
void test(Node* n);
Node* create_func(Node* n);

void destruct_stack();

bool is_leaf(Node* n);





