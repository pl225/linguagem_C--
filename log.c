/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int* tmp9;
int* tmp6;
float* tmp18;
int  tmp8;
char** tmp26;


int main(void)
{
	int  tmp1;
	int  tmp10;
	int  tmp11;
	int  tmp12;
	float  tmp13;
	int  tmp14;
	float  tmp15;
	int  tmp16;
	int  tmp17;
	int  tmp19;
	int  tmp2;
	int  tmp20;
	int  tmp21;
	int  tmp22;
	int  tmp23;
	float  tmp24;
	int  tmp25;
	int  tmp27;
	int  tmp28;
	char * tmp29;
	int  tmp3;
	int  tmp30;
	int  tmp31;
	char * tmp32;
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
	tmp11 = 1;
	tmp12 = 4;
	if(tmp1 < 0 || tmp11 >= tmp1) exit(1);
	if(tmp2 < 0 || tmp12 >= tmp2) exit(1);
	tmp14 = tmp11 * tmp2;
	tmp14 = tmp14 + tmp12;
	tmp13 = 14.5;
	tmp15 = (float ) tmp13;
	tmp9[tmp14] = tmp15;
	tmp16 = 2;
	tmp17 = 3;
	tmp19 = tmp16 + tmp17;
	tmp18 = (float*) malloc(sizeof(float )*tmp19);
	tmp20 = 1;
	tmp21 = 2;
	if(tmp16 < 0 || tmp20 >= tmp16) exit(1);
	if(tmp17 < 0 || tmp21 >= tmp17) exit(1);
	tmp23 = tmp20 * tmp17;
	tmp23 = tmp23 + tmp21;
	tmp22 = 27;
	tmp24 = (float ) tmp22;
	tmp18[tmp23] = tmp24;
	tmp25 = 2;
	tmp27 = tmp25;
	tmp26 = (char**) malloc(sizeof(char *)*tmp27);
	tmp28 = 1;
	tmp30 = 3 + 1;
	tmp29 = (char*) malloc(tmp30);
	strcat(tmp29, "OMG");
	if(tmp28 < 0 || tmp28 >= tmp27) exit(1);
	tmp26[tmp28] = (char*) realloc(tmp26[tmp28], tmp30);
	strcpy(tmp26[tmp28], tmp29);
	tmp31 = 1;
	tmp33 = strlen(tmp26[tmp31]) + 1;
	tmp32 = (char*) malloc(tmp33);
	strcat(tmp32, tmp26[tmp31]);
	cout << tmp32 << endl;
	if (tmp30 > 0) free(tmp29);
	if (tmp33 > 0) free(tmp32);
	return 0;
}
