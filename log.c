/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;
int main(void)
{
	char * tmp1;
	int  tmp10;
	char * tmp11;
	int  tmp12;
	char * tmp13;
	int  tmp14;
	char * tmp15;
	int  tmp16;
	char * tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp2;
	int  tmp20;
	int  tmp21;
	float  tmp22;
	float  tmp23;
	char * tmp24;
	int  tmp25;
	char * tmp26;
	int  tmp27;
	int  tmp28;
	int  tmp29;
	char * tmp3;
	char  tmp30;
	int  tmp4;
	char * tmp5;
	int  tmp6;
	char * tmp7;
	int  tmp8;
	char * tmp9;
	tmp2 = 4 + 1;
	tmp1 = (char*) malloc(tmp2);
	strcat(tmp1, "kkkk");
	tmp4 = 4 + 1;
	tmp3 = (char*) malloc(tmp4);
	strcat(tmp3, "aaaa");
	tmp6 = tmp2 + tmp4;
	tmp6 = tmp6 + 1;
	tmp5 = (char*) malloc(tmp6);
	strcat(tmp5, tmp1);
	strcat(tmp5, tmp3);
	tmp18 = tmp6;
	tmp17 = (char*) malloc(tmp18);
	strcat(tmp17, tmp5);
	tmp8 = 6 + 1;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, "arvore");
	tmp16 = tmp8;
	tmp15 = (char*) malloc(tmp16);
	strcat(tmp15, tmp7);
	tmp14 = 0;
	tmp10 = 7 + 1;
	tmp9 = (char*) malloc(tmp10);
	strcat(tmp9, "matheus");
	tmp12 = tmp10;
	tmp11 = (char*) malloc(tmp12);
	strcat(tmp11, tmp9);
	tmp19 = 777;
	tmp21 = tmp19;
	tmp22 = 44.4;
	tmp23 = tmp22;
	tmp25 = tmp18 + tmp12;
	tmp25 = tmp25 + 1;
	tmp24 = (char*) malloc(tmp25);
	strcat(tmp24, tmp17);
	strcat(tmp24, tmp11);
	tmp27 = tmp25;
	tmp26 = (char*) malloc(tmp27);
	strcat(tmp26, tmp24);
	tmp28 = 1;
	tmp29 = 0;
	cout << tmp17 << endl;
	cout << tmp11 << endl;
	cout << tmp26 << endl;
	cout << tmp21 << endl;
	cout << tmp23 << endl;
	cout << tmp29 << endl;
	cout << tmp15 << endl;
	if (tmp16 > 0) free(tmp15);
	tmp15 = (char*) malloc(SIZE_STR);
	tmp16 = 0;
	rotulo_3:
	tmp30 = getchar();
	if(tmp30 == '\0' || tmp30 == '\n') goto rotulo_4;
	tmp15[tmp16] = tmp30;
	tmp16 = tmp16 + 1;
	if(tmp16 < SIZE_STR) goto rotulo_3;
	tmp16 = tmp16 + SIZE_STR;
	tmp15 = (char*) realloc(tmp15, tmp16);
	goto rotulo_3;
	rotulo_4:
	tmp16 = tmp16 + 1;
	tmp15[tmp16] = '\0';
	cout << tmp15 << endl;
	return 0;
}
