/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

char** tmp3;
int* tmp18;


int main(void)
{
	int  tmp1;
	int  tmp10;
	int  tmp11;
	int  tmp12;
	int  tmp13;
	char * tmp14;
	int  tmp15;
	int  tmp16;
	int  tmp17;
	int  tmp19;
	int  tmp2;
	int  tmp20;
	int  tmp21;
	int  tmp22;
	int  tmp23;
	int  tmp24;
	int  tmp25;
	int  tmp26;
	int  tmp27;
	int  tmp4;
	int  tmp5;
	int  tmp6;
	char * tmp7;
	int  tmp8;
	int  tmp9;
	tmp1 = 4;
	tmp2 = 3;
	tmp4 = tmp1 * tmp2;
	tmp3 = (char**) malloc(sizeof(char *)*tmp4);
	tmp5 = 2;
	tmp6 = 2;
	if(tmp1 < 0 || tmp5 >= tmp1) exit(1);
	if(tmp2 < 0 || tmp6 >= tmp2) exit(1);
	tmp9 = tmp5 * tmp2;
	tmp9 = tmp9 + tmp6;
	tmp8 = 5 + 1;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, "aloha");
	tmp3[tmp9] = tmp7;
	tmp11 = 2;
	tmp12 = 2;
	if(tmp1 < 0 || tmp11 >= tmp1) exit(1);
	if(tmp2 < 0 || tmp12 >= tmp2) exit(1);
	tmp13 = tmp11 * tmp2;
	tmp13 = tmp13 + tmp12;
	tmp15 = strlen(tmp3[tmp13]) + 1;
	tmp14 = (char*) malloc(tmp15);
	strcat(tmp14, tmp3[tmp13]);
	cout << tmp14 << endl;
	tmp16 = 4;
	tmp17 = 5;
	tmp19 = tmp16 * tmp17;
	tmp18 = (int*) malloc(sizeof(int )*tmp19);
	tmp20 = 2;
	tmp21 = 3;
	if(tmp16 < 0 || tmp20 >= tmp16) exit(1);
	if(tmp17 < 0 || tmp21 >= tmp17) exit(1);
	tmp23 = tmp20 * tmp17;
	tmp23 = tmp23 + tmp21;
	tmp22 = 12;
	tmp18[tmp23] = tmp22;
	tmp24 = 2;
	tmp25 = 3;
	if(tmp16 < 0 || tmp24 >= tmp16) exit(1);
	if(tmp17 < 0 || tmp25 >= tmp17) exit(1);
	tmp26 = tmp24 * tmp17;
	tmp26 = tmp26 + tmp25;
	tmp27 = tmp18[tmp26];
	cout << tmp27 << endl;
	if (tmp15 > 0) free(tmp14);
	if (tmp8 > 0) free(tmp7);
	return 0;
}
