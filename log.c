/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

char * tmp14;

char *tmp1(int tmp3, char *tmp2){
	int  tmp4;
	char * tmp5;
	int  tmp6;
	tmp4 = 4;
	cout << tmp4 << endl;
	tmp6 = 14 + 1;
	tmp5 = (char*) malloc(tmp6);
	strcat(tmp5, "retorno_funcao");
	return tmp5;
}

int main(void)
{
	char * tmp10;
	int  tmp11;
	char * tmp12;
	int  tmp13;
	int  tmp15;
	int  tmp7;
	char * tmp8;
	int  tmp9;
	tmp7 = 4;
	tmp9 = 3 + 1;
	tmp8 = (char*) malloc(tmp9);
	strcat(tmp8, "alo");
	tmp11 = tmp9;
	tmp10 = (char*) malloc(tmp11);
	strcat(tmp10, tmp8);
	tmp12 = tmp1(tmp7,tmp10);
	tmp13 = strlen(tmp12);
	tmp15 = tmp13;
	tmp14 = (char*) malloc(tmp15);
	strcat(tmp14, tmp12);
	cout << tmp14 << endl;
	if (tmp11 > 0) free(tmp10);
	if (tmp13 > 0) free(tmp12);
	if (tmp9 > 0) free(tmp8);
	return 0;
}
