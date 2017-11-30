/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

char * tmp16;

char *tmp1(int tmp3, char *tmp2){
	int  tmp4;
	char * tmp5;
	int  tmp6;
	char * tmp7;
	int  tmp8;
	tmp4 = 4;
	cout << tmp4 << endl;
	tmp6 = 1 + 1;
	tmp5 = (char*) malloc(tmp6);
	strcat(tmp5, "z");
	tmp8 = tmp6;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, tmp5);
	return tmp7;
}

int main(void)
{
	char * tmp10;
	int  tmp11;
	char * tmp12;
	int  tmp13;
	char * tmp14;
	int  tmp15;
	int  tmp17;
	int  tmp9;
	tmp9 = 4;
	tmp11 = 3 + 1;
	tmp10 = (char*) malloc(tmp11);
	strcat(tmp10, "alo");
	tmp13 = tmp11;
	tmp12 = (char*) malloc(tmp13);
	strcat(tmp12, tmp10);
	tmp14 = tmp1(tmp9,tmp12);
	tmp15 = strlen(tmp14);
	tmp17 = tmp15;
	tmp16 = (char*) malloc(tmp17);
	strcat(tmp16, tmp14);
	cout << tmp16 << endl;
	if (tmp11 > 0) free(tmp10);
	if (tmp13 > 0) free(tmp12);
	if (tmp15 > 0) free(tmp14);
	return 0;
}
