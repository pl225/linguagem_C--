/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;
int tmp1(int tmp3, float tmp2){
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
char *tmp12(int tmp14, char *tmp13){
	int  tmp15;
	tmp15 = 4;
	cout << tmp15 << endl;
}
float tmp20(){
	cout << tmp17 << endl;
}

int main(void)
{
	int  tmp16;
	int  tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp21;
	float  tmp22;
	int  tmp23;
	tmp16 = 2;
	tmp17 = tmp16;
	tmp18 = 2;
	tmp19 = tmp17>tmp18;
	tmp23 = !tmp19;
	if (tmp23) goto rotulo_3;
	tmp20();
	tmp21 = 4;
	tmp22 = 4.32;
	tmp1(tmp21,tmp22);
	rotulo_3:
	return 0;
}
