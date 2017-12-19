/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp12;
char** tmp9;
int* tmp26;


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
	int  tmp23;
	int  tmp24;
	int  tmp25;
	int  tmp27;
	int  tmp28;
	int  tmp29;
	char * tmp3;
	int  tmp30;
	int  tmp31;
	int  tmp32;
	int  tmp33;
	int  tmp34;
	int  tmp35;
	int  tmp36;
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
	tmp27 = 2;
	tmp28 = 3;
	tmp29 = tmp27 * tmp28;
	tmp26 = (int*) malloc(sizeof(int )*tmp29);
	tmp20 = 4;
	tmp26[0] = tmp20;
	tmp21 = 5;
	tmp26[1] = tmp21;
	tmp22 = 7;
	tmp26[2] = tmp22;
	tmp23 = 5;
	tmp26[3] = tmp23;
	tmp24 = 6;
	tmp26[4] = tmp24;
	tmp25 = 7;
	tmp26[5] = tmp25;
	tmp30 = 1;
	tmp31 = 2;
	if(tmp30 < 0 || tmp30 >= tmp27) exit(1);
	if(tmp31 < 0 || tmp31 >= tmp28) exit(1);
	tmp32 = tmp30 * tmp28;
	tmp32 = tmp32 + tmp31;
	scanf("%d", &tmp26[tmp32]);
	setbuf(stdin, NULL);
	tmp33 = 1;
	tmp34 = 2;
	if(tmp27 < 0 || tmp33 >= tmp27) exit(1);
	if(tmp28 < 0 || tmp34 >= tmp28) exit(1);
	tmp35 = tmp33 * tmp28;
	tmp35 = tmp35 + tmp34;
	tmp36 = tmp26[tmp35];
	cout << tmp36 << endl;
	if (tmp2 > 0) free(tmp1);
	if (tmp16 > 0) free(tmp15);
	if (tmp4 > 0) free(tmp3);
	if (tmp6 > 0) free(tmp5);
	if (tmp8 > 0) free(tmp7);
	return 0;
}
