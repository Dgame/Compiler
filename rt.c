#include <stdio.h>

extern void prog();
void print_int(int n);
void print_3_ints(int a, int b, int c);

int main() {
	prog();
}

void print_int(int n) {
	printf("%d\n", n);
}

void print_3_ints(int a, int b, int c) {
	printf("3 ints: %d %d %d\n", a, b, c);
}