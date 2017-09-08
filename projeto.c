%{
	#include <stdio.h>
	#include <stdlib.h>
	#define INT 1
	#define FLOAT 2
	#define BIN 3
	#define OCT 4
	#define HEX 5
	#define BOOL 6
	#define CHAR 7
	#define NADA 8

%}

octal [0][0-7]
int1 [1-9]
int2 [0-9]
bin [b][0-1]+
hex [0x][A-F]*|[0-9]*
float [1-9][0-9]*|0
float2 [0-9]+
True "verdadeiro"
False "falso"
Letra [a-zA-z]


Inteiro "int"
PontoFlutuante "flut32"
PontoFlutuanteP "flut64"
Booleano "bool"
Caractere "char"
String "string"
Vazio "vazio"


If "se"
Else "senao"
Switch "escolha"
Case "caso"
For "para"
While "enquanto"
Each "cada"
Do "faca"
Break "pare"
Continue "continue"
Goto "vapara"


Soma "+"
Subtraçao "-"
Multiplicaçao "*"
Divisao "/"
Modulo "%"
Igual "=="
Diferente "~="
MaiorQue ">"
MenorQue "<"
MaiorOuIgualQue ">="
MenorOuIgualQue "<="


Atribui "="
MaisMais "++"
MenosMenos "--"
MaisIgual "+="
MenosIgual "-="
VezesIgual "*="
DivideIgual "/="
ModuloIgual "%=" 


And "e"
Or "ou"
Not "~"


DemilitaComando ";" 
DeliminaLinha "\n" 


Main "principal"
Retorna "retorna" 

%%

{int1}{int2}*   return INT;
{bin} return BIN;
{octal}* return OCT;
{hex}* return HEX;
{float}[\.]{float2} return FLOAT;
{True}|{False} return BOOL;  
{Letra}|{int2} return CHAR; 

.* return NADA;

%%
void main()
{
	int val, i = 0, o = 0, b = 0, h = 0, f = 0, bo = 0, c = 0;
	while ( (val = yylex()) > 0 ) {
		if (val == INT) {
			i++;
		}
		if (val == BIN) {
			b++;
		}
		if (val == OCT) {
			o++;
		}
		if (val == HEX) {
			h++;
		}
		if (val == FLOAT) {
			f++;
		}
		if (val == BOOL) {
			bo++;
		}
		if (val == CHAR) {
			c++;
		}
		printf("INT: %d\n", i);
		printf("BIN: %d\n", b);
		printf("OCT: %d\n", o);
		printf("HEX: %d\n", h);
		printf("FLOAT: %d\n", f);
		printf("BOOL: %d\n", bo);
		printf("CHAR: %d\n", c);

	}	
}
