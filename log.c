/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SIZE_STR 10
using namespace std;

int  tmp2;


int main(void)
{
	int  tmp1;
	int  tmp10;
	int  tmp11;
	int  tmp12;
	int  tmp13;
	int  tmp14;
	int  tmp15;
	int  tmp16;
	int  tmp17;
	int  tmp3;
	int  tmp4;
	int  tmp5;
	int  tmp6;
	int  tmp7;
	int  tmp8;
	int  tmp9;
	tmp1 = 0;
	tmp2 = tmp1;
	rotulo_3:
	tmp3 = 10;
	tmp4 = tmp2<tmp3;
	tmp17 = !tmp4;
	if (tmp17) goto rotulo_4;
	cout << tmp2 << endl;
	tmp5 = 10;
	tmp6 = tmp5;
	rotulo_5:
	tmp7 = 20;
	tmp8 = tmp6<tmp7;
	tmp14 = !tmp8;
	if (tmp14) goto rotulo_6;
	tmp9 = 5;
	tmp10 = tmp2==tmp9;
	tmp11 = !tmp10;
	if (tmp11) goto rotulo_7;
	goto rotulo_4;
	rotulo_7:
	tmp12 = 1;
	tmp13 = tmp6 + tmp12;
	tmp6 = tmp13;
	cout << tmp6 << endl;
	goto rotulo_5;
	rotulo_6:
	tmp15 = 1;
	tmp16 = tmp2 + tmp15;
	tmp2 = tmp16;
	goto rotulo_3;
	rotulo_4:
	return 0;
}
