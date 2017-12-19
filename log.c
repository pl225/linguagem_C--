/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp12;
char** tmp9;
int * tmp23;


int main(void)
{
	char * tmp1;
	int  tmp10;
	int  tmp11;
	int  tmp13;
	int  tmp14;
	char * tmp15;
	int  tmp16;
	int  tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp2;
	int  tmp20;
	int  tmp21;
	int  tmp22;
	int  tmp24;
	int  tmp25;
	char * tmp3;
	int  tmp4;
	char * tmp5;
	int  tmp6;
	char * tmp7;
	int  tmp8;
	tmp10 = 4;
	tmp9 = (char**) malloc(sizeof(char *)*tmp10);
	tmp2 = 2 + 1;
	tmp1 = (char*) malloc(tmp2);
	strcat(tmp1, "ok");
	tmp9[0] = tmp1;
	tmp4 = 3 + 1;
	tmp3 = (char*) malloc(tmp4);
	strcat(tmp3, "vlw");
	tmp9[1] = tmp3;
	tmp6 = 3 + 1;
	tmp5 = (char*) malloc(tmp6);
	strcat(tmp5, "sim");
	tmp9[2] = tmp5;
	tmp8 = 6 + 1;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, "td bem");
	tmp9[3] = tmp7;
	tmp11 = 0;
	tmp12 = tmp11;
	rotulo_3:
	tmp13 = 4;
	tmp14 = tmp12<tmp13;
	tmp19 = !tmp14;
	if (tmp19) goto rotulo_4;
	tmp16 = strlen(tmp9[tmp12]) + 1;
	tmp15 = (char*) malloc(tmp16);
	strcat(tmp15, tmp9[tmp12]);
	cout << tmp15 << endl;
	tmp17 = 1;
	tmp18 = tmp12 + tmp17;
	tmp12 = tmp18;
	goto rotulo_3;
	rotulo_4:
	tmp24 = 3;
	tmp23 = (int *) malloc(sizeof(int )*tmp24);
	tmp20 = 4;
	tmp23[0] = tmp20;
	tmp21 = 5;
	tmp23[1] = tmp21;
	tmp22 = 7;
	tmp23[2] = tmp22;
	if(tmp25 < 0 || tmp25 >= tmp24) exit(1);
	tmp25 = 2;
	scanf("%d", &tmp23[tmp25]);
	setbuf(stdin, NULL);
	if (tmp2 > 0) free(tmp1);
	if (tmp16 > 0) free(tmp15);
	if (tmp4 > 0) free(tmp3);
	if (tmp6 > 0) free(tmp5);
	if (tmp8 > 0) free(tmp7);
	return 0;
}
