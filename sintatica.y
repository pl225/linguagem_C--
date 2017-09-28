%{
#include <iostream>
#include <string>
#include <sstream>

#define YYSTYPE atributos
#define INT "0"
#define FLUT32 "1"
#define BOOL "2"
#define CHAR "3"

using namespace std;

struct atributos
{
	string label;
	string traducao;
	string tipo;
};

string proximaVariavelTemporaria () {
	static int n = 0;
	n++;
	return to_string(n);
}

string decideTipo (string tipo) {
	if (tipo == INT) return "int ";
	else if (tipo == FLUT32) return "float ";
	else if (tipo == BOOL) return "int ";
	else if (tipo == CHAR) return "char ";
}

string decideOperadorRelacional (string op) {
	if (op == "e") return " && ";
	else if (op == "ou") return " || ";
	else if (op == "~=") return " != ";
	else if (op == "~") return " ! ";
	else return op;
}

string decideValorBooleano (string b) {
	if (b == "falso") return "0";
	else if (b == "verdadeiro") return "1";
}

int yylex(void);
void yyerror(string);
%}

%token TK_NUM
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLUT32 TK_TIPO_FLUT64  TK_TIPO_BOOL
%token TK_TIPO_CHAR TK_MAIS_MENOS TK_MULTI_DIV
%token TK_BIN TK_HEX TK_OCT
%token TK_RELACIONAL TK_LOGICO
%token TK_FIM TK_ERROR

%start S

%left TK_MAIS_MENOS
%left TK_MULTI_DIV
%left TK_LOGICO TK_RELACIONAL
%left TK_CAST

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
			| L ';'
			;

ATRIBUICAO  : TK_TIPO_FLUT32 TK_ID '=' TK_NUM
			{
				$$.traducao = '\t' + $1.traducao + ' ' + $2.label + " = " + $4.traducao + ";\n"; 
			}
			;

E 			: E TK_MAIS_MENOS E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label + 
				$2.traducao + $3.label + ";\n";
			}
			|
			E TK_MULTI_DIV E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label + 
				$2.traducao + $3.label + ";\n";
			}
			| 
			TK_NUM
			{
				$$.tipo = decideTipo($1.tipo);
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = '\t' + $$.tipo + $$.label + " = " + $1.traducao + ";\n";
			}
			|
			TK_MAIS_MENOS E 
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
			|
			TK_CAST E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = decideTipo($1.tipo);
				$$.traducao = $2.traducao + '\t' + $$.tipo + $$.label + " = ( " + $$.tipo + " ) " + $2.label + ";\n";
			}
			;
			

R			: E TK_RELACIONAL E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + "int " +
				$$.label + " = " + $1.label + decideOperadorRelacional($2.traducao) + $3.label + ";\n";
			}
			| R TK_RELACIONAL TK_TIPO_BOOL 
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + '\t' + "int " + $$.label + " = " + $1.label + 
				"==" + decideValorBooleano($3.traducao) + ";\n";
			}
			;

L 			: L TK_LOGICO L
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao + '\t' + "int " + $$.label + " = " + $1.label + 
				decideOperadorRelacional($2.traducao) + $3.label + ";\n";				
			}
			|
			'(' L ')'
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $2.traducao + '\t' + "int " + $$.label + " = " + $2.label + ";\n";
			}
			|
			R
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
