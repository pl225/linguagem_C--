/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;
int main(void)
{
	char * tmp1;
	int  tmp10;
	char * tmp11;
	int  tmp12;
	char * tmp13;
	int  tmp14;
	char * tmp15;
	int  tmp16;
	char * tmp17;
	int  tmp18;
	int  tmp19;
	int  tmp2;
	int  tmp20;
	int  tmp21;
	float  tmp22;
	float  tmp23;
	char * tmp24;
	int  tmp25;
	char * tmp26;
	int  tmp27;
	char  tmp28;
	int  tmp29;
	char * tmp3;
	int  tmp30;
	int  tmp31;
	char  tmp32;
	char  tmp33;
	int  tmp34;
	int  tmp35;
	int  tmp36;
	int  tmp37;
	int  tmp38;
	int  tmp39;
	int  tmp4;
	int  tmp40;
	int  tmp41;
	int  tmp42;
	int  tmp43;
	int  tmp44;
	int  tmp45;
	int  tmp46;
	int  tmp47;
	int  tmp48;
	int  tmp49;
	char * tmp5;
	int  tmp50;
	int  tmp51;
	int  tmp52;
	int  tmp53;
	int  tmp54;
	int  tmp55;
	int  tmp56;
	int  tmp57;
	int  tmp58;
	int  tmp59;
	int  tmp6;
	int  tmp60;
	char * tmp7;
	int  tmp8;
	char * tmp9;
	tmp2 = 4 + 1;
	tmp1 = (char*) malloc(tmp2);
	strcat(tmp1, "kkkk");
	tmp4 = 4 + 1;
	tmp3 = (char*) malloc(tmp4);
	strcat(tmp3, "aaaa");
	tmp6 = tmp2 + tmp4;
	tmp6 = tmp6 + 1;
	tmp5 = (char*) malloc(tmp6);
	strcat(tmp5, tmp1);
	strcat(tmp5, tmp3);
	tmp18 = tmp6;
	tmp17 = (char*) malloc(tmp18);
	strcat(tmp17, tmp5);
	tmp8 = 6 + 1;
	tmp7 = (char*) malloc(tmp8);
	strcat(tmp7, "arvore");
	tmp16 = tmp8;
	tmp15 = (char*) malloc(tmp16);
	strcat(tmp15, tmp7);
	tmp14 = 0;
	tmp10 = 7 + 1;
	tmp9 = (char*) malloc(tmp10);
	strcat(tmp9, "matheus");
	tmp12 = tmp10;
	tmp11 = (char*) malloc(tmp12);
	strcat(tmp11, tmp9);
	tmp19 = 777;
	tmp21 = tmp19;
	tmp22 = 44.4;
	tmp23 = tmp22;
	tmp25 = tmp18 + tmp12;
	tmp25 = tmp25 + 1;
	tmp24 = (char*) malloc(tmp25);
	strcat(tmp24, tmp17);
	strcat(tmp24, tmp11);
	tmp27 = tmp25;
	tmp26 = (char*) malloc(tmp27);
	strcat(tmp26, tmp24);
	tmp29 = 1;
	tmp30 = 0;
	cout << tmp17 << endl;
	cout << tmp11 << endl;
	cout << tmp26 << endl;
	cout << tmp21 << endl;
	cout << tmp23 << endl;
	cout << tmp30 << endl;
	cout << tmp15 << endl;
	tmp31 = 9999;
	cout << tmp31 << endl;
	tmp32 = 'b';
	cout << tmp32 << endl;
	scanf("%f", &tmp23);
	setbuf(stdin, NULL);
	if (tmp16 > 0) free(tmp15);
	tmp15 = (char*) malloc(SIZE_STR);
	tmp16 = 0;
	rotulo_3:
	tmp33 = getchar();
	if(tmp33 == '\0' || tmp33 == '\n') goto rotulo_4;
	tmp15[tmp16] = tmp33;
	tmp16 = tmp16 + 1;
	if(tmp16 < SIZE_STR) goto rotulo_3;
	tmp16 = tmp16 + SIZE_STR;
	tmp15 = (char*) realloc(tmp15, tmp16);
	goto rotulo_3;
	rotulo_4:
	tmp16 = tmp16 + 1;
	tmp15[tmp16] = '\0';
	setbuf(stdin, NULL);
	scanf("%d", &tmp20);
	setbuf(stdin, NULL);
	scanf("%c", &tmp28);
	setbuf(stdin, NULL);
	tmp34 = strcmp(tmp17, tmp15);
	tmp34 = tmp34> 0;
	tmp35 = !tmp34;
	if (tmp35) goto rotulo_5;
	cout << tmp17 << endl;
	goto rotulo_6;
	rotulo_5:
	cout << tmp15 << endl;
	rotulo_6:
	tmp36 = tmp21;
	tmp37 = 45;
	if (tmp37 != tmp36) goto rotulo_8;
	cout << tmp17 << endl;
	goto rotulo_7;
	rotulo_8:
	tmp38 = 778;
	if (tmp38 != tmp36) goto rotulo_9;
	cout << tmp21 << endl;
	goto rotulo_7;
	rotulo_9:
	cout << tmp26 << endl;
	goto rotulo_7;
	rotulo_7:
	tmp39 = 0;
	tmp21 = tmp39;
	rotulo_10:
	tmp40 = 10;
	tmp41 = tmp21<tmp40;
	tmp60 = !tmp41;
	if (tmp60) goto rotulo_11;
	tmp42 = 2;
	tmp43 = tmp21==tmp42;
	tmp44 = 4;
	tmp45 = tmp21==tmp44;
	tmp46 = tmp43 || tmp45;
	tmp47 = 7;
	tmp48 = tmp21==tmp47;
	tmp49 = tmp46 || tmp48;
	tmp59 = !tmp49;
	if (tmp59) goto rotulo_16;
	cout << tmp21 << endl;
	goto rotulo_17;
	rotulo_16:
	tmp50 = 1;
	tmp51 = tmp21==tmp50;
	tmp52 = 3;
	tmp53 = tmp21==tmp52;
	tmp54 = tmp51 || tmp53;
	tmp58 = !tmp54;
	if (tmp58) goto rotulo_14;
	cout << tmp23 << endl;
	goto rotulo_15;
	rotulo_14:
	tmp55 = 4;
	tmp56 = tmp21==tmp55;
	tmp57 = !tmp56;
	if (tmp57) goto rotulo_12;
	cout << tmp15 << endl;
	goto rotulo_13;
	rotulo_12:
	cout << tmp26 << endl;
	rotulo_13:
	rotulo_15:
	rotulo_17:
	tmp21 = tmp21 + 1;
	goto rotulo_10;
	rotulo_11:
	if (tmp2 > 0) free(tmp1);
	if (tmp12 > 0) free(tmp11);
	if (tmp14 > 0) free(tmp13);
	if (tmp16 > 0) free(tmp15);
	if (tmp18 > 0) free(tmp17);
	if (tmp25 > 0) free(tmp24);
	if (tmp27 > 0) free(tmp26);
	if (tmp4 > 0) free(tmp3);
	if (tmp6 > 0) free(tmp5);
	if (tmp8 > 0) free(tmp7);
	if (tmp10 > 0) free(tmp9);
	return 0;
}
