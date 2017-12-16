/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

char** tmp3;


int main(void)
{
	int  tmp1;
	int  tmp10;
	int  tmp2;
	int  tmp4;
	int  tmp5;
	int  tmp6;
	char * tmp7;
	int  tmp8;
	int  tmp9;
	tmp1 = 4;
	tmp2 = 3;
	tmp4 = tmp1 * tmp2;
	tmp3 = (char**) malloc(sizeof(char *)*tmp4);
	tmp5 = 2;
	tmp6 = 2;
	if(tmp1 < 0 || tmp5 >= tmp1) exit(1);
	if(tmp2 < 0 || tmp6 >= tmp2) exit(1);
	tmp9 = tmp5 * tmp2;
	tmp9 = tmp9 + tmp6;
	tmp8 = 5 + 1;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, "aloha");
	tmp3[tmp9] = tmp7;
	cout << tmp3[tmp9] << endl;
	if (tmp8 > 0) free(tmp7);
	return 0;
}
