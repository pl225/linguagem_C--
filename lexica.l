%{
	#include <string>
%}

ESPACO [ \t]
DELIM {ESPACO}+

OCT [0][0-7]+
INT [1-9][0-9]*|0
BIN [b][0-1]+
HEX [0x]([A-F]|[0-9])*
FLOAT [1-9][0-9]*[\.][0-9]+|0[\.][0-9]+

True "verdadeiro"
False "falso"
Letra [a-zA-z_]

ID {Letra}({Letra}|[0-9])*

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

Main "principal"
Retorna "retorna" 

%%

"\n"		{}

{DELIM} 	{}

[+\-*/]		{ return *yytext; }

[(){}[\];,]	{ return *yytext; }

{Atribui} { return *yytext; }

{MaiorQue}|{MaiorOuIgualQue}|{MenorQue}|{MenorOuIgualQue}|{Diferente}|{Igual} { yylval.traducao = yytext; return TK_RELACIONAL; }
{And}|{Or}|{Not} { yylval.traducao = yytext; return TK_LOGICO; }

"main"		{return TK_MAIN;}

"int"		{return TK_TIPO_INT;}

{INT}   {yylval.traducao = yytext; return TK_NUM; };

{BIN} {yylval.traducao = yytext; return TK_BIN; };

{OCT} {yylval.traducao = yytext; return TK_OCT; };

{HEX} {yylval.traducao = yytext; return TK_HEX; };

{FLOAT} { yylval.traducao = yytext; return TK_NUM; }

{True}|{False} {yylval.traducao = yytext; return TK_TIPO_BOOL; };  

{PontoFlutuante} {yylval.traducao = yytext; return TK_TIPO_FLUT32; }

{PontoFlutuanteP} {yylval.traducao = yytext; return TK_TIPO_FLUT64; }

{ID} { yylval.label = yytext; return TK_ID; } 

.			{ *yytext; }

%%