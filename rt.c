#include <stdio.h>

extern void prog();
void print_int(int n);

int main() {
	prog();
}

void print_int(int n) {
	printf("%d\n", n);
}