/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

float  tmp12;
float  tmp10;
char** tmp60;
char** tmp66;
float * tmp27;
int * tmp46;
float * tmp14;
int * tmp39;


int main(void)
{
	int  tmp1;
	float  tmp11;
	float  tmp13;
	int  tmp15;
	float  tmp16;
	float  tmp17;
	float  tmp18;
	float  tmp19;
	int  tmp2;
	float  tmp20;
	float  tmp21;
	float  tmp22;
	int  tmp23;
	int  tmp24;
	float * tmp25;
	int  tmp26;
	int  tmp28;
	int  tmp29;
	int  tmp3;
	float  tmp30;
	int  tmp31;
	float  tmp32;
	float  tmp33;
	int  tmp34;
	float  tmp35;
	int  tmp36;
	int  tmp37;
	int  tmp38;
	int  tmp4;
	int  tmp40;
	int  tmp41;
	int  tmp42;
	int  tmp43;
	int * tmp44;
	int  tmp45;
	int  tmp47;
	int  tmp48;
	int  tmp49;
	int  tmp5;
	float  tmp50;
	float  tmp51;
	char * tmp52;
	int  tmp53;
	char * tmp54;
	int  tmp55;
	char * tmp56;
	int  tmp57;
	char * tmp58;
	int  tmp59;
	int  tmp6;
	int  tmp61;
	int  tmp62;
	int  tmp63;
	char** tmp64;
	int  tmp65;
	int  tmp67;
	int  tmp68;
	char * tmp69;
	int  tmp7;
	int  tmp70;
	char * tmp71;
	int  tmp72;
	char * tmp73;
	int  tmp74;
	int  tmp8;
	int  tmp9;
	tmp15 = 7;
	tmp14 = (float *) malloc(sizeof(float )*tmp15);
	tmp1 = 2;
	tmp16 = (float) tmp1;
	tmp14[0] = tmp16;
	tmp2 = 4;
	tmp17 = (float) tmp2;
	tmp14[1] = tmp17;
	tmp3 = 5;
	tmp18 = (float) tmp3;
	tmp14[2] = tmp18;
	tmp4 = 6;
	tmp19 = (float) tmp4;
	tmp14[3] = tmp19;
	tmp5 = 7;
	tmp20 = (float) tmp5;
	tmp14[4] = tmp20;
	tmp6 = 8;
	tmp21 = (float) tmp6;
	tmp14[5] = tmp21;
	tmp7 = 8;
	tmp22 = (float) tmp7;
	tmp14[6] = tmp22;
	tmp8 = 3;
	tmp13 = (float) tmp8;
	tmp12 = tmp13;
	tmp9 = 5;
	tmp11 = (float) tmp9;
	tmp10 = tmp11;
	tmp23 = 2;
	tmp24 = 5;
	if(tmp23 < 0 || tmp23 >= tmp15) exit(1);
	if(tmp24 < 0 || tmp24 >= tmp15) exit(1);
	tmp26 = tmp24 - tmp23;
	if(tmp26 <= 0) exit(1);
	tmp26 = tmp26 + 1;
	tmp25 = tmp14 + tmp23;
	tmp28 = tmp26;
	tmp27 = tmp25;
	tmp29 = 0;
	if (tmp29 < 0 || tmp29 >= tmp28) exit(1);
	tmp30 = tmp27[tmp29];
	cout << tmp30 << endl;
	tmp31 = 2;
	if (tmp31 < 0 || tmp31 >= tmp28) exit(1);
	tmp32 = tmp27[tmp31];
	tmp33 = tmp32+tmp10;
	cout << tmp33 << endl;
	tmp40 = 5;
	tmp39 = (int *) malloc(sizeof(int )*tmp40);
	tmp34 = 11;
	tmp39[0] = tmp34;
	tmp35 = 14.6;
	tmp41 = (int) tmp35;
	tmp39[1] = tmp41;
	tmp36 = 89;
	tmp39[2] = tmp36;
	tmp37 = 56;
	tmp39[3] = tmp37;
	tmp38 = 90;
	tmp39[4] = tmp38;
	tmp42 = 1;
	tmp43 = 4;
	if(tmp42 < 0 || tmp42 >= tmp40) exit(1);
	if(tmp43 < 0 || tmp43 >= tmp40) exit(1);
	tmp45 = tmp43 - tmp42;
	if(tmp45 <= 0) exit(1);
	tmp45 = tmp45 + 1;
	tmp44 = tmp39 + tmp42;
	tmp47 = tmp45;
	tmp46 = tmp44;
	tmp48 = 2;
	if (tmp48 < 0 || tmp48 >= tmp47) exit(1);
	tmp49 = tmp46[tmp48];
	tmp51 = (float) tmp49;
	tmp50 = tmp51+tmp12;
	cout << tmp50 << endl;
	tmp61 = 4;
	tmp60 = (char**) malloc(sizeof(char *)*tmp61);
	tmp53 = 2 + 1;
	tmp52 = (char*) malloc(tmp53);
	strcat(tmp52, "ok");
	tmp60[0] = tmp52;
	tmp55 = 3 + 1;
	tmp54 = (char*) malloc(tmp55);
	strcat(tmp54, "vlw");
	tmp60[1] = tmp54;
	tmp57 = 3 + 1;
	tmp56 = (char*) malloc(tmp57);
	strcat(tmp56, "sim");
	tmp60[2] = tmp56;
	tmp59 = 6 + 1;
	tmp58 = (char*) malloc(tmp59);
	strcat(tmp58, "td bem");
	tmp60[3] = tmp58;
	tmp62 = 2;
	tmp63 = 3;
	if(tmp62 < 0 || tmp62 >= tmp61) exit(1);
	if(tmp63 < 0 || tmp63 >= tmp61) exit(1);
	tmp65 = tmp63 - tmp62;
	if(tmp65 <= 0) exit(1);
	tmp65 = tmp65 + 1;
	tmp64 = tmp60 + tmp62;
	tmp67 = tmp65;
	tmp66 = tmp64;
	tmp68 = 1;
	tmp70 = strlen(tmp66[tmp68]) + 1;
	tmp69 = (char*) malloc(tmp70);
	strcat(tmp69, tmp66[tmp68]);
	tmp72 = 11 + 1;
	tmp71 = (char*) malloc(tmp72);
	strcat(tmp71, ", te perdoo");
	tmp74 = tmp70 + tmp72;
	tmp74 = tmp74 + 1;
	tmp73 = (char*) malloc(tmp74);
	strcat(tmp73, tmp69);
	strcat(tmp73, tmp71);
	cout << tmp73 << endl;
	if (tmp53 > 0) free(tmp52);
	if (tmp55 > 0) free(tmp54);
	if (tmp57 > 0) free(tmp56);
	if (tmp59 > 0) free(tmp58);
	if (tmp70 > 0) free(tmp69);
	if (tmp72 > 0) free(tmp71);
	if (tmp74 > 0) free(tmp73);
	return 0;
}
