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
STRING \"([^\"\n]|\\\")*\"

True "verdadeiro"
False "falso"
Letra [a-zA-z_]

ID {Letra}({Letra}|[0-9])*

Inteiro "int"
PontoFlutuante "flut32"
PontoFlutuanteP "flut64"
Booleano "bool"
Caractere "char"
Caracteres "chars"
Vazio "vazio"
Padrao "padrao"
Funcao "funcao"

Castint "(int)"
Castflut32 "(flut32)"
Var "seja"

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

PRINT "escreva"
SCAN "leia"

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
ConcatenaIgual ".="


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
{Modulo} { yylval.traducao = yytext; return TK_MODULO; }

[(){}[\];,:]	{ return *yytext; }

{Atribui} { return *yytext; }

{Retorna} { return TK_RETORNA; }

{Switch} { return TK_SWITCH; }
{Case} { return TK_CASE; }
{Padrao} { return TK_DEFAULT; }

{If} { return TK_IF; }
{Else} { return TK_ELSE; }

{While} { return TK_WHILE; }
{Do} { return TK_DO; }
{For} { return TK_FOR; }


{MaisIgual}|{MenosIgual}|{VezesIgual}|{DivideIgual}|{ModuloIgual} { yylval.traducao = yytext; return TK_OP_ABREV; }
{MaisMais}|{MenosMenos} { yylval.traducao = yytext; return TK_OP_1; }
{ConcatenaIgual} { return TK_CONCAT_IGUAL; }

{MaiorQue}|{MaiorOuIgualQue}|{MenorQue}|{MenorOuIgualQue}|{Diferente}|{Igual} { yylval.traducao = yytext; return TK_RELACIONAL; }
{And}|{Or}|{Not} { yylval.traducao = yytext; return TK_LOGICO; }

{PRINT} { return TK_PRINT; }
{SCAN} { return TK_SCAN; }

{Var} { return TK_SEJA; }

"main"	{return TK_MAIN;}

{Break} { return TK_BREAK; }
{Continue} { return TK_CONTINUE; }

{Castint} { yylval.tipo = INT; return TK_CAST; }
{Castflut32} { yylval.tipo = FLUT32; return TK_CAST; }

{Funcao} {return TK_TIPO_FUNCAO;}

{Inteiro}		{yylval.traducao = yytext; yylval.tipo = INT; return TK_TIPO_INT;}

{PontoFlutuante} {yylval.traducao = yytext; yylval.tipo = FLUT32; return TK_TIPO_FLUT32; }

{PontoFlutuanteP} {yylval.traducao = yytext; yylval.tipo = FLUT32; return TK_TIPO_FLUT32; }

{Booleano} {yylval.traducao = yytext; yylval.tipo = BOOL; return TK_TIPO_BOOL; }

{Caractere} {yylval.traducao = yytext; yylval.tipo = CHAR; return TK_TIPO_CHAR; }

{Caracteres} {yylval.traducao = yytext; yylval.tipo = CHARS; return TK_TIPO_STRING; }

{INT}   {yylval.traducao = yytext; yylval.tipo = INT; return TK_NUM; }

{STRING} { yylval.traducao = yytext; yylval.tipo = CHARS; return TK_STRING; }

{FLOAT} { yylval.traducao = yytext; yylval.tipo = FLUT32; return TK_NUM; }

{True}|{False} {yylval.traducao = yytext; yylval.tipo = BOOL; yylval.label = ""; return TK_BOOL; }

{CHAR} {yylval.traducao = yytext; yylval.tipo = CHAR; return TK_CHAR; }

{BIN} {yylval.traducao = yytext; return TK_BIN; }

{OCT} {yylval.traducao = yytext; return TK_OCT; }

{HEX} {yylval.traducao = yytext; return TK_HEX; }

{ID} { yylval.label = yytext; yylval.tipo = ""; yylval.traducao = ""; return TK_ID; } 

"." { return TK_CONCATENACAO; }

.			{ *yytext; }

%%
