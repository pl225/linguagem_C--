/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp10;
int * tmp8;
int * tmp6;
int * tmp15;
int * tmp13;


int main(void)
{
	int  tmp1;
	int  tmp11;
	int  tmp12;
	int  tmp14;
	int  tmp16;
	int  tmp2;
	int  tmp3;
	int  tmp4;
	int  tmp5;
	int  tmp7;
	int  tmp9;
	tmp1 = 88;
	tmp10 = tmp1;
	tmp9 = 3;
	tmp8 = (int*) malloc(sizeof(int)*tmp9);
	tmp2 = 4;
	tmp8[0] = tmp2;
	tmp3 = 5;
	tmp8[1] = tmp3;
	tmp4 = 6;
	tmp8[2] = tmp4;
	tmp7 = 1;
	tmp6 = (int*) malloc(sizeof(int)*tmp7);
	tmp5 = 9;
	tmp6[0] = tmp5;
	tmp16 = 1;
	tmp15 = (int*) malloc(sizeof(int)*tmp16);
	tmp11 = 4444;
	tmp15[0] = tmp11;
	tmp12 = 2;
	tmp14 = tmp12;
	tmp13 = (int*) malloc(sizeof(int)*tmp14);
	return 0;
}
