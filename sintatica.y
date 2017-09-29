%{
#include <iostream>
#include <string>
#include <sstream>
#include <map>

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

typedef struct
{
	string id;
	string tipo;	
} variavelTemporaria;

typedef struct 
{
	string id;
	string tipo;
	string temporario;
} variavelDeclarada;

typedef map<string, variavelTemporaria> mapT;
typedef map<string, variavelDeclarada> mapV;

static mapT mapaTemporario;

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

string declaraVariaveisTemporarias () {
	string s = "";
	for (mapT::iterator it = mapaTemporario.begin(); it!=mapaTemporario.end(); ++it)
    	s += '\t' + decideTipo(it->second.tipo) + ' ' + it->second.id + ";\n";
    return s;
}

int yylex(void);
void yyerror(string);
%}

%token TK_NUM TK_BOOL TK_CHAR
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
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n" << declaraVariaveisTemporarias() + $1.traducao << "\treturn 0;\n}" << endl; 
			}
			;

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;
			}
			;

COMANDOS	: COMANDO COMANDOS
			{
				$2.traducao = "";
				$$.traducao = $1.traducao + $2.traducao;
			}
			|
			;

COMANDO 	: E ';'
			| DECLARACAO ';'
			| L ';'
			;

DECLARACAO  : TK_TIPO_FLUT32 TK_ID '=' TK_NUM
			{
				$$.traducao = '\t' + $1.traducao + ' ' + $2.label + " = " + $4.traducao + ";\n";
			}
			;

E 			: E TK_MAIS_MENOS E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao;
				if ($1.tipo != $3.tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();
					$$.tipo = FLUT32;
					mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
					mapaTemporario[$$.label] = { .id = $$.label, .tipo = FLUT32 };
					if ($1.tipo == INT && $3.tipo == FLUT32) {
						$$.traducao += '\t' + varCast + " = (float) " + $1.label + ";\n" +
						'\t' + $$.label + " = " + varCast + $2.traducao + $3.label + ";\n";
					} else if ($1.tipo == FLUT32 && $3.tipo == INT) {
						$$.traducao += '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + $2.traducao + $1.label + ";\n";
					}
				} else {
					$$.tipo = $1.tipo;
					mapaTemporario[$$.label] = { .id = $$.label, .tipo = $1.tipo };
					$$.traducao += '\t' + $$.label + " = " + $1.label + $2.traducao + $3.label + ";\n";
				}
			}
			|
			E TK_MULTI_DIV E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.traducao = $1.traducao + $3.traducao;
				if ($1.tipo != $3.tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();
					$$.tipo = FLUT32;
					mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
					mapaTemporario[$$.label] = { .id = $$.label, .tipo = FLUT32 };
					if ($1.tipo == INT && $3.tipo == FLUT32) {
						$$.traducao += '\t' + varCast + " = (float) " + $1.label + ";\n" +
						'\t' + $$.label + " = " + varCast + $2.traducao + $3.label + ";\n";
					} else if ($1.tipo == FLUT32 && $3.tipo == INT) {
						$$.traducao += '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + $2.traducao + $1.label + ";\n";
					}
				} else {
					$$.tipo = $1.tipo;
					mapaTemporario[$$.label] = { .id = $$.label, .tipo = $1.tipo };
					$$.traducao += '\t' + $$.label + " = " + $1.label + $2.traducao + $3.label + ";\n";
				}
			}
			| 
			TK_NUM
			{
				$$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = '\t' + $$.label + " = " + $1.traducao + ";\n";
			}
			|
			TK_MAIS_MENOS E 
			{
				$$.tipo = $2.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = -" + $2.label + ";\n";
			}
			|
			'(' E ')'
			{
				$$.tipo = $2.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
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
				$$.tipo = $1.tipo;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = ( " + decideTipo($1.tipo) + " ) " + $2.label + ";\n";
			}
			;


R			: E TK_RELACIONAL E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = INT;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + $3.traducao + '\t' +
				$$.label + " = " + $1.label + decideOperadorRelacional($2.traducao) + $3.label + ";\n";
			}
			| R TK_RELACIONAL TK_BOOL 
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = INT;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + '\t' + $$.label + " = " + $1.label + 
				"==" + decideValorBooleano($3.traducao) + ";\n";
			}
			;

L 			: L TK_LOGICO L
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = INT;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label + 
				decideOperadorRelacional($2.traducao) + $3.label + ";\n";				
			}
			|
			'(' L ')'
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = INT;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = " + $2.label + ";\n";
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
