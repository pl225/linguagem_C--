/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp19;

int tmp1(int tmp3, float tmp2){
	char * tmp10;
	int  tmp11;
	float  tmp12;
	int  tmp13;
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
	tmp12 = 4.5;
	tmp13 = (int ) tmp12;
	return tmp13;
}
char *tmp14(int tmp16, char *tmp15){
	int  tmp17;
	tmp17 = 4;
	cout << tmp17 << endl;
}

int main(void)
{
	int  tmp18;
	int  tmp20;
	int  tmp21;
	int  tmp22;
	int  tmp23;
	float  tmp24;
	float  tmp25;
	int  tmp26;
	int  tmp27;
	tmp18 = 2;
	tmp19 = tmp18;
	tmp20 = 2;
	tmp21 = tmp19>=tmp20;
	tmp27 = !tmp21;
	if (tmp27) goto rotulo_3;
	tmp22 = 3;
	tmp23 = tmp22;
	tmp24 = 4.27;
	tmp25 = 4.32;
	tmp26 = (int ) tmp24;
	tmp1(tmp26,tmp25);
	rotulo_3:
	return 0;
}
