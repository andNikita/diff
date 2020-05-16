#include <stdio.h>
#include <assert.h>

char* s = NULL;
int GetG(char* str);
int GetN();
int GetE();
int get_value();

int main(int argc, char* argv[]) {
	char str[] = "(30/10+15*3)*4#";
	printf("val = %d\n", GetG(str));
}

int get_value() {
	printf("val = %d\n", GetG(str));
}

int GetP() {
	if (*s == '(') {
		s++;
		//printf("*s = %c", *s);
		int val = GetE();
		assert(*s == ')');
		s++;
		return val;
	} else
		return GetN();
}

int GetT() {
	int val = 0;
	val = GetP();
	while(*s == '*' || *s == '/') {
		char op = *s;
		s++;
		int val2 = GetP();
		if (op == '*')
			val *= val2;
		else
			val /= val2;
	}
	return val;
}

int GetE() {
	int val = 0;
	val = GetT();
	while(*s == '+' || *s == '-') {
		char op = *s;
		s++;
		int val2 = GetT();
		if (op == '+')
			val += val2;
		else
			val -= val2;
	}
	return val;
}

int GetG(char* str) {
	s = str;
	int val = GetE();
	assert(*s == '#');
	return val;
}

int GetN() {
	int val = 0;
	do {
		val = val * 10 + *s - '0';
		s++;
	} while('0' <= *s && *s <= '9');
	return val;
}