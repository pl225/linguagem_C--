/*Compilador FAEN*/
#include <iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
int main(void)
{
	int  tmp1;
	int  tmp2;
	int  tmp3;
	int  tmp4;
	int  tmp5;
	int  tmp6;
	int  tmp7;
	tmp1 = 1;
	tmp2 = tmp1;
	tmp3 = 0;
	tmp4 = tmp2>tmp3;
	tmp7 = !tmp4;
	if (tmp7) goto rotulo_3;
	tmp5 = 2;
	tmp6 = tmp5;
	cout << tmp6 << endl;
	goto rotulo_4;
	rotulo_3:
	cout << tmp2 << endl;
	rotulo_4:
	cout << tmp2 << endl;
	return 0;
}
