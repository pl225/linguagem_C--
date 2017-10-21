%{
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stack>

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

typedef struct 
{
	bool quebravel;
	mapV mapaVariaveis;
	string rotuloInicio;
	string rotuloFim;
} contextoBloco;

static stack<contextoBloco> pilhaContexto;

static mapT mapaTemporario;
static mapV mapaDeclarado;

mapV controiMapaVariaveis () {
	mapV m;
	return m;
}

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
	for (mapT::iterator it = mapaTemporario.begin(); it!=mapaTemporario.end(); ++it) {
    	s += '\t' + decideTipo(it->second.tipo) + ' ' + it->second.id + ";\n";
	}
    return s;
}

int yylex(void);
void yyerror(string);

void verificaVariavelNaoDeclarada (string s) {
	stack<contextoBloco> p = pilhaContexto;
	bool v = true;
	while (not p.empty()){
		if (p.top().mapaVariaveis.find(s) != p.top().mapaVariaveis.end()) {
			v = false;
			break;
		}
	    p.pop( );
	}
	if (v) {
		yyerror("A variável "+ s + " não foi declarada.");					
	}
}

mapV buscaMapa (string s) {
	stack<contextoBloco> p = pilhaContexto;
	while (not p.empty()){
		if (p.top().mapaVariaveis.find(s) != p.top().mapaVariaveis.end()) {
			return p.top().mapaVariaveis;
		}
	    p.pop();
	}
	yyerror("A variável "+ s + " não foi declarada.");						
}

void verificaVariavelJaDeclarada (string s) {
	if (pilhaContexto.top().mapaVariaveis.find(s) != pilhaContexto.top().mapaVariaveis.end()) {
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
%token TK_IF TK_WHILE TK_BREAK TK_CONTINUE TK_DO

%start S

%left TK_MAIS_MENOS
%left TK_MULTI_DIV
%left TK_LOGICO TK_RELACIONAL
%left TK_CAST

%%

S 			: PILHA_GLOBAL BLOCO
			{
				cout << "/*Compilador FAEN*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n" << declaraVariaveisTemporarias() + $2.traducao << "\treturn 0;\n}" << endl; 
			}
			;

PILHA_GLOBAL: 
			{
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
			}

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
			| LOOP_CONTROLE ';'
			| REPITA ';'
			;

DECLARACAO  : TK_TIPO_FLUT32 TK_ID DECLARACAO_VF32 DECLARACAO_F32
			{
				if ($3.tipo != "") defineTiposCompativeis($1.tipo, $3.tipo);
				verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $1.tipo;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
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
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "" && $3.label == "") {
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + decideValorBooleano($3.traducao) + ";\n";
				} 
				else if ($3.label != "" && $3.tipo == "") {
					mapV mapa = buscaMapa($3.label);
					defineTiposCompativeis(BOOL, pilhaContexto.top().mapaVariaveis[$3.label].tipo);
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
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
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") { // caracter puro com tipo
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + $3.traducao + ";\n";
				} else if ($3.label != "") { // variavel
					mapV mapa = buscaMapa($3.label);
					defineTiposCompativeis(CHAR, mapa[$3.label].tipo);
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
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
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
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
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
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
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "" && $3.label == "") {
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + decideValorBooleano($3.traducao) + ";\n";
					} 
					else if ($3.label != "" && $3.tipo == ""){
						mapV mapa = buscaMapa($3.label);
						defineTiposCompativeis(BOOL,mapa[$3.label].tipo);
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
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
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") { // caracter puro com tipo
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + $3.traducao + ";\n";
					} else if ($3.label != "") { // variavel
						mapV mapa = buscaMapa($3.label);
						defineTiposCompativeis(CHAR, mapa[$3.label].tipo);
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
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
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
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
				mapV mapa = buscaMapa($1.label);
				$$.traducao = '\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + $3.traducao + ";\n";
			}
			|
			 TK_ID '=' TK_CHAR
			{
				mapV mapa = buscaMapa($1.label);
				$$.traducao = '\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + $3.traducao + ";\n";
			} 
			|
			TK_ID '=' E
			{
				mapV mapa = buscaMapa($1.label);
				$1.tipo = mapaTemporario[mapa[$1.label].temporario].tipo;
				defineTiposCompativeis($1.tipo, $3.tipo);
				if ($1.tipo != $3.tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();

					if ($1.tipo == INT) {
						$$.tipo = INT;
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
						'\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + varCast + ";\n";
					} else if ($1.tipo == FLUT32){
						$$.tipo = FLUT32;
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + varCast + ";\n";
					}
				} else {
					$$.traducao = $3.traducao + '\t' + mapaTemporario[mapa[$1.label].temporario].id + 
					" = " + $3.label + ";\n";
				}
			}
			|
			TK_ID '=' L
			{
				mapV mapa = buscaMapa($1.label);
				defineTiposCompativeis($1.tipo, $3.tipo);
				$$.traducao = $3.traducao + '\t' + mapaTemporario[mapa[$1.label].temporario].id + 
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
				mapV mapa = buscaMapa($1.label);
        		string var = mapa[$1.label].temporario;
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


ENQUANTO	: TK_WHILE '(' L ')' BLOCO_ITERACAO BLOCO
			{
				string varWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varWhile] = { .id = varWhile, .tipo = BOOL };
				$$.traducao = '\t' + pilhaContexto.top().rotuloInicio + ":\n" +	$3.traducao +
				"\t" + varWhile + " = !" + $3.label + ";\n" +
				"\tif (" + varWhile + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				$6.traducao + "\tgoto " + pilhaContexto.top().rotuloInicio + ";\n" +
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}

REPITA		: TK_DO BLOCO_ITERACAO BLOCO TK_WHILE '(' L ')'
			{
				string varDoWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varDoWhile] = { .id = varDoWhile, .tipo = BOOL };
				$$.traducao = '\t' + pilhaContexto.top().rotuloInicio + ":\n" + $3.traducao +
				$6.traducao + '\t' + varDoWhile + " = !" + $6.label + ";\n" + 
				"\tif (" + varDoWhile + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				"\tgoto " + pilhaContexto.top().rotuloInicio + ";\n" +
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}


BLOCO_ITERACAO:
			{
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
			}

LOOP_CONTROLE: TK_BREAK 
			{
				if (pilhaContexto.top().quebravel)
					$$.traducao = "\tgoto " + pilhaContexto.top().rotuloFim + ";\n";
				else
					yyerror("Este bloco não é quebrável.");
			}
			|
			TK_CONTINUE
			{
				if (pilhaContexto.top().quebravel)
					$$.traducao = "\tgoto " + pilhaContexto.top().rotuloInicio + ";\n";
				else
					yyerror("Este bloco não é quebrável.");
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
