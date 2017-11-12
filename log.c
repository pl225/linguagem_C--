/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;
int tmp1(int tmp3, char *tmp2){
	char * tmp10;
	int  tmp11;
	char * tmp4;
	int  tmp5;
	char * tmp6;
	int  tmp7;
	char * tmp8;
	int  tmp9;
	tmp5 = 4 + 1;
	tmp4 = (char*) malloc(tmp5);
	strcat(tmp4, "hhhh");
	tmp7 = tmp5;
	tmp6 = (char*) malloc(tmp7);
	strcat(tmp6, tmp4);
	cout << tmp6 << endl;
	tmp9 = 3 + 1;
	tmp8 = (char*) malloc(tmp9);
	strcat(tmp8, "jjj");
	tmp11 = tmp7 + tmp9;
	tmp11 = tmp11 + 1;
	tmp10 = (char*) malloc(tmp11);
	strcat(tmp10, tmp6);
	strcat(tmp10, tmp8);
	if(tmp7 > 0) free(tmp6);
	tmp7 = tmp11;
	tmp6 = (char*) malloc(tmp7);
	strcat(tmp6, tmp10);
	cout << tmp6 << endl;
}

int main(void)
{
	char *a; // feito manualmente
	tmp1(3, a); // feito manualmente
	return 0;
}
