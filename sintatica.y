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
static mapV mapaDeclarado;

string proximaVariavelTemporaria () {
	static int n = 0;
	n++;
	return to_string(n);
}

string proximoRotulo () {
	static int m = 0;
	m++;
	return "rotulo_" + to_string(m);
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
	if (b == "falso") return "false";
	else if (b == "verdadeiro") return "true";
}

string declaraVariaveisTemporarias () {
	string s = "";
	for (mapT::iterator it = mapaTemporario.begin(); it!=mapaTemporario.end(); ++it)
    	s += '\t' + decideTipo(it->second.tipo) + ' ' + it->second.id + ";\n";
    return s;
}

int yylex(void);
void yyerror(string);

void verificaVariavelNaoDeclarada (string s) {
	if (mapaDeclarado.find(s) == mapaDeclarado.end()) {
		yyerror("A variável "+ s + " não foi declarada.");					
	}
}

void verificaVariavelJaDeclarada (string s) {
	if (mapaDeclarado.find(s) != mapaDeclarado.end()) {
		yyerror("A variável "+ s + " já foi declarada.");					
	}
}

void defineTiposCompativeis (string s1, string s2) {
	bool v = false;
	if (s1 == BOOL && s2 != BOOL)
		v = true;
	if (s1 == CHAR && s2 != CHAR)
		v = true;
	if (s1 == INT && s2 != INT && s2 != FLUT32)
		v = true;
	if (s1 == FLUT32 && s2 != INT && s2 != FLUT32)
		v = true;
	if (v)
		yyerror("As variáveis " + s1 + " e " + s2 + " não são de tipos compatíveis.");
}

%}

%token TK_NUM TK_BOOL TK_CHAR
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLUT32 TK_TIPO_FLUT64  TK_TIPO_BOOL
%token TK_TIPO_CHAR TK_MAIS_MENOS TK_MULTI_DIV
%token TK_BIN TK_HEX TK_OCT
%token TK_RELACIONAL TK_LOGICO
%token TK_FIM TK_ERROR
%token TK_IF TK_WHILE

%start S

%left TK_MAIS_MENOS
%left TK_MULTI_DIV
%left TK_LOGICO TK_RELACIONAL
%left TK_CAST

%%

S 			: BLOCO
			{
				cout << "/*Compilador FAEN*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n" << declaraVariaveisTemporarias() + $1.traducao << "\treturn 0;\n}" << endl; 
			}
			;

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;
			}
			;

COMANDOS	: COMANDO COMANDOS
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			| 
			{
				$$.traducao = "";
			}
			;

COMANDO 	: E ';'
			| DECLARACAO ';'
			| ATRIBUICAO ';'
			| L ';'
			| SE
			| ENQUANTO
			;

DECLARACAO  : TK_TIPO_FLUT32 TK_ID DECLARACAO_VF32 DECLARACAO_F32
			{
				if ($3.tipo != "") defineTiposCompativeis($1.tipo, $3.tipo);
				verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $1.tipo;
	  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					if ($3.tipo == INT) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
					} else {
						$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
					}
				} else {
					$$.traducao = $4.traducao;
				}
			}
			| TK_TIPO_BOOL TK_ID DECLARACAO_VBOOL DECLARACAO_BOOL
			{
				verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = BOOL;
	  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "" && $3.label == "") {
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + decideValorBooleano($3.traducao) + ";\n";
				} 
				else if ($3.label != "" && $3.tipo == "") {
					verificaVariavelJaDeclarada($3.label);
					defineTiposCompativeis(BOOL, mapaDeclarado[$3.label].tipo);
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapaDeclarado[$3.label].temporario].id + ";\n";
				}
				else if ($3.label != "" && $3.tipo != ""){
					$$.traducao = $4.traducao + $3.traducao + 
					'\t' + $$.label + " = " + $3.label + ";\n";
				}
				else {
					$$.traducao = $4.traducao;
				}
			}
			| TK_TIPO_CHAR TK_ID DECLARACAO_VCHAR DECLARACAO_CHAR
      		{
      			verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = CHAR;
	  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") { // caracter puro com tipo
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + $3.traducao + ";\n";
				} else if ($3.label != "") { // variavel
					verificaVariavelNaoDeclarada($3.label);
					defineTiposCompativeis(CHAR, mapaDeclarado[$3.label].tipo);
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapaDeclarado[$3.label].temporario].id + ";\n";
				} else {
					$$.traducao = $4.traducao;
				}
      		}
			| TK_TIPO_INT TK_ID DECLARACAO_VINT DECLARACAO_INT
      		{
      			if ($3.tipo != "") defineTiposCompativeis($1.tipo, $3.tipo);
      			verificaVariavelJaDeclarada($2.label);
        		$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $1.tipo;
	  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					if ($3.tipo == FLUT32) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
					} else {
						$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
					}
				} else {
					$$.traducao = $4.traducao;	
				}
      		}
			;

DECLARACAO_VF32: '=' E
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;

DECLARACAO_F32 : ',' TK_ID DECLARACAO_VF32 DECLARACAO_F32
				{
					if ($3.tipo != "") defineTiposCompativeis(FLUT32, $3.tipo);
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = FLUT32;
		  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						if ($3.tipo == INT) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
							$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
							'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
						} else {
							$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
						}
					} else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VBOOL: '=' TK_BOOL
				 {
				 	$$ = $2;
				 }
				 |
				 '=' TK_ID 
				 {
				 	$$ = $2;
				 }
				 |
				 '=' L
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_BOOL : ',' TK_ID DECLARACAO_VBOOL DECLARACAO_BOOL
				{
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = BOOL;
		  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "" && $3.label == "") {
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + decideValorBooleano($3.traducao) + ";\n";
					} 
					else if ($3.label != "" && $3.tipo == ""){
						verificaVariavelJaDeclarada($3.label);
						defineTiposCompativeis(BOOL, mapaDeclarado[$3.label].tipo);
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapaDeclarado[$3.label].temporario].id + ";\n";
					}
					else if ($3.label != "" && $3.tipo != ""){
						$$.traducao = $4.traducao + $3.traducao + 
						'\t' + $$.label + " = " + $3.label + ";\n";
					}
					else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VCHAR: '=' TK_CHAR
				 {
				 	$$ = $2;
				 }
				 |
				 '=' TK_ID 
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_CHAR : ',' TK_ID DECLARACAO_VCHAR DECLARACAO_CHAR
				{
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = CHAR;
		  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") { // caracter puro com tipo
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + $3.traducao + ";\n";
					} else if ($3.label != "") { // variavel
						verificaVariavelNaoDeclarada($3.label);
						defineTiposCompativeis(CHAR, mapaDeclarado[$3.label].tipo);
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapaDeclarado[$3.label].temporario].id + ";\n";
					} else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VINT: '=' E
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_INT : ',' TK_ID DECLARACAO_VINT DECLARACAO_INT
				{
					if ($3.tipo != "") defineTiposCompativeis(INT, $3.tipo);
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = INT;
		  			mapaDeclarado[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						if ($3.tipo == FLUT32) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
							$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
							'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
						} else {
							$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
						}
					} else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

ATRIBUICAO	: TK_ID '=' TK_BOOL
			{
				verificaVariavelNaoDeclarada($1.label);
				$$.traducao = '\t' + mapaTemporario[mapaDeclarado[$1.label].temporario].id + " = " + $3.traducao + ";\n";
			}
			|
			 TK_ID '=' TK_CHAR
			{
				verificaVariavelNaoDeclarada($1.label);
				$$.traducao = '\t' + mapaTemporario[mapaDeclarado[$1.label].temporario].id + " = " + $3.traducao + ";\n";
			} 
			|
			TK_ID '=' E
			{
				verificaVariavelNaoDeclarada($1.label);
				$1.tipo = mapaTemporario[mapaDeclarado[$1.label].temporario].tipo;
				defineTiposCompativeis($1.tipo, $3.tipo);
				if ($1.tipo != $3.tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();

					if ($1.tipo == INT) {
						$$.tipo = INT;
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
						'\t' + mapaTemporario[mapaDeclarado[$1.label].temporario].id + " = " + varCast + ";\n";
					} else if ($1.tipo == FLUT32){
						$$.tipo = FLUT32;
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + mapaTemporario[mapaDeclarado[$1.label].temporario].id + " = " + varCast + ";\n";
					}
				} else {
					$$.traducao = $3.traducao + '\t' + mapaTemporario[mapaDeclarado[$1.label].temporario].id + 
					" = " + $3.label + ";\n";
				}
			}
			|
			TK_ID '=' L
			{
				verificaVariavelNaoDeclarada($1.label);
				defineTiposCompativeis($1.tipo, $3.tipo);
				$$.traducao = $3.traducao + '\t' + mapaTemporario[mapaDeclarado[$1.label].temporario].id + 
				" = " + $3.label + ";\n";	
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

				verificaVariavelNaoDeclarada($1.label);
        		string var = mapaDeclarado[$1.label].temporario;
        		$$.label = mapaTemporario[var].id;
        		$$.tipo = mapaTemporario[var].tipo;
        		$$.traducao = "";
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
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + $3.traducao + '\t' +
				$$.label + " = " + $1.label + decideOperadorRelacional($2.traducao) + $3.label + ";\n";
			}
			| R TK_RELACIONAL TK_BOOL 
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + '\t' + $$.label + " = " + $1.label + 
				"==" + decideValorBooleano($3.traducao) + ";\n";
			}
			;

L 			: L TK_LOGICO L
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label + 
				decideOperadorRelacional($2.traducao) + $3.label + ";\n";				
			}
			|
			'(' L ')'
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = " + $2.label + ";\n";
			}
			|
			R
			;

SE  		: TK_IF '(' L ')' BLOCO
			{
				cout << "haha" << endl;
			}


ENQUANTO	: TK_WHILE '(' L ')' BLOCO
			{
				string labelWhileBegin = proximoRotulo();
				string labelWhileFim = proximoRotulo();
				string varWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varWhile] = { .id = varWhile, .tipo = BOOL };
				$$.traducao = $3.traducao +	'\t' + labelWhileBegin + ":\n" +
				"\t" + varWhile + " = !" + $3.label + ";\n" +
				"\tif (" + varWhile + ") goto " + labelWhileFim + ";\n" +
				$5.traducao + "\tgoto " + labelWhileBegin + ";\n" +
				'\t' + labelWhileFim + ":\n";
			}



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
