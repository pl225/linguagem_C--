/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

char  tmp5;
char  tmp4;
int  tmp50;
char  tmp3;
int  tmp9;
int  tmp8;
int  tmp43;
char  tmp51;
float  tmp53;
int  tmp47;
int  tmp42;
char * tmp62;
char * tmp28;
int  tmp10;

char *tmp13(int tmp15, char tmp14){
	int  tmp16;
	char * tmp17;
	int  tmp18;
	char * tmp19;
	int  tmp20;
	tmp16 = 4;
	cout << tmp16 << endl;
	tmp18 = 1 + 1;
	tmp17 = (char*) malloc(tmp18);
	strcat(tmp17, "z");
	tmp20 = tmp18;
	tmp19 = (char*) malloc(tmp20);
	strcat(tmp19, tmp17);
	cout << tmp14 << endl;
	return tmp19;
}
int tmp21(int tmp22){
	int  tmp23;
	cout << tmp22 << endl;
	tmp23 = 0;
	return tmp23;
}

int main(void)
{
	char  tmp1;
	char  tmp11;
	int  tmp12;
	char  tmp2;
	int  tmp24;
	char  tmp25;
	char * tmp26;
	int  tmp27;
	int  tmp29;
	int  tmp30;
	int  tmp31;
	int  tmp32;
	int  tmp33;
	int  tmp34;
	int  tmp35;
	int  tmp36;
	int  tmp37;
	int  tmp38;
	int  tmp39;
	int  tmp40;
	int  tmp41;
	int  tmp44;
	int  tmp45;
	int  tmp46;
	char  tmp48;
	int  tmp49;
	float  tmp52;
	int  tmp54;
	int  tmp55;
	int  tmp56;
	int  tmp57;
	int  tmp58;
	int  tmp59;
	int  tmp6;
	char * tmp60;
	int  tmp61;
	int  tmp63;
	int  tmp7;
	tmp1 = 'j';
	tmp5 = tmp1;
	tmp2 = 'c';
	tmp3 = tmp2;
	tmp6 = 1;
	tmp10 = tmp6;
	tmp7 = 0;
	tmp9 = tmp7;
	tmp11 = 'k';
	tmp4 = tmp11;
	tmp12 = 1;
	tmp8 = tmp12;
	tmp4 = tmp3;
	cout << tmp4 << endl;
	cout << tmp8 << endl;
	tmp24 = 4;
	tmp25 = 'a';
	tmp26 = tmp13(tmp24,tmp25);
	tmp27 = strlen(tmp26);
	tmp29 = tmp27;
	tmp28 = (char*) malloc(tmp29);
	strcat(tmp28, tmp26);
	cout << tmp28 << endl;
	tmp30 = 4;
	tmp31 = 7;
	tmp32 = 2;
	tmp33 = tmp31%tmp32;
	tmp34 = tmp30+tmp33;
	tmp35 = 5;
	tmp36 = tmp34+tmp35;
	tmp43 = tmp36;
	tmp37 = 9;
	tmp38 = 4;
	tmp39 = tmp37/tmp38;
	tmp40 = 7;
	tmp41 = tmp39+tmp40;
	tmp42 = tmp41;
	tmp44 = 2;
	tmp45 = 6;
	tmp46 = tmp44%tmp45;
	tmp47 = tmp46;
	tmp48 = 'o';
	tmp51 = tmp48;
	tmp49 = 47;
	tmp50 = tmp49;
	tmp52 = 666.66666;
	tmp53 = tmp52;
	tmp54 = tmp43 + tmp43;
	tmp43 = tmp54;
	tmp55 = tmp47 * tmp42;
	tmp47 = tmp55;
	tmp56 = 2;
	tmp57 = tmp42 % tmp56;
	tmp42 = tmp57;
	cout << tmp43 << endl;
	cout << tmp42 << endl;
	cout << tmp47 << endl;
	tmp58 = 1;
	tmp59 = tmp21(tmp58);
	cout << tmp59 << endl;
	cout << tmp50 << endl;
	cout << tmp51 << endl;
	cout << tmp53 << endl;
	tmp61 = 5 + 1;
	tmp60 = (char*) malloc(tmp61);
	strcat(tmp60, "pular");
	tmp63 = tmp61;
	tmp62 = (char*) malloc(tmp63);
	strcat(tmp62, tmp60);
	cout << tmp62 << endl;
	if (tmp27 > 0) free(tmp26);
	if (tmp61 > 0) free(tmp60);
	return 0;
}
