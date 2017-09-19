%{
#include <iostream>
#include <string>
#include <sstream>

#define YYSTYPE atributos

using namespace std;

struct atributos
{
	string label;
	string traducao;
};

string proximaVariavelTemporaria () {
	static int n = 0;
	n++;
	return to_string(n);
}

int yylex(void);
void yyerror(string);
%}

%token TK_NUM
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLUT32 TK_TIPO_FLUT64  TK_TIPO_BOOL
%token TK_BIN TK_HEX TK_OCT
%token TK_FIM TK_ERROR

%start S

%left '+' '-'
%left '*' '/'

%%

S 			: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n" << $5.traducao << "\treturn 0;\n}" << endl; 
			}
			| BLOCO
			{
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n" << $1.traducao << "\treturn 0;\n}" << endl; 
			}
			;

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;
			}
			;

COMANDOS	: COMANDO COMANDOS
			|
			;

COMANDO 	: E ';'
			| ATRIBUICAO ';'
			;

ATRIBUICAO  : TK_TIPO_FLUT32 TK_ID '=' TK_NUM
			{
				$$.traducao = '\t' + $1.traducao + ' ' + $2.label + " = " + $4.traducao + ";\n"; 
			}
			;

E 			: E '+' E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label +  " + " + $3.label + ";\n";
			}
			|
			E '-' E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label +  " - " + $3.label + ";\n";
			}
			|
			E '*' E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label +  " * " + $3.label + ";\n";
			}
			|
			E '/' E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label +  " / " + $3.label + ";\n";
			}
			| TK_NUM
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = '\t' + $$.label + " = " + $1.traducao + ";\n";
			}
			|
			'-' E 
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $2.traducao + '\t' + $$.label + " = -" + $2.label + ";\n";
			}
			|
			'(' E ')'
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $2.traducao + '\t' + $$.label + " = " + $2.label + ";\n";
			}
			| TK_ID 
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = '\t' + $$.label + " = " + $1.label + ";\n";
			}
			;

%%

#include "lex.yy.c"

int yyparse();

int main( int argc, char* argv[] )
{
	yyparse();

	return 0;
}

void yyerror( string MSG )
{
	cout << MSG << endl;
	exit (0);
}				
