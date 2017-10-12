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
CHAR \'(\\.|[^'])\'

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

Castint "(int)"
Castflut32 "(flut32)"

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
Subtracao "-"
Multiplicacao "*"
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

{Soma}|{Subtracao} { yylval.traducao = yytext; return TK_MAIS_MENOS; }
{Divisao}|{Multiplicacao} { yylval.traducao = yytext; return TK_MULTI_DIV; }

[(){}[\];,]	{ return *yytext; }

{Atribui} { return *yytext; }

{If} { return TK_IF; }

{While} { return TK_WHILE; }

{MaiorQue}|{MaiorOuIgualQue}|{MenorQue}|{MenorOuIgualQue}|{Diferente}|{Igual} { yylval.traducao = yytext; return TK_RELACIONAL; }
{And}|{Or}|{Not} { yylval.traducao = yytext; return TK_LOGICO; }

"main"	{return TK_MAIN;}

{Castint} { yylval.tipo = INT; return TK_CAST; }
{Castflut32} { yylval.tipo = FLUT32; return TK_CAST; }

{Inteiro}		{yylval.traducao = yytext; yylval.tipo = INT; return TK_TIPO_INT;}

{PontoFlutuante} {yylval.traducao = yytext; yylval.tipo = FLUT32; return TK_TIPO_FLUT32; }

{PontoFlutuanteP} {yylval.traducao = yytext; yylval.tipo = FLUT32; return TK_TIPO_FLUT32; }

{Booleano} {yylval.traducao = yytext; yylval.tipo = BOOL; return TK_TIPO_BOOL; }

{Caractere} {yylval.traducao = yytext; yylval.tipo = CHAR; return TK_TIPO_CHAR; }

{INT}   {yylval.traducao = yytext; yylval.tipo = INT; return TK_NUM; }

{FLOAT} { yylval.traducao = yytext; yylval.tipo = FLUT32; return TK_NUM; }

{True}|{False} {yylval.traducao = yytext; yylval.tipo = BOOL; return TK_BOOL; }

{CHAR} {yylval.traducao = yytext; yylval.tipo = CHAR; return TK_CHAR; }

{BIN} {yylval.traducao = yytext; return TK_BIN; }

{OCT} {yylval.traducao = yytext; return TK_OCT; }

{HEX} {yylval.traducao = yytext; return TK_HEX; }

{ID} { yylval.label = yytext; yylval.tipo = ""; yylval.traducao = ""; return TK_ID; } 

.			{ *yytext; }

%%
