/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp18;

int tmp1(int tmp3, float tmp2){
	char * tmp10;
	int  tmp11;
	int  tmp12;
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
	tmp12 = 4;
	return tmp12;
}
char *tmp13(int tmp15, char *tmp14){
	int  tmp16;
	tmp16 = 4;
	cout << tmp16 << endl;
}
float tmp21(){
	cout << tmp18 << endl;
}

int main(void)
{
	int  tmp17;
	int  tmp19;
	int  tmp20;
	int  tmp22;
	float  tmp23;
	int  tmp24;
	tmp17 = 2;
	tmp18 = tmp17;
	tmp19 = 2;
	tmp20 = tmp18>=tmp19;
	tmp24 = !tmp20;
	if (tmp24) goto rotulo_3;
	tmp21();
	tmp22 = 4;
	tmp23 = 4.32;
	tmp1(tmp22,tmp23);
	rotulo_3:
	return 0;
}
