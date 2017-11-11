#include <stdio.h>	
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	void **array = (void**)malloc(4 * sizeof(void*));

	int a = 4; float b = 5; char c = 'h'; char e [] = "matheus";

	char* g = (char*) malloc(10);
	strcat(g, "notebook");

	array[0] = (void*) &a;
	array[1] = (void*) &b;
	array[2] = (void*) &c;
	array[3] = (void*) g ; // &e

	printf("%d\n", *((int*) array[0]));
	printf("%f\n", *((float*) array[1]));
	printf("%c\n", *((char*) array[2]));
	printf("%s\n", (char*) array[3]); //printf("%c\n", ((char*) array[3])[1]);

	free(g);
	free(array);
}