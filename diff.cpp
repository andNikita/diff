#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "libstr.h"
#include "st.h"
#include "tree.h"
#include <time.h>

//___type
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
#define VQRT 754

//_______________DSL
#define _D(x) differentiate(x)
#define _C(x) cpy_node(x)
#define _CONST(x) create_node(x, TNUM, NULL, NULL)
#define _ADD(x, y) create_node(VADD, TADD, x, y)
#define _SUB(x, y) create_node(VSUB, TADD, x, y)
#define _MUL(x, y) create_node(VMUL, TMUL, x, y)
#define _DIV(x, y) create_node(VDIV, TMUL, x, y)
#define _COS(x) create_node(VCOS, TUNAR, NULL, x)
#define _SIN(x) create_node(VSIN, TUNAR, NULL, x)
#define _EXP(x) create_node(VEXP, TUNAR, NULL, x)
#define _QRT(x) create_node(VQRT, TUNAR, NULL, x)



//_________________|

char* s = NULL;
Node* GetG(char* str);
Node* GetN();
Node* GetE();
Node* GetUNAR();
Node* GetP();
Node* cpy_node(Node* n);
Node* dif_mul(Node* n, int VAL);
Node* differentiate(Node* n);
void print_latex();
Node* simplify(Node* n);
const char* generate_str();

int main(int argc, char* argv[]) {
	init_stack();
	init_file("oko", "w");
	char str[] = "x*sin(x)+qrt(3*x)+exp(x)/(2*x)#";
	Node* n = GetG(str);
	visit_p(n, Print_node);
	close_file();
	init_file("dif_ur", "w");
	print_latex();
	visit_p(n, Print_node);
	Node* n_dif = differentiate(n);
	print_text_in_file(generate_str());
	visit_p(n_dif, Print_node);
	n_dif = simplify(n_dif);
	print_text_in_file(generate_str());
	visit_p(n_dif, Print_node);
	print_text_in_file("\n\\end{document}\n");
	delete_node(n_dif);
	delete_node(n);
	destruct_stack();
	
	close_file();
}

Node* add_left_node(Node* n) {

	assert(n->left);
	Node* left = n->left;
	n->type = n->left->type;
	n->data = n->left->data;
	n->right = n->left->right;
	n->left = n->left->left;
	left->left = NULL;
	left->right = NULL;
	delete_node(left);
	return n;
}

Node* add_right_node(Node* n) {
	assert(n->left);
	Node* right = n->right;
	n->type = n->right->type;
	n->data = n->right->data;
	n->left = n->right->left;
	n->right = n->right->right;
	right->left = NULL;
	right->right = NULL;
	delete_node(right);
	return n;
}

Node* simplify(Node* n) {
	if (n == NULL)
		return n;
	n->left = simplify(n->left);
	n->right = simplify(n->right);
	if (n->data == VMUL && (((n->right->data == 0) && (n->right->type == TNUM)) || ((n->left->data == 0) &&  (n->left->type == TNUM))) ) {
		n->type = TNUM;
		n->data = 0;
		delete_node(n->left);
		delete_node(n->right);
		n->left = NULL;
		n->right = NULL;
	} else if (n->data == VMUL && n->right->data == 1) {
		delete_node(n->right);
		add_left_node(n);
	} else if (n->data == VMUL && n->left->data == 1) {
		delete_node(n->left);
		add_right_node(n);
	} else if (n->data == VADD && n->right->data == 0) {
		delete_node(n->right);
		add_left_node(n);
	} else if(n->data == VADD && n->left->data == 0) {
		delete_node(n->left);
		add_right_node(n);
	} else if (n->data == VSUB && n->right->data == 0) {
		delete_node(n->right);
		add_left_node(n);
	} else if (n->left!= NULL && n->right != NULL){
		if (n->left->type == TNUM && n->right->type == TNUM){
			n->type = TNUM;
			switch (n->data) {
				case VMUL:
					n->data = n->left->data * n->right->data;
					break;
				case VADD:
					n->data = n->left->data + n->right->data;
					break;
				case VSUB:
					n->data = n->left->data - n->right->data;
					break;
				}
			delete_node(n->left);
			delete_node(n->right);
			n->left = NULL;
			n->right = NULL;
		}
	}
	return n;
}

const char* generate_str() {
    srand(time(NULL));
    const char* str;
	int rand_chislo = 1 + rand() % 3;
	switch (rand_chislo) {
		case 1:
			str = " очевидно, что = ";
			break;
		case 2:
			str = " следовательно = ";
			break;
		case 3:
			str = " из не сложных соображений = ";
			break;
	}
}

void print_latex() {
	print_text_in_file("\\documentclass{article}\n"
				"\\usepackage[utf8]{inputenc}\n"
				"\\usepackage[T2A]{fontenc}\n"
				"\\usepackage[english,russian]{babel}\n"
				"\\title{DIFFFFFFFF}\n"
				"\\author{from nikita }\n"
				"\\begin{document}\n"
				"\\maketitle\n");

}

Node* differentiate(Node* n) {
	assert(n);
	Node* new_node;
	switch (n->type) {
		case TNUM:
			return _CONST(0);
		case TX:
			return _CONST(1);
		case TADD:
			new_node = (n->data == VADD)? _ADD(_D(n->left), _D(n->right)) : _SUB(_D(n->right), _D(n->left));
			return new_node;
		case TMUL:
			if (n->data == VMUL) {
				new_node = _ADD(_MUL(_C(n->left), _D(n->right)), _MUL(_C(n->right), _D(n->left)));
				return new_node;
			} else {
				new_node = _DIV(_SUB(_MUL(_C(n->left), _D(n->right)), _MUL(_C(n->right), _D(n->left))), _MUL(_C(n->right), _C(n->right)));
				return new_node;
			}
		case TUNAR:
			if (n->data == VSIN) {
				assert(n->right);
				new_node = _MUL(_COS(_C(n->right)), _D(n->right));
				return new_node;
			} else if (n->data == VCOS) {
				assert(n->right);
				new_node = _MUL(_CONST(-1), _MUL(_SIN(_C(n->right)), _D(n->right)));
				return new_node;
			} else if (n->data == VEXP) {
				new_node = _MUL(_EXP(_C(n->right)), _D(n->right));
				assert(n->right);
				return new_node;
			} else {
				new_node = _DIV(_D(n->right), _MUL(_CONST(2), _QRT(_C(n->right))));
				assert(n->right);
				return new_node;
			}
	}
	return NULL;
}


Node* cpy_node(Node* n) {
	if (n == NULL)
		return NULL;
	Node* new_node = create_node(n->data, n->type, NULL, NULL);
	new_node->left = cpy_node(n->left);
	new_node->right = cpy_node(n->right);
	return new_node;
}

Node* GetP() {
	if (*s == '(') {
		s++;
		Node* val = GetE();
		assert(*s == ')');
		s++;
		return val;
	} else{
		return GetUNAR();
	}
}

Node* GetT() {
	Node* val = GetP();
	while(*s == '*' || *s == '/') {
		Node* new_val = create_node((*s == '*')? VMUL : VDIV, TMUL, NULL, NULL);
		s++;
		new_val->right = GetP();
		new_val->left = val;
		val = new_val;
	}
	return val;
}

Node* GetE() {
	Node* val = GetT();
	while(*s == '+' || *s == '-') {
		Node* new_val = create_node((*s == '+')? VADD : VSUB, TADD, NULL, NULL);
		s++;
		new_val->right = GetT();//правая
		new_val->left = val;
		val = new_val;
	}
	return val;
}

Node* GetUNAR() {
	if (*s == 'c') {
		s += 3;
		Node* new_val = create_node(VCOS, TUNAR, NULL, NULL);
		new_val->right = GetP();
		assert(new_val->right);
		return new_val;
	} else if (*s == 's') {
		s += 3;
		Node* new_val = create_node(VSIN, TUNAR, NULL, NULL);
		new_val->right = GetP();
		assert(new_val->right);
		return new_val;
	} else if (*s == 'e') {
		s += 3;
		Node* new_val = create_node(VEXP, TUNAR, NULL, NULL);
		new_val->right = GetP();
		assert(new_val->right);
		return new_val;
	} else if (*s == 'q') {
		s += 3;
		Node* new_val = create_node(VQRT, TUNAR, NULL, NULL);
		new_val->right = GetP();
		assert(new_val->right);
		return new_val;
	} 
	return GetN();
}

Node* GetG(char* str) {
	s = str;
	Node* val = GetE();
	assert(*s == '#');
	return val;
}

Node* GetN() {
	int val = 0;
	if (*s == 'x') {
		s++;
		return create_node(val, TX, NULL, NULL);
	}
	do {
		val = val * 10 + *s - '0';
		s++;
	} while('0' <= *s && *s <= '9');
	return create_node(val, TNUM, NULL, NULL);
}