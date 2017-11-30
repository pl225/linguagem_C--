/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp31;
int  tmp32;
int  tmp36;
char * tmp17;

char *tmp1(int tmp4, char *tmp2){
	int  tmp3;
	int  tmp5;
	char * tmp6;
	int  tmp7;
	char * tmp8;
	int  tmp9;
	tmp5 = 4;
	cout << tmp5 << endl;
	tmp7 = 1 + 1;
	tmp6 = (char*) malloc(tmp7);
	strcat(tmp6, "z");
	tmp9 = tmp7;
	tmp8 = (char*) malloc(tmp9);
	strcat(tmp8, tmp6);
	return tmp8;
}

int main(void)
{
	int  tmp10;
	char * tmp11;
	int  tmp12;
	char * tmp13;
	int  tmp14;
	char * tmp15;
	int  tmp16;
	int  tmp18;
	int  tmp19;
	int  tmp20;
	int  tmp21;
	int  tmp22;
	int  tmp23;
	int  tmp24;
	int  tmp25;
	int  tmp26;
	int  tmp27;
	int  tmp28;
	int  tmp29;
	int  tmp30;
	int  tmp33;
	int  tmp34;
	int  tmp35;
	tmp10 = 4;
	tmp12 = 3 + 1;
	tmp11 = (char*) malloc(tmp12);
	strcat(tmp11, "alo");
	tmp14 = tmp12;
	tmp13 = (char*) malloc(tmp14);
	strcat(tmp13, tmp11);
	tmp15 = tmp1(tmp10,tmp13);
	tmp16 = strlen(tmp15);
	tmp18 = tmp16;
	tmp17 = (char*) malloc(tmp18);
	strcat(tmp17, tmp15);
	cout << tmp17 << endl;
	tmp19 = 4;
	tmp20 = 7;
	tmp21 = 2;
	tmp22 = tmp20*tmp21;
	tmp23 = tmp19+tmp22;
	tmp24 = 5;
	tmp25 = tmp23+tmp24;
	tmp32 = tmp25;
	tmp26 = 9;
	tmp27 = 4;
	tmp28 = tmp26/tmp27;
	tmp29 = 7;
	tmp30 = tmp28+tmp29;
	tmp31 = tmp30;
	tmp33 = 2;
	tmp34 = 6;
	tmp35 = tmp33/tmp34;
	tmp36 = tmp35;
	cout << tmp32 << endl;
	cout << tmp31 << endl;
	cout << tmp36 << endl;
	if (tmp12 > 0) free(tmp11);
	if (tmp14 > 0) free(tmp13);
	if (tmp16 > 0) free(tmp15);
	return 0;
}
