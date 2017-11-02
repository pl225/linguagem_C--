/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
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
	int  tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp2;
	float  tmp20;
	float  tmp21;
	char * tmp22;
	int  tmp23;
	char * tmp24;
	int  tmp25;
	int  tmp26;
	int  tmp27;
	char * tmp3;
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
	tmp16 = tmp6;
	tmp15 = (char*) malloc(tmp16);
	strcat(tmp15, tmp5);
	tmp14 = 0;
	tmp12 = 0;
	tmp8 = 7 + 1;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, "matheus");
	tmp10 = tmp8;
	tmp9 = (char*) malloc(tmp10);
	strcat(tmp9, tmp7);
	tmp17 = 777;
	tmp19 = tmp17;
	tmp20 = 44.4;
	tmp21 = tmp20;
	tmp23 = tmp16 + tmp10;
	tmp23 = tmp23 + 1;
	tmp22 = (char*) malloc(tmp23);
	strcat(tmp22, tmp15);
	strcat(tmp22, tmp9);
	tmp25 = tmp23;
	tmp24 = (char*) malloc(tmp25);
	strcat(tmp24, tmp22);
	tmp26 = 1;
	tmp27 = 0;
	cout << tmp15 << endl;
	cout << tmp9 << endl;
	cout << tmp24 << endl;
	cout << tmp19 << endl;
	cout << tmp21 << endl;
	cout << tmp27 << endl;
	return 0;
}
