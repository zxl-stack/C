#include<stdio.h>
int main()
{  int P, Q;
	scanf("%d %d", &P, &Q);
	printf("P合取Q为%d\n", (P && Q));
	printf("P析取Q为%d\n", (P || Q));
	int a, b, c;
	if (P == 1 && Q == 0)
		a = 0;
	else
		a = 1;
	printf("P->Q为%d\n", a);
	if (Q == 1 && P == 0)
		b = 0;
	else
		b = 1;
	printf("Q->P为%d\n", b);
	if (Q == P)
		c = 1;
	else
		c = 0;
	printf("P<->Q为%d", c);
	return 0;
}