/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp10;
float * tmp30;
int * tmp8;
float * tmp24;
int * tmp6;
int * tmp15;
int * tmp13;
float  tmp28;


int main(void)
{
	int  tmp1;
	int  tmp11;
	int  tmp12;
	int  tmp14;
	int  tmp16;
	int  tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp2;
	int  tmp20;
	int  tmp21;
	int  tmp22;
	float  tmp23;
	float  tmp25;
	float  tmp26;
	float  tmp27;
	float  tmp29;
	int  tmp3;
	float  tmp31;
	int  tmp32;
	float  tmp33;
	int  tmp4;
	int  tmp5;
	int  tmp7;
	int  tmp9;
	tmp1 = 88;
	tmp10 = tmp1;
	tmp9 = 3;
	tmp8 = (int *) malloc(sizeof(int )*tmp9);
	tmp2 = 4;
	tmp8[0] = tmp2;
	tmp3 = 5;
	tmp8[1] = tmp3;
	tmp4 = 6;
	tmp8[2] = tmp4;
	tmp7 = 1;
	tmp6 = (int *) malloc(sizeof(int )*tmp7);
	tmp5 = 9;
	tmp6[0] = tmp5;
	tmp16 = 1;
	tmp15 = (int *) malloc(sizeof(int )*tmp16);
	tmp11 = 4444;
	tmp15[0] = tmp11;
	tmp12 = 2;
	tmp14 = tmp12;
	tmp13 = (int *) malloc(sizeof(int )*tmp14);
	tmp17 = 2;
	if (tmp17 < 0 || tmp17 >= tmp9) exit(1);
	tmp18 = tmp8[tmp17];
	cout << tmp18 << endl;
	tmp19 = 17;
	tmp31 = tmp19;
	tmp30 = (float *) malloc(sizeof(float )*tmp31);
	tmp20 = 2;
	tmp29 = (float) tmp20;
	tmp28 = tmp29;
	tmp25 = 3;
	tmp24 = (float *) malloc(sizeof(float )*tmp25);
	tmp21 = 4;
	tmp26 = (float) tmp21;
	tmp24[0] = tmp26;
	tmp22 = 12;
	tmp27 = (float) tmp22;
	tmp24[1] = tmp27;
	tmp23 = 44.8;
	tmp24[2] = tmp23;
	tmp32 = 1;
	if (tmp32 < 0 || tmp32 >= tmp25) exit(1);
	tmp33 = tmp24[tmp32];
	cout << tmp33 << endl;
	return 0;
}
