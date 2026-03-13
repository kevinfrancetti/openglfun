#include <stdio.h>



typedef struct{
	int x;
	int y;
}Point;



//mai
int main(){

	const Point p1;
	Point p2;
	Point* ptr1 = &p1;



	//p1.x = 123;
	(*ptr1).x = 66;

	//p1 = p2;

	int var1 = 4;
	int var2 = 9;
	int* const p_var1 = &var1;
	p_var1 = &var2;
	(*p_var1) = 6;


	printf("Hello test1 %d \n", p1.x);
	printf("Hello test2 %d \n", p2.x);
	printf("Hello test3 %d \n", var1);

//	printf("Hello test3 %d \n", sizeof(int));
//	printf("Hello test4 %d \n", sizeof(void));
//	printf("Hello test5 %d \n", sizeof(void*));
//	printf("Hello test6 %d \n", sizeof(int*));


	return 0;
}
