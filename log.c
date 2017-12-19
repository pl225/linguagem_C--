/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

char** tmp9;
int* tmp28;


int main(void)
{
	char * tmp1;
	int  tmp10;
	int  tmp11;
	int  tmp12;
	int  tmp13;
	int  tmp14;
	char  tmp15;
	int  tmp16;
	int  tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp2;
	char * tmp20;
	int  tmp21;
	int  tmp22;
	int  tmp23;
	int  tmp24;
	int  tmp25;
	int  tmp26;
	int  tmp27;
	int  tmp29;
	char * tmp3;
	int  tmp30;
	int  tmp31;
	int  tmp32;
	int  tmp33;
	int  tmp34;
	int  tmp35;
	int  tmp36;
	int  tmp37;
	int  tmp38;
	int  tmp4;
	char * tmp5;
	int  tmp6;
	char * tmp7;
	int  tmp8;
	tmp10 = 2;
	tmp11 = 2;
	tmp12 = tmp10 * tmp11;
	tmp9 = (char**) malloc(sizeof(char *)*tmp12);
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
	tmp13 = 0;
	tmp14 = 1;
	if(tmp13 < 0 || tmp13 >= tmp10) exit(1);
	if(tmp14 < 0 || tmp14 >= tmp11) exit(1);
	tmp16 = tmp13 * tmp11;
	tmp16 = tmp16 + tmp14;
	if (tmp10 > 0) free(tmp9[tmp16]);
	tmp9[tmp16] = (char*) malloc(SIZE_STR);
	tmp10 = 0;
	rotulo_3:
	tmp15 = getchar();
	if(tmp15 == '\0' || tmp15 == '\n') goto rotulo_4;
	tmp9[tmp16][tmp10] = tmp15;
	tmp10 = tmp10 + 1;
	if(tmp10 < SIZE_STR) goto rotulo_3;
	tmp10 = tmp10 + SIZE_STR;
	tmp9[tmp16] = (char*) realloc(tmp9[tmp16], tmp10);
	goto rotulo_3;
	rotulo_4:
	tmp10 = tmp10 + 1;
	tmp9[tmp16][tmp10] = '\0';
	setbuf(stdin, NULL);
	tmp17 = 0;
	tmp18 = 1;
	if(tmp10 < 0 || tmp17 >= tmp10) exit(1);
	if(tmp11 < 0 || tmp18 >= tmp11) exit(1);
	tmp19 = tmp17 * tmp11;
	tmp19 = tmp19 + tmp18;
	tmp21 = strlen(tmp9[tmp19]) + 1;
	tmp20 = (char*) malloc(tmp21);
	strcat(tmp20, tmp9[tmp19]);
	cout << tmp20 << endl;
	tmp29 = 2;
	tmp30 = 3;
	tmp31 = tmp29 * tmp30;
	tmp28 = (int*) malloc(sizeof(int )*tmp31);
	tmp22 = 4;
	tmp28[0] = tmp22;
	tmp23 = 5;
	tmp28[1] = tmp23;
	tmp24 = 7;
	tmp28[2] = tmp24;
	tmp25 = 5;
	tmp28[3] = tmp25;
	tmp26 = 6;
	tmp28[4] = tmp26;
	tmp27 = 7;
	tmp28[5] = tmp27;
	tmp32 = 1;
	tmp33 = 2;
	if(tmp32 < 0 || tmp32 >= tmp29) exit(1);
	if(tmp33 < 0 || tmp33 >= tmp30) exit(1);
	tmp34 = tmp32 * tmp30;
	tmp34 = tmp34 + tmp33;
	scanf("%d", &tmp28[tmp34]);
	setbuf(stdin, NULL);
	tmp35 = 1;
	tmp36 = 2;
	if(tmp29 < 0 || tmp35 >= tmp29) exit(1);
	if(tmp30 < 0 || tmp36 >= tmp30) exit(1);
	tmp37 = tmp35 * tmp30;
	tmp37 = tmp37 + tmp36;
	tmp38 = tmp28[tmp37];
	cout << tmp38 << endl;
	if (tmp2 > 0) free(tmp1);
	if (tmp21 > 0) free(tmp20);
	if (tmp4 > 0) free(tmp3);
	if (tmp6 > 0) free(tmp5);
	if (tmp8 > 0) free(tmp7);
	return 0;
}
