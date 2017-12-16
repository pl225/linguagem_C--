/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int* tmp9;
float* tmp20;
char** tmp32;
int* tmp6;
float* tmp16;
char** tmp28;
int  tmp8;
float  tmp18;
char * tmp30;


int main(void)
{
	int  tmp1;
	int  tmp10;
	int  tmp11;
	int  tmp12;
	int  tmp13;
	int  tmp14;
	int  tmp15;
	int  tmp17;
	float  tmp19;
	int  tmp2;
	int  tmp21;
	int  tmp22;
	int  tmp23;
	char * tmp24;
	int  tmp25;
	int  tmp26;
	int  tmp27;
	int  tmp29;
	int  tmp3;
	int  tmp31;
	int  tmp33;
	int  tmp4;
	int  tmp5;
	int  tmp7;
	tmp1 = 3;
	tmp2 = 5;
	tmp10 = tmp1 + tmp2;
	tmp9 = (int*) malloc(sizeof(int )*tmp10);
	tmp3 = 4;
	tmp8 = tmp3;
	tmp4 = 7;
	tmp5 = 2;
	tmp7 = tmp4 + tmp5;
	tmp6 = (int*) malloc(sizeof(int )*tmp7);
	tmp11 = 5;
	tmp12 = 9;
	tmp21 = tmp11 + tmp12;
	tmp20 = (float*) malloc(sizeof(float )*tmp21);
	tmp13 = 7;
	tmp19 = (float) tmp13;
	tmp18 = tmp19;
	tmp14 = 12;
	tmp15 = 3;
	tmp17 = tmp14 + tmp15;
	tmp16 = (float*) malloc(sizeof(float )*tmp17);
	tmp22 = 4;
	tmp23 = 6;
	tmp33 = tmp22 + tmp23;
	tmp32 = (char**) malloc(sizeof(char *)*tmp33);
	tmp25 = 3 + 1;
	tmp24 = (char*) malloc(tmp25);
	strcat(tmp24, "abc");
	tmp31 = tmp25;
	tmp30 = (char*) malloc(tmp31);
	strcat(tmp30, tmp24);
	tmp26 = 3;
	tmp27 = 5;
	tmp29 = tmp26 + tmp27;
	tmp28 = (char**) malloc(sizeof(char *)*tmp29);
	if (tmp25 > 0) free(tmp24);
	return 0;
}
